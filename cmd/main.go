package main

import (
	"fmt"
	"time"

	"github.com/ULAS-HiPR/sat-x/internal/config"
	"github.com/ULAS-HiPR/sat-x/internal/utils"
	"github.com/ULAS-HiPR/sat-x/pkg/sensors"
	"github.com/ULAS-HiPR/sat-x/pkg/storage"

	"github.com/ULAS-HiPR/sat-x/internal/logging"
	"github.com/ULAS-HiPR/sat-x/pkg/telemetry"
)

func main() {
	cfg, err := config.LoadConfig("config.yaml")
	if err != nil {
		fmt.Printf("Error loading config: %v\n", err)
		return
	}

	logging.InitLogger(cfg)
	log := logging.GetLogger()

	// Local backup
	if err := storage.InitDB(); err != nil {
		log.Fatalf("Failed to initialize database: %v", err)
	}

	if err := sensors.InitSensors(); err != nil {
		log.Fatalf("Failed to initialize sensors: %v", err)
	}

	if err := telemetry.InitQueue(cfg); err != nil {
		log.Fatalf("Failed to initialize telemetry queue: %v", err)
	}

	for {
		data, err := sensors.ReadAll()
		if err != nil {
			log.Printf("Failed to read sensor data: %v", err)
			continue
		}

		// Save data locally
		if err := storage.SaveData(data); err != nil {
			log.Printf("Failed to save data locally: %v", err)
		}

		// Send data to telemetry queue with retry mechanism
		err = utils.Retry(func() error {
			return telemetry.PublishData(data)
		}, 5, time.Second*2)
		if err != nil {
			log.Printf("Failed to send data to telemetry queue: %v", err)
		}

		time.Sleep(time.Second * time.Duration(cfg.Sensors.FrequencySeconds))
	}
}
