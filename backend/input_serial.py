import time
from typing import Iterator
from inputs import Inputs
from serial import Serial

class InputSerial(Inputs):
    def __init__(self) -> None:
        self.stream = Serial(port='/dev/ttyS0')

    def get_data(self) -> Iterator[str]:
        data = ''
        first_received = False
        while True:
            c = self.stream.read(1).decode('utf-8')
            if c != '|' and first_received:
                data += c
            elif first_received:
                yield data
                data = ''
            else:
                first_received = True
