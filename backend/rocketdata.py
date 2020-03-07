class RocketData:
    def __init__(self, datastream: str) -> None:
        data = datastream.split(',')
        self.time = float(data[0])
        self.gps_lat = float(data[1])
        self.gps_lon = float(data[2])
        self.gps_alt = float(data[3])
        self.gps_num_sat = float(data[4])
        self.acc_x = float(data[5])
        self.acc_y = float(data[6])
        self.acc_z = float(data[7])
        self.giro_x = float(data[8])
        self.giro_y = float(data[9])
        self.giro_z = float(data[10])
        self.pressure_alt = float(data[11])
        self.pressure_temp = float(data[12])

