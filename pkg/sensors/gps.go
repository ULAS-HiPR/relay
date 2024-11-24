package sensors

import (
	"log"
)

// InitGPS initializes the GPS module
func InitGPS() error {
	log.Println("GPS module initialized")
	return nil
}

// ReadGPS reads data from the GPS module
func ReadGPS() (string, error) {
	data := "gps_data"
	return data, nil
}
