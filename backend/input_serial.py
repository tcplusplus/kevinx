import time
from typing import Iterator
from inputs import Inputs
try:
    from serial import Serial
except:
    pass  # rpi only

class InputSerial(Inputs):
    def __init__(self) -> None:
        self.stream = Serial(port='/dev/ttyS0')

    def get_data(self) -> Iterator[str]:
        data = ''
        first_received = False
        while True:
            try:
                c = self.stream.read(1).decode('utf-8')
                if c != '|' and first_received:
                    data += c
                elif first_received:
                    yield data
                    data = ''
                else:
                    first_received = True
            except:
                self.stream.close()
                self.stream = Serial(port='/dev/ttyS0')

