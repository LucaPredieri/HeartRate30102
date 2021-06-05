# Heart Rate with the MAXIM 30102 sensor.
  Measuring heart rate with the MAX30102, using the SpO2 configuration. The MAX30102 is a nice sensor made by MAXIM INTEGRATED and it is really useful and fun. In this simple project, I used it to measure the heartbeat, throught the SpO2 configuration, with 400 Sa/s, every 8 samples they are averaged so we have 50 Sa/s. The rate is processed and the output is visible on the monitor.

# Introduction.
My university gave us a special Arduino shield for Arduino Uno, which is called UniGEng ExpBoard 2. The shield covers completely every single pin on the board so it's way easier to use and we don't have to use breadboards, jumpers and other components that would slow our project study. Here's the pin scheme of the shield:

![Image of the board](https://github.com/LucaPredieri/HeartRate30102/blob/main/UniGeExpboard.JPG)

The word "ossimetro" means oximeter which is the sensor MAX30102. As every project that involves registers, we will use the library Wire of Arduino, that works with A4 and A5 pins.
Here's shown the board, the sensor is on the bottom left:

![Image of the board](https://github.com/LucaPredieri/HeartRate30102/blob/main/UnigExpboard2.0.jpeg)

The sensor is the one on the bottom left. So, I won't show you any kind of circuit. It'll be just code and some datas I got from the project. I will show you how to filter datas, store them and process them.

# Reading the registers of the MAX30102 with Wire library. 
If we read the datasheet that the company published on the web thorught this [link](https://datasheets.maximintegrated.com/en/ds/MAX30102.pdf), we can see there are some specific istructions to communicate with the sensor, using the Wire library. The Wire library helps us because the sensor uses a I2C protocol. The I2C protocol is usually read with the Wire library because with the A4 and A5 pins both the SDA (data line) and SCL (clock line) are way easier to manage.
At page 16 of the [datasheet](https://datasheets.maximintegrated.com/en/ds/MAX30102.pdf) we can see how to communicate with the sensor, I implemented two easy functions to make reading and writing faster:
```
byte Read(byte reg, byte dati){
  Wire.beginTransmission(MAXIM);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(MAXIM,dati);
  if(Wire.available()){
    return Wire.read();
  }
  else{ 
    return 0;
  }
  Wire.endTransmission();
}

void Write(byte reg, byte N) {
  Wire.beginTransmission(MAXIM);
  Wire.write(reg);
  Wire.write(N);
  Wire.endTransmission(); 
}

```
Once we have them we can configure the sensor to get the better signal we can from our body. 

# Sensor and output configuration.
If we look the datasheet we can read there are some registers that have bits dedicated to the configuration of the device and the data output (page 10 of the [datasheet](https://datasheets.maximintegrated.com/en/ds/MAX30102.pdf)). Inside the setup() function we have five lines they use the function Write() to configure the data output and the sensor. I found that the best configurations (I  bet you can do it better!) are the following:
- **0x0A SpO2 configuration**: 01101111
  - Max range of the A/D converter, max samples frequency with 18 bits of resolution (400 Sa/s), max pulse width in order to get max number of bits.
- **0x08 FIFO Configuration**: 11110000
  - Max possible average of samples, the sensor takes every 32 samples and make the average of them, then it sends it to the output. The fourth bit it's linked with the rollover of the FIFO, which means that the sensor doesn't wait for the user to read it but it just refresh it every time he can.
