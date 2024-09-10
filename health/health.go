package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"strconv"
	"strings"
	"time"

	"github.com/eclipse/paho.mqtt.golang"
)

// Function to check for errors
func check(err error) {
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
}

func publishHealthData(client mqtt.Client) {
	for {
		cpuTemp := GetCPUtemp()
		cpuUsage := GetCPUusage()

		healthData := fmt.Sprintf(`{"cpu_temp": %.4f, "cpu_usage": %.4f}`, cpuTemp, cpuUsage)

		// Publish the health data to the "HEALTH" topic
		token := client.Publish("HEALTH", 0, false, healthData)
		token.Wait()

		fmt.Println("Published health data:", healthData)

		// Sleep for 10 seconds before sending the next update
		time.Sleep(3 * time.Millisecond)
	}
}

func main() {
	opts := mqtt.NewClientOptions()
	opts.AddBroker("localhost:1883")  
	opts.SetClientID("health")

	client := mqtt.NewClient(opts)
	if token := client.Connect(); token.Wait() && token.Error() != nil {
		panic(token.Error())
	}
	fmt.Println("Connected to MQTT broker")

	publishHealthData(client)
}

func readFile(filename string) string {
	content, err := ioutil.ReadFile(filename)
	check(err)
	return strings.TrimSpace(string(content))
}

// Function to get CPU temperature
func GetCPUtemp() float64 {
	// CPU temperature is stored in /sys/class/thermal/thermal_zone0/temp
	tempStr := readFile("/sys/class/thermal/thermal_zone0/temp")

	// Convert the temperature to Celsius (tempStr is in millidegrees Celsius)
	tempInt, err := strconv.Atoi(tempStr)
	check(err)
	tempCelsius := float64(tempInt) / 1000.0

	return tempCelsius
}

// Function to get CPU usage
func GetCPUusage() float64 {
	content := readFile("/proc/stat")

	lines := strings.Split(content, "\n")
	firstLineFields := strings.Fields(lines[0])
	var total, idle int
	for i := 1; i < len(firstLineFields); i++ {
		value, err := strconv.Atoi(firstLineFields[i])
		check(err)
		total += value
		if i == 4 {
			idle = value
		}
	}
	idlePercentage := 100.0 * (float64(idle) / float64(total))
	usagePercentage := 100.0 - idlePercentage

	return usagePercentage
}