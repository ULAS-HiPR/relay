import toml

class ConfigLoader:
    def __init__(self, file_path):
        self.config = self._load_config(file_path)

    def _load_config(self, file_path):
        try:
            with open(file_path, "r") as file:
                config = toml.load(file)
            return config
        except FileNotFoundError:
            raise FileNotFoundError(f"Config file not found: {file_path}")

    def get_amqp_config(self):
        return self.config.get("amqp", {})
