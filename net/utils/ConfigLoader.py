import toml


class ConfigLoader:
    def __init__(self, file_path):
        self.file_path = file_path
        self.config = self._load_config()

    def _load_config(self):
        try:
            with open(self.file_path, "r") as file:
                config = toml.load(file)
            return config
        except FileNotFoundError:
            raise FileNotFoundError(f"Config file not found: {self.file_path}")

    def get_amqp_config(self):
        return self.config.get("amqp", {})
