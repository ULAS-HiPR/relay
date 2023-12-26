use tokio::time::{sleep, Duration};
use std::time;
use lapin::options::BasicPublishOptions;
use lapin::{BasicProperties, Connection, ConnectionProperties};
use config_loader::{ConfigLoader, ProductionConfigLoader};

mod config_loader;
mod system_info;

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    let config_loader: Box<dyn ConfigLoader> = Box::new(ProductionConfigLoader);
    let app_config = config_loader.load_config()?;

    let addr = format!(
        "amqp://{}:{}@{}:{}/{}",
        app_config.amqp.username,
        app_config.amqp.password,
        app_config.amqp.host,
        app_config.amqp.port,
        app_config.amqp.virtual_host
    );

    let conn = Connection::connect(&addr, ConnectionProperties::default()).await?;
    let channel = conn.create_channel().await?;
    let queue_name = "health";
    channel.queue_declare(queue_name, Default::default(), Default::default()).await?;

    let duration = Duration::from_secs(1);
    loop {
        let current_time = time::SystemTime::now().duration_since(time::UNIX_EPOCH).unwrap().as_secs();
        let system_info = system_info::gather_system_info().await?;

        let payload = system_info::format_payload(&system_info, current_time);

        channel.basic_publish(
            "",
            queue_name,
            BasicPublishOptions::default(),
            payload.as_bytes(),
            BasicProperties::default(),
        ).await?;

        println!("[{}] Payload sent", current_time);

        sleep(duration).await;
    }
}
