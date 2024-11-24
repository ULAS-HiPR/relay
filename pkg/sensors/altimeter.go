package sensors

import (
	"log"
)

// InitAltimeter initializes the altimeter sensor
func InitAltimeter() error {
	log.Println("Altimeter initialized")
	return nil
}

// ReadAltimeter reads data from the altimeter
func ReadAltimeter() (string, error) {
	data := "altimeter_data"
	return data, nil
}
