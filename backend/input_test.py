import time
from typing import Iterator
from inputs import Inputs

class InputTest(Inputs):
    def get_data(self) -> Iterator[str]:
        while True:
            data = str(time.time()) + ','
            for i in range(0, 11):
                data += str(i) + ','
            yield data + '13.2'


