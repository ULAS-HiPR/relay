package config

import (
	"github.com/spf13/viper"
)

type LogConfig struct {
	LogPath    string `mapstructure:"log_path"`
	MaxSize    int    `mapstructure:"max_size"`
	MaxBackups int    `mapstructure:"max_backups"`
	MaxAge     int    `mapstructure:"max_age"`
	Compress   bool   `mapstructure:"compress"`
	LogLevel   string `mapstructure:"log_level"`
}

type SensorConfig struct {
	FrequencySeconds int `mapstructure:"frequency_seconds"`
}

type RMQConfig struct {
	ConnStr string `mapstructure:"connection_string"`
}

type Config struct {
	Logs    LogConfig    `mapstructure:"logs"`
	Sensors SensorConfig `mapstructure:"sensors"`
	RMQ     RMQConfig    `mapstructure:"rmq"`
}

func LoadConfig(path string) (*Config, error) {
	viper.SetConfigFile(path)
	err := viper.ReadInConfig()
	if err != nil {
		return nil, err
	}

	var config Config
	err = viper.Unmarshal(&config)
	if err != nil {
		return nil, err
	}

	return &config, nil
}
