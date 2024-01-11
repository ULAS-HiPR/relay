import pika
from utils.ConfigLoader import ConfigLoader

class MessageConsumer:
    def __init__(self, amqp_config, queue_name="camera"):
        self.connection = pika.BlockingConnection(pika.ConnectionParameters(
            host=amqp_config["host"],
            port=amqp_config["port"],
            virtual_host=amqp_config["virtual_host"],
            credentials=pika.PlainCredentials(amqp_config["username"], amqp_config["password"])
        ))
        self.channel = self.connection.channel()
        self.queue_name = queue_name

    def consume_messages(self, callback):
        self.channel.queue_declare(queue=self.queue_name)
        self.channel.basic_consume(queue=self.queue_name, on_message_callback=callback, auto_ack=True)
        print("Waiting for messages. To exit press CTRL+C")
        self.channel.start_consuming()

class MessageHandler:
    def callback(self, ch, method, properties, body):
        print(" [x] Received %r" % body)

def main():
    amqp_config = ConfigLoader("../config.toml").get_amqp_config()
    message_consumer = MessageConsumer(amqp_config)
    message_handler = MessageHandler()

    message_consumer.consume_messages(callback=message_handler.callback)

if __name__ == "__main__":
    main()
