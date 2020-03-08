//********** LIBRARIES **********//
#include <TinyGPS.h>
#include <SPI.h>
#include <SD.h>
#include <MPU9250.h>
#include <SoftwareSerial.h> 


//********** DEFINES **********//
//MPL3115A2 Altimeter
#define MPL3115A2_ADDRESS 0x60 // 7-bit I2C address
#define STATUS     0x00
#define OUT_P_MSB  0x01
#define OUT_P_CSB  0x02
#define OUT_P_LSB  0x03
#define OUT_T_MSB  0x04
#define OUT_T_LSB  0x05
#define DR_STATUS  0x06
#define OUT_P_DELTA_MSB  0x07
#define OUT_P_DELTA_CSB  0x08
#define OUT_P_DELTA_LSB  0x09
#define OUT_T_DELTA_MSB  0x0A
#define OUT_T_DELTA_LSB  0x0B
#define WHO_AM_I   0x0C
#define F_STATUS   0x0D
#define F_DATA     0x0E
#define F_SETUP    0x0F
#define TIME_DLY   0x10
#define SYSMOD     0x11
#define INT_SOURCE 0x12
#define PT_DATA_CFG 0x13
#define BAR_IN_MSB 0x14
#define BAR_IN_LSB 0x15
#define P_TGT_MSB  0x16
#define P_TGT_LSB  0x17
#define T_TGT      0x18
#define P_WND_MSB  0x19
#define P_WND_LSB  0x1A
#define T_WND      0x1B
#define CTRL_REG1  0x26

//SD Card
#define SDPIN 4 //ChipSelect pin SD card

//MPU-9250 Accelerometer-Gyro
#define AHRS false //Set to false for basic data read
#define SerialDebug true  // Set to true to get Serial output for debugging
#define I2Cclock 400000
#define I2Cport Wire
#define MPU9250_ADDRESS MPU9250_ADDRESS_AD0
#define GYRO_FULL_SCALE_2000_DPS 0x18 //Gyro scale
#define ACC_FULL_SCALE_16_G 0x18 //Accelerometer scale


//********** VARIABLES **********//
//File dataLog;
SoftwareSerial gpsSerial(2,3); //gps tx, rx
TinyGPS gps; //create gps object
MPU9250 myIMU(MPU9250_ADDRESS, I2Cport, I2Cclock);

int counter_ = 0;
int cycle = 0;
String data; //data for datalog file
bool lock = false;

float lat = 0.0; //create latitude variable (in degrees) (GY-GPS6MV2)
float lon = 0.0; //create longtitude variable (in degrees) (GY-GPS6MV2)
float alt_gps= 0.0; //create altitude variable (in meter) (GY-GPS6MV2)
float num_sats = 0.0; //create number of satellites variable (GY-GPS6MV2)
float acc_x = 0.0; //create accelerometer x variable (MPU-9250)
float acc_y = 0.0; //create accelerometer y variable (MPU-9250)
float acc_z = 0.0; //create accelerometer z variable (MPU-9250)
float gyro_x = 0.0; //create gyro x variable (MPU-9250)
float gyro_y = 0.0; //create gyro y variable (MPU-9250)
float gyro_z = 0.0; //create gyro z variable (MPU-9250)
float pressure_mpl = 0.0; //create pressure variable (in Pascal) (MPL3115A2)
float temperature_mpl = 0.0; //create temperature variable (in Celsius) (MPL3115A2)
//float sea_pressure_mpl = 0.0; //create altitude at sea level variable (in meter) (MPL3115A2)


//********** SETUP FUNCTION **********//
void setup()
{ 
  Wire.begin();
  Serial.begin(9600); //connect terminal serial
  //Serial1.begin(9600); //connect LORA serial, only used on Arduino Nano Every
  gpsSerial.begin(9600); // connect gps sensor

  /*
  byte c = myIMU.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
  Serial.println(c);
  
  if (c == 0x73) // WHO_AM_I should always be 0x71
  {
    Serial.println("MPU9250 is online...");
  }
  else 
    Serial.println("MPU9250 is offline...");
  */

  // Calibrate gyro and accelerometers, load biases in bias registers
  myIMU.calibrateMPU9250(myIMU.gyroBias, myIMU.accelBias);

  // Initialize device for active mode read of acclerometer, gyroscope, and temperature
  myIMU.initMPU9250();
  
  // Get magnetometer calibration from AK8963 ROM
  // Initialize device for active mode read of magnetometer
  myIMU.initAK8963(myIMU.factoryMagCalibration);
  
  // Configure Gyro Sensitivity
  Wire.beginTransmission(MPU9250_ADDRESS);
  Wire.write(0x1B);                   // Talk to the GYRO_CONFIG register (1B hex)
  Wire.write(GYRO_FULL_SCALE_2000_DPS);                   // Set the register bits as 00011000 (2000deg/s full scale)
  Wire.endTransmission(true);

  // Configure Accelerometer Sensitivity
  Wire.beginTransmission(MPU9250_ADDRESS);
  Wire.write(0x1C);                  //Talk to the ACCEL_CONFIG register (1C hex)
  Wire.write(ACC_FULL_SCALE_16_G);                  //Set the register bits as 00011000 (+/- 16g full scale range)
  Wire.endTransmission(true);

  // Get sensor resolutions, only need to do this once
  myIMU.getAres();
  myIMU.getGres();
  myIMU.getMres();

  // The next call delays for 4 seconds, and then records about 15 seconds of
  // data to calculate bias and scale.
  myIMU.magCalMPU9250(myIMU.magBias, myIMU.magScale);
  
  // Configure the MPL sensor
  setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa
  setOversampleRate(1); // Set Oversample to the recommended 128
  enableEventFlags(); // Enable all three pressure and temp event flags
  
  /*
  if (SD.begin(SDPIN)) 
  { 
    Serial.println("SD Initialization successful!");
    if(SD.exists("datalog.txt"))
    {
      SD.remove("datalog.txt");
    }
  }
  else 
    Serial.println("SD Initialization failed!");
  */
} 


//********** MAIN LOOP **********//
void loop()
{ 

  Serial.write('|');
  Serial.print(millis());
  Serial.write(",");
  Serial.print(lat*100000);
  Serial.write(",");
  Serial.print(lon*100000);
  Serial.write(","); 
  Serial.print(alt_gps, 6);
  Serial.write(",");
  Serial.print(num_sats);
  Serial.write(",");
  Serial.print(acc_x);
  Serial.write(",");
  Serial.print(acc_y);
  Serial.write(",");
  Serial.print(acc_z);
  Serial.write(",");
  Serial.print(gyro_x);
  Serial.write(",");
  Serial.print(gyro_y);
  Serial.write(",");
  Serial.print(gyro_z);
  Serial.write(",");
  Serial.print(pressure_mpl);
  Serial.write(",");
  Serial.println(temperature_mpl);
}


/*
void printData()
{
  dataLog = SD.open("datalog.txt", FILE_WRITE);
  data = (String)counter_ + "\t" + (String)pressure_mpl + "\t" + (String)temperature_mpl+ "\t" + (String)alt_mpl + "\t" + (String)alt_gps + "\t" + (String)(lat*10000) + "\t" + (String)(lon*10000) + "\t" + (String)gps.satellites(); 
  dataLog.println(data);
  dataLog.close();
}
*/


static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (gpsSerial.available())
      gps.encode(gpsSerial.read());
  } while (millis() - start < ms);
}


//Reads the current pressure in Pa
//Unit must be set in barometric pressure mode
float readPressure()
{ 
  toggleOneShot(); //Toggle the OST bit causing the sensor to immediately take another reading

  //Wait for PDR bit, indicates we have new pressure data
  int counter = 0;
  while( (IIC_Read(STATUS) & (1<<2)) == 0)
  {
      if(++counter > 100) return(-999); //Error out
      delay(1);
  }
  
  // Read pressure registers
  Wire.beginTransmission(MPL3115A2_ADDRESS);
  Wire.write(OUT_P_MSB);  // Address of data to get
  Wire.endTransmission(false); // Send data to I2C dev with option for a repeated start. THIS IS NECESSARY and not supported before Arduino V1.0.1!
  Wire.requestFrom(MPL3115A2_ADDRESS, 3); // Request three bytes

  //Wait for data to become available
  counter = 0;
  while(Wire.available() < 3)
  {
    if(counter++ > 100) return(-999); //Error out
    delay(1);
  }

  byte msb, csb, lsb;
  msb = Wire.read();
  csb = Wire.read();
  lsb = Wire.read();

  toggleOneShot(); //Toggle the OST bit causing the sensor to immediately take another reading

  // Pressure comes back as a left shifted 20 bit number
  long pressure_whole = (long)msb<<16 | (long)csb<<8 | (long)lsb;
  pressure_whole >>= 6; //Pressure is an 18 bit number with 2 bits of decimal. Get rid of decimal portion.

  lsb &= 0b00110000; //Bits 5/4 represent the fractional component
  lsb >>= 4; //Get it right aligned
  float pressure_decimal = (float)lsb/4.0; //Turn it into fraction

  float pressure = (float)pressure_whole + pressure_decimal;

  return(pressure);
}


float readTemp()
{
  toggleOneShot(); //Toggle the OST bit causing the sensor to immediately take another reading

  //Wait for TDR bit, indicates we have new temp data
  int counter = 0;
  while( (IIC_Read(STATUS) & (1<<1)) == 0)
  {
      if(++counter > 100) return(-999); //Error out
      delay(1);
  }
  
  // Read temperature registers
  Wire.beginTransmission(MPL3115A2_ADDRESS);
  Wire.write(OUT_T_MSB);  // Address of data to get
  Wire.endTransmission(false); // Send data to I2C dev with option for a repeated start. THIS IS NECESSARY and not supported before Arduino V1.0.1!
  Wire.requestFrom(MPL3115A2_ADDRESS, 2); // Request two bytes

  //Wait for data to become available
  counter = 0;
  while(Wire.available() < 2)
  {
    if(++counter > 100) return(-999); //Error out
    delay(1);
  }

  byte msb, lsb;
  msb = Wire.read();
  lsb = Wire.read();
  
  // The least significant bytes l_altitude and l_temp are 4-bit,
  // fractional values, so you must cast the calulation in (float),
  // shift the value over 4 spots to the right and divide by 16 (since 
  // there are 16 values in 4-bits). 
  float templsb = (lsb>>4)/16.0; //temp, fraction of a degree

  float temperature = (float)(msb + templsb);

  return(temperature);
}

//Sets the mode to Barometer
//CTRL_REG1, ALT bit
void setModeBarometer()
{
  byte tempSetting = IIC_Read(CTRL_REG1); //Read current settings
  tempSetting &= ~(1<<7); //Clear ALT bit
  IIC_Write(CTRL_REG1, tempSetting);
}


//Call with a rate from 0 to 7. See page 33 for table of ratios.
//Sets the over sample rate. Datasheet calls for 128 but you can set it 
//from 1 to 128 samples. The higher the oversample rate the greater
//the time between data samples.
void setOversampleRate(byte sampleRate)
{
  if(sampleRate > 7) sampleRate = 7; //OS cannot be larger than 0b.0111
  sampleRate <<= 3; //Align it for the CTRL_REG1 register

  byte tempSetting = IIC_Read(CTRL_REG1); //Read current settings
  tempSetting &= 0b11000111; //Clear out old OS bits
  tempSetting |= sampleRate; //Mask in new OS bits
  IIC_Write(CTRL_REG1, tempSetting);
}


//Clears then sets the OST bit which causes the sensor to immediately take another reading
//Needed to sample faster than 1Hz
void toggleOneShot(void)
{
  byte tempSetting = IIC_Read(CTRL_REG1); //Read current settings
  tempSetting &= ~(1<<1); //Clear OST bit
  IIC_Write(CTRL_REG1, tempSetting);

  tempSetting = IIC_Read(CTRL_REG1); //Read current settings to be safe
  tempSetting |= (1<<1); //Set OST bit
  IIC_Write(CTRL_REG1, tempSetting);
}


//Enables the pressure and temp measurement event flags so that we can
//test against them. This is recommended in datasheet during setup.
void enableEventFlags()
{
  IIC_Write(PT_DATA_CFG, 0x07); // Enable all three pressure and temp event flags 
}


// These are the two I2C functions in this sketch.
byte IIC_Read(byte regAddr)
{
  // This function reads one byte over IIC
  Wire.beginTransmission(MPL3115A2_ADDRESS);
  Wire.write(regAddr);  // Address of CTRL_REG1
  Wire.endTransmission(false); // Send data to I2C dev with option for a repeated start. THIS IS NECESSARY and not supported before Arduino V1.0.1!
  Wire.requestFrom(MPL3115A2_ADDRESS, 1); // Request the data...
  return Wire.read();
}


void IIC_Write(byte regAddr, byte value)
{
  // This function writes one byto over IIC
  Wire.beginTransmission(MPL3115A2_ADDRESS);
  Wire.write(regAddr);
  Wire.write(value);
  Wire.endTransmission(true);
}
