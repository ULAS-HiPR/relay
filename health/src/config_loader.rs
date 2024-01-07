use std::fs;
use toml::{map::Map, Value};

#[derive(Debug)]
pub struct Config {
    data: Map<String, Value>,
}

impl Config {
    pub fn load() -> Result<Self, Box<dyn std::error::Error>> {
        let file_content = fs::read_to_string("../config.toml")?;
        let toml_value: Value = toml::from_str(&file_content)?;

        if let Value::Table(map) = toml_value {
            Ok(Config { data: map })
        } else {
            Err("Invalid TOML format".into())
        }
    }

    pub fn get_section(&self, section_name: &str) -> Option<&Value> {
        self.data.get(section_name)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_load_config() {
        let config = Config::load();

        assert!(config.is_ok());
    }

    #[test]
    fn test_get_section() {
        let config = Config::load().unwrap();

        let section = config.get_section("amqp");

        assert!(section.is_some());
    }

    #[test]
    fn test_get_section_invalid() {
        let config = Config::load().unwrap();

        let section = config.get_section("invalid");

        assert!(section.is_none());
    }    
}
