import time
from typing import Iterator
from inputs import Inputs
try:
    from serial import Serial
except:
    pass

class InputSerial(Inputs):
    def __init__(self) -> None:
        self.stream = Serial(port='/dev/ttyS0')

    def __del__(self) -> None:
        self.stream.close()

    def get_data(self) -> Iterator[str]:
        data = ''
        while True:
            c = self.stream.read(1).decode('utf-8')
            if c != '|':
                data += c
            else:
                yield data
                data = ''
