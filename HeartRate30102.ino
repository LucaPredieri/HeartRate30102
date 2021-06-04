
///////////// Defining libraries and registers. ///////////////////////

#include <Wire.h> 
#define MAXIM 0x57
#define Mode_conf 0x09
#define Spo2_conf 0x0A
#define LED1_PA 0x0C
#define LED2_PA 0x0D
#define FIFO_DATA 0x07
#define FIFO_CONF 0x08
#define INTERRUPT_EN 0x02
#define INTERRUPT_STATUS_1 0x00

///////////////////// Declaring variables. ////////////////////////////

byte FIFO_R[3];
byte FIFO_IR[3];

unsigned long ARRAY_RED[10];
unsigned long DATA_R = 0;
unsigned long DATA_IR = 0;
unsigned long T=10^3;
unsigned long DeltaTime=0;
unsigned long SettingTime;

bool count=true;
bool stato=false;

int n=0;
int i=0;

//////////////////// Setup Function. //////////////////////////////////

void setup() {
Wire.begin();                  
Wire.setClock(100000);
Serial.begin(9600);
Write(Mode_conf, 0b00000011);
Write(Spo2_conf, 0b01101111);
Write(LED1_PA, 100);
Write(LED2_PA, 100);
Write(FIFO_CONF, 0b11110000);
Write(INTERRUPT_EN, 0b01000000);
pinMode(7, INPUT);
pinMode(6, OUTPUT);
}

///////////////////// Loop Function. ////////////////////////////////////

void loop() { 
 if (digitalRead(7)==LOW) {
  stato=true;
  SettingTime=millis();
  Serial.println("PLACE THE FOREFINGER OVER THE SENSOR.");
  delay(2000);
  Serial.println("CALCULATING...");
 }
  
 if (stato){
  byte control = Read(INTERRUPT_STATUS_1, 1);
  if (bitRead(control, 6)==1) {
    FIFO_Outuput();
  }
 if (digitalRead(7)==LOW) {
    Serial.println("PRESS THE BUTTON TO START AGAIN THE COUNT.");
    stato=false;
    delay(2000);
  }
 }
 else{
    analogWrite(6, 255);
  }
}

//////////////// Reading and Writing Registers. ///////////////////////////

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

////////////// Function to get datas and to elaborate them. ///////////////

void FIFO_Outuput(){
  
  Wire.beginTransmission(MAXIM);
  Wire.write(FIFO_DATA);
  Wire.endTransmission(false);
  Wire.requestFrom(MAXIM,6);
  
  FIFO_R[0] = (Wire.read()&0x03);
  FIFO_R[1] = (Wire.read());
  FIFO_R[2] = (Wire.read());
  FIFO_IR[0] = (Wire.read()&0x03);
  FIFO_IR[1] = (Wire.read());
  FIFO_IR[2] = (Wire.read());
  
  Wire.endTransmission(); 
  
  DATA_R = ((unsigned long)FIFO_R<<16) | ((unsigned long)FIFO_R[1]<<8) | (FIFO_R[2]);
  DATA_IR = ((unsigned long)FIFO_IR[0]<<16) | ((unsigned long)FIFO_IR[1]<<8) | (FIFO_IR[2]);
  
  ARRAY_RED[i]=DATA_R;
  if(i < 10) i++;
  else i=0;
  unsigned long SUM_R=0;
  for(int j=0; j<10; j++) {
    SUM_R += ARRAY_RED[j];
  } 
  SUM_R=SUM_R/10;
  
  float peaks=(float)DATA_R-(float)SUM_R;

  if (n==1) DeltaTime=millis();
  if(peaks>0){
    if(count && ((millis()-T)>333)){
      T=millis();
      n++;
      count=false;
    }
  }
  else {
    count=true;
  }
  
  if(n==15){
    unsigned long period=millis()-DeltaTime;
    SettingTime=millis()-SettingTime;
    float frequency=((float)(n-1)/(float)period)*1000*60;
    if (frequency<30){
      Serial.println("PROBLEMS WITH THE POSITION, TRY TO PLACE IT BETTER.");
    }
    else{
      Serial.print("BPM: ");
      Serial.println(round(frequency));
    }
    n=1;
    // Serial.println(SettingTime);
  }
  
  if (count==false){
    analogWrite(6, 254);
  }
  else{
    analogWrite(6, 255);
  } 
}

///////////////////////////////////////////////////////////////////////
