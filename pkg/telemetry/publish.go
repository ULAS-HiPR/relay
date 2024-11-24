package telemetry

import (
	"log"

	"github.com/streadway/amqp"
)

// PublishData sends data to the telemetry queue
func PublishData(data map[string]string) error {
	body := ""
	for key, value := range data {
		body += key + ": " + value + "\n"
	}
	err := ch.Publish(
		"",          // exchange
		"telemetry", // routing key
		false,       // mandatory
		false,       // immediate
		amqp.Publishing{
			ContentType: "text/plain",
			Body:        []byte(body),
		})
	if err != nil {
		return err
	}
	log.Println("Data sent to telemetry queue")
	return nil
}
