from typing import Optional
import argparse
import asyncio
import signal
import logging
import os
from inputs import Inputs
from input_test import InputTest
from input_serial import InputSerial
from rocketdata import RocketData
from webserver import WebServer
from rocketsocket import RocketSocket
from networkserver import NetworkServer

def signal_handler(sign, frame) -> None:      # pragma: no cover
    """
    make sure the program stops when doing crtl-c
    """
    logging.info('You pressed Ctrl+C! ')
    os.kill(os.getpid(), 9)


class Server(NetworkServer):
    def __init__(self, test_mode: bool) -> None:
        super().__init__()
        self.test_mode = test_mode
        handlers = [(r'/socket', RocketSocket, dict(server=self))] 
        self.webserver = WebServer(port=8080, handlers=handlers)
        self.source: Inputs = InputTest if test_mode else InputSerial()

    async def run_main(self) -> None:
        rocket_data = RocketData()
        for data in self.source.get_data():
            try:
                rocket_data.update(datastream=data)
                print('data: ' + str(rocket_data))
                for socket in self.sockets:
                    try:
                        socket.write_message(rocket_data.to_json())
                    except Exception as error:
                        self.remove_socket(socket)
            except Exception as error:
                print('Could not parse ' + str(error))
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
    signal.signal(signal.SIGINT, signal_handler)
    server = Server(test_mode=args.test)
    server.run()
