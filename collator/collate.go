package main

import (
	"fmt"
	"log"
	"net/url"
	"os"
	"time"
	"strconv"
	mqtt "github.com/eclipse/paho.mqtt.golang"
)

// Function to check for errors
func check(err error) {
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
}

func connect(clientId string, uri *url.URL) mqtt.Client {
	opts := mqtt.NewClientOptions()
	opts.AddBroker(fmt.Sprintf("tcp://%s", uri.Host))
	client := mqtt.NewClient(opts)
	token := client.Connect()
	for !token.WaitTimeout(1 * time.Millisecond) {
	}
	if err := token.Error(); err != nil {
		log.Fatal(err)
	}	
	return client
}

func listen(uri *url.URL, topic string, channel_state *string) {
	client := connect("sub", uri)
	fmt.Println(fmt.Sprintf("LISTENING TO %s", string(topic)))
	client.Subscribe(topic, 0, func(client mqtt.Client, msg mqtt.Message) {
		fmt.Println(fmt.Sprintf("%s RECEIVED", msg.Topic()))
		*channel_state = string(msg.Payload())
	})
}

func main() {
	mqtt.ERROR = log.New(os.Stdout, "[ERROR] ", 0)
	mqtt.CRITICAL = log.New(os.Stdout, "[CRIT] ", 0)
	mqtt.WARN = log.New(os.Stdout, "[WARN]  ", 0)
	
	count := 0
	
	uri, err := url.Parse("http://localhost:1883")
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(uri.Path)

	health := ""
	odom := ""

	go listen(uri, "HEALTH", &health)
	go listen(uri, "ODOM", &odom)

	client := connect("pub", uri)

	for {
		if (health == "" || odom == "") {
			time.Sleep(5*time.Millisecond)
			continue	
		}
		client.Publish("COLLATED", 0, false, fmt.Sprintf(`{"HEALTH": %s, "ODOM": %s}`, health, odom))
		time.Sleep(500*time.Millisecond)
		fmt.Println(strconv.Itoa(count))
		count++
	}

}
