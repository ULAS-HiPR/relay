package main

import (
	"bufio"
	"fmt"
	"os/exec"
	"time"
	"sync"
	
)

func readOutput(cmd *exec.Cmd, sensorName string,  outputChan chan<- string, wg *sync.WaitGroup) {
	defer wg.Done()

	stdout, err := cmd.StdoutPipe()
	if err != nil {
		fmt.Printf("Error getting stdout pipe for %s: %v\n", sensorName, err)
		return
	}

	if err := cmd.Start(); err != nil {
		fmt.Printf("Error starting command %s: %v\n", sensorName, err)
		return
	}

	scanner := bufio.NewScanner(stdout)
	for scanner.Scan() {
		outputChan <- fmt.Sprintf("%s: %s", sensorName, scanner.Text())
	}

	if err := cmd.Wait(); err != nil {
		fmt.Printf("Command %s finished with error: %v\n", sensorName, err)
	}
}

func processSensorData(output string, radioChan chan<- string) {
	// Process sensor data and prepare radio data
	// For demonstration, let's just concatenate sensor data
	radioData := "Processed data: " + output

	// Send radio data to radio sensor
	radioChan <- radioData
}

func main() {
	odometryCmd := exec.Command("sudo","./odometry/odometryMain")
	

	gpsOutputChan := make(chan string)
	//altimeterOutputChan := make(chan string)
	//radioChan := make(chan string)
//
	var wg sync.WaitGroup
	wg.Add(2)
//
	go readOutput(odometryCmd, "odometry", gpsOutputChan, &wg)
	

	go func() {
		for {
			select {
			case gpsOutput := <-gpsOutputChan:
				fmt.Println(gpsOutput)
			}
			//	processSensorData(gpsOutput, radioChan)
			//case altimeterOutput := <-altimeterOutputChan:
			//	fmt.Println(altimeterOutput)
			//	processSensorData(altimeterOutput, radioChan)
			//}
		}
	}()

	// Radio sensor
	//go func() {
	//	for {
	//		select {
	//		case radioData := <-radioChan:
	//			fmt.Println("Radio data:", radioData)
	//			// Here, you can implement logic to send radio data to the radio sensor program
	//		}
	//	}
	//}()

	wg.Wait()
}
