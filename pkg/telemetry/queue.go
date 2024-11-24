package telemetry

import (
	"log"

	"github.com/ULAS-HiPR/sat-x/internal/config"
	"github.com/streadway/amqp"
)

var conn *amqp.Connection
var ch *amqp.Channel

// InitQueue initializes the telemetry queue
func InitQueue(cfg *config.Config) error {
	var err error
	conn, err = amqp.Dial(cfg.RMQ.ConnStr)
	if err != nil {
		return err
	}
	ch, err = conn.Channel()
	if err != nil {
		return err
	}
	_, err = ch.QueueDeclare(
		"telemetry", // name
		true,        // durable
		false,       // delete when unused
		false,       // exclusive
		false,       // no-wait
		nil,         // arguments
	)
	if err != nil {
		return err
	}
	log.Println("Telemetry queue initialized")
	return nil
}
