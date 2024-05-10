import numpy as np
import time
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
from utils.LogManager import LogManager
from utils.Model import Generator
from utils.DataLoader import load_single, save_result
from utils.EnviroIndices import parse_indices

WATCH_DIR = "/home/agrisat/relay/camera/results"
OUTPUT_DIR = "/home/agrisat/relay/agrinet/results"


class ImageHandler(FileSystemEventHandler):
    def __init__(self, generator, logger):
        super().__init__()
        self.generator = generator
        self.logger = logger

    def on_created(self, event):
        if event.is_directory:
            return
        if event.src_path.endswith(".jpg"):
            self.logger.info(f"New image detected: {event.src_path}")
            self.process_image(event.src_path)

    def process_image(self, image_path):
        try:
            input_image = load_single(image_path)
            input_image = np.expand_dims(input_image, 0)
            prediction = self.generator(input_image, training=False)[0]
            stamp = time.strftime("%Y%m%d-%H%M%S")
            save_result(input_image[0], prediction, f"{OUTPUT_DIR}/{stamp}/output.jpg")
            self.logger.info("Output saved")

            ndvi, ndwi, ndvi_regions, ndwi_regions = parse_indices(
                input_image[0], prediction, regions=True
            )
            ndvi = np.stack([np.zeros_like(ndvi), ndvi, np.zeros_like(ndvi)], axis=-1)
            ndwi = np.stack([np.zeros_like(ndwi), np.zeros_like(ndwi), ndwi], axis=-1)
            save_result(ndvi, ndwi, f"{OUTPUT_DIR}/{stamp}/indices.jpg")
            self.logger.info("Indices saved")

            ndvi_regions = ndvi_regions * 255
            ndwi_regions = ndwi_regions * 255
            save_result(ndvi_regions, ndwi_regions, f"{OUTPUT_DIR}/{stamp}/regions.jpg")
            self.logger.info("Regions saved")
        except Exception as e:
            self.logger.error(f"Error processing image: {e}")


def main():
    logger = LogManager.get_logger("AGRINET INFERENCE")
    logger.info("Warming up...")

    generator = Generator()
    try:
        generator.load_weights("generator_weights")
        logger.debug("Model loaded")
    except Exception as e:
        logger.critical("Error while loading weights: {}".format(e))

    event_handler = ImageHandler(generator, logger)
    observer = Observer()
    observer.schedule(event_handler, path=WATCH_DIR, recursive=True)
    observer.start()

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        observer.stop()
    observer.join()


if __name__ == "__main__":
    main()
