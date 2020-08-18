from typing import Dict, Any
from rocketdata import RocketData

class Recorder:
    def __init__(self) -> None:
        self.enabled = False

    def set_enabled(self, enabled) -> None:
        self.enabled = enabled

    def name(self) -> str:
        raise NotImplementedError()

    def new_data(self, data: RocketData) -> None:
        raise NotImplementedError()