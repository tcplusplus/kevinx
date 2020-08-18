from typing import Dict, Any
from recorder import Recorder
from rocketdata import RocketData

class LogRecorder(Recorder):
    def __init__(self) -> None:
        super().__init__()
        self.enabled = True

    def name(self) -> str:
        return 'log'

    def new_data(self, data: RocketData) -> None:
        print('Data: ' + str(data))
