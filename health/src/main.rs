use config_loader::Config;

mod config_loader;
mod system_info;

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    let app_config = Config::load()?;

    let addr = format!(
        "amqp://{}:{}@{}:{}/{}",
        app_config
            .get_section("amqp")
            .and_then(|amqp| amqp.get("username"))
            .and_then(|value| value.as_str())
            .unwrap_or_default(),
        app_config
            .get_section("amqp")
            .and_then(|amqp| amqp.get("password"))
            .and_then(|value| value.as_str())
            .unwrap_or_default(),
        app_config
            .get_section("amqp")
            .and_then(|amqp| amqp.get("host"))
            .and_then(|value| value.as_str())
            .unwrap_or_default(),
        app_config
            .get_section("amqp")
            .and_then(|amqp| amqp.get("port"))
            .and_then(|value| value.as_integer())
            .unwrap_or_default(),
        app_config
            .get_section("amqp")
            .and_then(|amqp| amqp.get("virtual_host"))
            .and_then(|value| value.as_str())
            .unwrap_or_default()
    );

    let conn = lapin::Connection::connect(&addr, lapin::ConnectionProperties::default()).await?;
    let channel = conn.create_channel().await?;
    let queue_name = "health";
    channel
        .queue_declare(queue_name, Default::default(), Default::default())
        .await?;

    let duration = std::time::Duration::from_secs(1);
    loop {
        let current_time = std::time::SystemTime::now()
            .duration_since(std::time::UNIX_EPOCH)
            .unwrap()
            .as_secs();
        let system_info = system_info::gather_system_info().await?;

        let payload = system_info::format_payload(&system_info, current_time);

        channel
            .basic_publish(
                "",
                queue_name,
                lapin::options::BasicPublishOptions::default(),
                payload.as_bytes(),
                lapin::BasicProperties::default(),
            )
            .await?;

        tokio::time::sleep(duration).await;
    }
}
