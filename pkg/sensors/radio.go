package sensors

import (
	"log"
)

// InitRadio initializes the radio module
func InitRadio() error {
	log.Println("Radio module initialized")
	return nil
}

// ReadRadio reads data from the radio module
func ReadRadio() (string, error) {
	data := "radio_data"
	return data, nil
}
