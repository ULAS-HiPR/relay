import logging
import colorlog


class LogManager:
    __instance = None

    @staticmethod
    def get_logger(name=None):
        if LogManager.__instance is None:
            LogManager(name)
        elif not LogManager.__instance.log.handlers:
            # Add a new handler only if no handlers are present
            formatter = colorlog.ColoredFormatter(
                "%(asctime)s [%(name)s] %(log_color)s%(levelname)s%(reset)s - %(message)s",
                log_colors={
                    "DEBUG": "cyan",
                    "INFO": "green",
                    "WARNING": "yellow",
                    "ERROR": "red",
                    "CRITICAL": "red,bg_white",
                },
            )
            handler = logging.StreamHandler()
            handler.setFormatter(formatter)
            LogManager.__instance.log.addHandler(handler)

        return LogManager.__instance.log

    def __init__(self, name=None):
        if LogManager.__instance is not None:
            raise Exception("This class is a singleton!")
        else:
            LogManager.__instance = self
            self.log = logging.getLogger(name if name else __name__)

            self.log.setLevel(logging.DEBUG)

            # Only set up handlers if they are not already configured
            if not self.log.handlers:
                formatter = colorlog.ColoredFormatter(
                    "%(asctime)s [%(name)s] %(log_color)s%(levelname)s%(reset)s - %(message)s",
                    log_colors={
                        "DEBUG": "cyan",
                        "INFO": "green",
                        "WARNING": "yellow",
                        "ERROR": "red",
                        "CRITICAL": "red,bg_white",
                    },
                )
                handler = logging.StreamHandler()
                handler.setFormatter(formatter)
                self.log.addHandler(handler)

    def set_level(self, level):
        self.log.setLevel(level)
