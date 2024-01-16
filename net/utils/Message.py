import pika
import base64
import numpy as np

from .LogManager import LogManager

log = LogManager("Message").get_logger()


class MessageConsumer:
    def __init__(self, amqp_config, queue_name="camera"):
        self.connection = pika.BlockingConnection(
            pika.ConnectionParameters(
                host=amqp_config["host"],
                port=amqp_config["port"],
                virtual_host=amqp_config["virtual_host"],
                credentials=pika.PlainCredentials(
                    amqp_config["username"], amqp_config["password"]
                ),
            )
        )
        self.channel = self.connection.channel()
        self.queue_name = queue_name

    def consume_messages(self, callback):
        self.channel.queue_declare(queue=self.queue_name)
        self.channel.basic_consume(
            queue=self.queue_name, on_message_callback=callback, auto_ack=True
        )

        log.info("Waiting for messages. To exit press CTRL+C")
        self.channel.start_consuming()


class MessageHandler:
    def validate_msg(self, message):
        raw_message = base64.b64decode(message)
        message = np.frombuffer(raw_message, dtype=np.float32)

        return np.reshape(message, (256, 256, 3))

    def callback(self, ch, method, properties, body):
        body = self.validate_msg(body)
        log.debug(f"Received frame with shape {body.shape}")
