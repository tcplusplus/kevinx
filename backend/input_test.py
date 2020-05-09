import time
from typing import Iterator
from inputs import Inputs

class InputTest(Inputs):
    def get_data(self) -> Iterator[str]:
        while True:
            for i in range(0, 11):
                data = str(i) + ',' + str(time.time())
                yield data


