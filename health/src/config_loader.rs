use config::{Config, File};
use serde::Deserialize;

#[derive(Debug, Deserialize)]
pub struct AmqpConfig {
    pub host: String,
    pub port: u16,
    pub virtual_host: String,
    pub username: String,
    pub password: String,
}

#[derive(Debug, Deserialize)]
pub struct AppConfig {
    pub amqp: AmqpConfig,
}

pub trait ConfigLoader {
    fn load_config(&self) -> Result<AppConfig, config::ConfigError>;
}

pub struct ProductionConfigLoader;

impl ConfigLoader for ProductionConfigLoader {
    fn load_config(&self) -> Result<AppConfig, config::ConfigError> {
        let mut settings = Config::default();
        settings.merge(File::with_name("../config")).unwrap();
        settings.try_into()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_load_config() {
        let config_loader: Box<dyn ConfigLoader> = Box::new(ProductionConfigLoader);
        let result = config_loader.load_config();

        assert!(result.is_ok());

        let app_config = result.unwrap();

        assert_eq!(app_config.amqp.host, "localhost");
        assert_eq!(app_config.amqp.port, 5672);
        assert_eq!(app_config.amqp.virtual_host, "%2f");
        assert_eq!(app_config.amqp.username, "guest");
        assert_eq!(app_config.amqp.password, "guest");
    }
}