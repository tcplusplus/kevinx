from typing import Optional
import argparse
from inputs import Inputs
from input_test import InputTest
from input_serial import InputSerial
from rocketdata import RocketData

def parse_input() -> argparse.Namespace:
    """
    parse argments
    """
    parser = argparse.ArgumentParser(description='KevinX backend ')
    parser.add_argument('--test', '-t', default=False, help='Using test data', action='store_true')
    return parser.parse_args()

if __name__ == '__main__':
    args = parse_input()
    source: Optional[Inputs] = None
    if args.test:
        source = InputTest()
    else:
        source = InputSerial()
    if source is None:
        exit(0)
    
    for data in source.get_data():
        rocket_data = RocketData(datastream=data)
        print(rocket_data)


'''
from serial import Serial
stream = Serial(port='/dev/ttyS0')
data = ''
while True:
    c = stream.read(1)
    if c != '|':
        data += c
    else:
        print(c)
'''
'''
with open('/dev/ttyS0') as fptr:
    while True:
        data = fptr.read()
        print(data)
'''
