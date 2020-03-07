from typing import Optional
import argparse
import asyncio
from inputs import Inputs
from input_test import InputTest
from input_serial import InputSerial
from rocketdata import RocketData
from webserver import WebServer
from rocketsocket import RocketSocket
from networkserver import NetworkServer

class Server(NetworkServer):
    def __init__(self, test_mode: bool) -> None:
        super().__init__()
        self.test_mode = test_mode
        handlers = [(r'/socket', RocketSocket, dict(server=self))] 
        self.webserver = WebServer(port=5678, handlers=handlers)
        self.source: Optional[Inputs] = None
        if test_mode:
            self.source = InputTest()
        else:
            self.source = InputSerial()
    
    async def run_main(self) -> None:
        for data in self.source.get_data():
            rocket_data = RocketData(datastream=data)
            # print(rocket_data)
            for socket in self.sockets:
                socket.write_message(rocket_data.to_json())
            await asyncio.sleep(0.001)

    def run(self) -> None:
        self.webserver.run(callback=self.run_main)


def parse_input() -> argparse.Namespace:
    """
    parse argments
    """
    parser = argparse.ArgumentParser(description='KevinX backend')
    parser.add_argument('--test', '-t', default=False, help='Using test data', action='store_true')
    return parser.parse_args()

if __name__ == '__main__':
    args = parse_input()
    server = Server(test_mode=args.test)
    server.run()
