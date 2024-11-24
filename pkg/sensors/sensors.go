package sensors

// InitSensors initializes all sensors
func InitSensors() error {
	if err := InitAltimeter(); err != nil {
		return err
	}
	if err := InitGPS(); err != nil {
		return err
	}
	if err := InitRadio(); err != nil {
		return err
	}
	return nil
}

// ReadAll reads data from all sensors
func ReadAll() (map[string]string, error) {
	data := make(map[string]string)
	var err error

	data["altimeter"], err = ReadAltimeter()
	if err != nil {
		return nil, err
	}
	data["gps"], err = ReadGPS()
	if err != nil {
		return nil, err
	}
	data["radio"], err = ReadRadio()
	if err != nil {
		return nil, err
	}

	return data, nil
}
