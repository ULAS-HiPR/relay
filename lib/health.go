package health

import (
	"fmt"
	"io/ioutil"
	"os"
	"strconv"
	"strings"
)

// Function to check for errors
func check(err error) {
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
}

// Function to read a file and return its content as a string
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

	// Calculate CPU usage percentage
	usage := 100.0 * (float64(total-idle) / float64(total))

	return usage
}