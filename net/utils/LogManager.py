import logging
import coloredlogs


class LogManager:
    _shared_state = {}

    def __init__(self, name, level=logging.DEBUG):
        self.__dict__ = self._shared_state
        if not self._shared_state:
            self.logger = logging.getLogger(name)
            self.logger.setLevel(level)
            self.formatter = logging.Formatter(
                "%(asctime)s - %(name)s - %(levelname)s - %(message)s"
            )
            self.consoleHandler = logging.StreamHandler()
            self.consoleHandler.setFormatter(self.formatter)
            self.logger.addHandler(self.consoleHandler)
            coloredlogs.install(level=level, logger=self.logger)

    def get_logger(self):
        return self.logger
