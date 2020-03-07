from typing import Iterator

class Inputs:
    def get_data(self) -> Iterator[str]:
        raise NotImplementedError()
