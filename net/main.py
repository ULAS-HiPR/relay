from utils.ConfigLoader import ConfigLoader
from utils.Message import MessageConsumer, MessageHandler


def main():
    amqp_config = ConfigLoader("../config.toml").get_amqp_config()

    message_consumer = MessageConsumer(amqp_config)
    message_handler = MessageHandler()

    message_consumer.consume_messages(callback=message_handler.callback)


if __name__ == "__main__":
    main()
