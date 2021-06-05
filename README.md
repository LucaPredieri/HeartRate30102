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
If we read the datasheet that the company published on the web thorught this [link](https://datasheets.maximintegrated.com/en/ds/MAX30102.pdf)
> 
