import pika
import time

import base64
import numpy as np

from utils.ConfigLoader import ConfigLoader
from utils.LogManager import LogManager


def load_base64_encoded_array():
    array_shape = (256, 256, 3)
    random_array = np.random.randint(0, 256, array_shape, dtype=np.uint8)
    random_array = random_array.astype(np.float32)

    array_bytes = random_array.tobytes()
    b64_encoded = base64.b64encode(array_bytes).decode("utf-8")

    return b64_encoded


def send_to_camera_queue(base64_data, config):
    amqp_config = config.get_amqp_config()

    connection_params = pika.ConnectionParameters(
        host=amqp_config.get("host", "localhost"),
        port=amqp_config.get("port", 5672),
        credentials=pika.PlainCredentials(
            username=amqp_config.get("username", "guest"),
            password=amqp_config.get("password", "guest"),
        ),
    )

    connection = pika.BlockingConnection(connection_params)
    channel = connection.channel()

    channel.queue_declare(queue="camera")

    channel.basic_publish(
        exchange="",
        routing_key="camera",
        body=base64_data,
        properties=pika.BasicProperties(
            delivery_mode=2,
        ),
    )

    connection.close()


if __name__ == "__main__":
    log = LogManager("DummyProducer").get_logger()
    config_loader = ConfigLoader("../config.toml")
    base64_data = load_base64_encoded_array()

    while True:
        send_to_camera_queue(base64_data, config_loader)
        img_size_in_bytes = len(base64_data.encode("utf-8"))
        log.info(f"Sent {img_size_in_bytes} bytes frame to camera queue")
        time.sleep(1)
