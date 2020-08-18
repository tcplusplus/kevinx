from typing import Dict, Any
import time
from recorder import Recorder
from rocketdata import RocketData

class CSVRecorder(Recorder):
    def __init__(self) -> None:
        super().__init__()
        self.enabled = False
        self.fptr = None
        self.first = False
        self.last_time = 0.0

    def set_enabled(self, enabled) -> None:
        self.enabled = enabled
        if enabled:
            self.fptr = open('/var/www/html/recordings/recording-' + str(time.time()) + '.csv', 'w')
            self.first = True
        else:
            close(self.fptr)
            self.first = False

    def name(self) -> str:
        return 'CSV'

    def new_data(self, data: RocketData) -> None:
        if self.first:
            self.keys = [key for key in data.to_dict().keys() if key != 'recorders']
            self.fptr.write(','.join(self.keys) + '\n')
            self.first = False
        dict_data = data.to_dict()
        if self.last_time != data.time:
            self.fptr.write(','.join([str(dict_data[key]) for key in self.keys]) + '\n')
            self.last_time = data.time
