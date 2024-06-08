package main

import (
	"bufio"
	"fmt"
	"os/exec"
	"time"
	Health "./lib"
	"strings"
)

type AltitudeData struct {
    Temperature float64
    Pressure    float64
    Altitude    float64
}

type GPSData struct {
    Fix            bool
    Latitude       float64
    Longitude      float64
    GeoidHeight    float64
    GPSAltitude    float64
    Speed          float64
    FixQuality     int
    Satellites     int
}

type OdometryData struct {
	AltData AltitudeData
	GpsData GPSData
}

type healthData struct {
	usage float64
	temp float64
}

type Data struct {
	health healthData
	odo OdometryData
}

func readOdometry(cmd *exec.Cmd, odometryCh chan<- OdometryData) {
	stdout, err := cmd.StdoutPipe()
	if err != nil {
        fmt.Println("Error getting stdout pipe:", err)
        return
    }

    if err := cmd.Start(); err != nil {
        fmt.Println("Error starting command:", err)
        return
    }

    scanner := bufio.NewScanner(stdout)
	var altitudeData AltitudeData
	var gpsData GPSData
	var altitudeReceived, gpsReceived bool

	for scanner.Scan() {
		line := scanner.Text()
		if strings.HasPrefix(line, "Temperature:") {
			fmt.Sscanf(line, "Temperature: %f C", &altitudeData.Temperature)
		} else if strings.HasPrefix(line, "Pressure:") {
			fmt.Sscanf(line, "Pressure: %f Pa", &altitudeData.Pressure)
		} else if strings.HasPrefix(line, "Altitude:") {
			fmt.Sscanf(line, "Altitude: %f m", &altitudeData.Altitude)
			altitudeReceived = true
		} else if strings.HasPrefix(line, "Fix:") {
			gpsData.Fix = strings.Contains(line, "Yes")
			gpsReceived = true
		} else if strings.HasPrefix(line, "Latitude:") {
			fmt.Sscanf(line, "Latitude: %f", &gpsData.Latitude)
		} else if strings.HasPrefix(line, "Longitude:") {
			fmt.Sscanf(line, "Longitude: %f", &gpsData.Longitude)
		} else if strings.HasPrefix(line, "Geoid Height:") {
			fmt.Sscanf(line, "Geoid Height: %f", &gpsData.GeoidHeight)
		} else if strings.HasPrefix(line, "AltitudeGPS:") {
			fmt.Sscanf(line, "AltitudeGPS: %f", &gpsData.GPSAltitude)
		} else if strings.HasPrefix(line, "Speed:") {
			fmt.Sscanf(line, "Speed: %f", &gpsData.Speed)
		} else if strings.HasPrefix(line, "Fix Quality:") {
			fmt.Sscanf(line, "Fix Quality: %d", &gpsData.FixQuality)
		} else if strings.HasPrefix(line, "Satellites:") {
			fmt.Sscanf(line, "Satellites: %d", &gpsData.Satellites)
		}

		//might but a if gps true & satalite 0, reset, cause than happens sometimes? (on c side tho??)
		if altitudeReceived && gpsReceived {
			odometryCh <- OdometryData{AltData: altitudeData, GpsData: gpsData}
			altitudeReceived, gpsReceived = false, false
		}
	}
}

func sendToRadio(ch <-chan Data) {
	for {
		select {
		case data := <-ch:
			fmt.Println("Sending Data:")
			fmt.Println("Health:", data.health)
			fmt.Println("Altitude:", data.odo.AltData)
			fmt.Println("GPS:", data.odo.GpsData)
		}
	}
}

func piStats(ch chan<- healthData) {
	for {
	cpuTemp := Health.GetCPUtemp()
	cpuUsage := Health.GetCPUusage()
	ch <- healthData{usage: cpuUsage, temp: cpuTemp}
	time.Sleep(100 * time.Millisecond)
	}
}

func main() {
	//starts gps & altimeter c++ code
	odometryCmd := exec.Command("sudo", "./odometry/odometryMain")

	//every channel is 1 length so I can use len() to see if its full or not
	healthOutputChan := make(chan healthData, 1)
	odometryCh := make(chan OdometryData, 1)
	combinedCh := make(chan Data, 1)

	// starts reading in values from c++ code (cmd line)
	go readOdometry(odometryCmd, odometryCh)
	// starts getting pi health stats
	go piStats(healthOutputChan)
	go sendToRadio(combinedCh)

	for{
		//sends data to sensors when ever piece of data is ready 
		//time limited by c code (every 0.5s once gps )
		if (len(healthOutputChan) == 1 && len(odometryCh) == 1) {
			combinedCh <- Data{health: <-healthOutputChan , odo: <-odometryCh}
		}
	}
}

