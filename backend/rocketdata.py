from typing import Dict
from exportable import Exportable

class RocketData(Exportable):
    def __init__(self) -> None:
        super().__init__()
        self.time = 0
        self.gps_lat = 0
        self.gps_lon = 0 
        self.gps_alt = 0
        self.gps_num_sat = 0
        self.acc_x = 0
        self.acc_y = 0
        self.acc_z = 0
        self.giro_x = 0
        self.giro_y = 0
        self.giro_z = 0
        self.pressure_alt = 0
        self.pressure_temp = 0
        self.recorders: Dict[str, bool] = {}

    def update(self, datastream: str) -> bool:
        data = datastream.split(',')
        index = int(data[0])
        value = float(data[1])
        if index == 0:
            self.time = value
        if index == 1:
            self.gps_lat = value
        if index == 2:
            self.gps_lon = value
        if index == 3:
            self.gps_alt = value
        if index == 4:
            self.gps_num_sat = value
        if index == 5:
            self.acc_x = value
        if index == 6:
            self.acc_y = value
        if index == 7:
            self.acc_z = value
        if index == 8:
            self.giro_x = value
        if index == 9:
            self.giro_y = value
        if index == 10:
            self.giro_z = value
        if index == 11:
            self.pressure_alt = value
        if index == 12:
            self.pressure_temp = value
        

