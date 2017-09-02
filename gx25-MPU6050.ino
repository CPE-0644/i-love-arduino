 
    www.hobbydistrict.com
    Lesson 6
    Tilt Angle Module - GY-25 board -
    The following code reads data
    outputted by the tilt angle module
    over software serial and prints it
    to the terminal. 
    
    Connections
    VCC - 5V
    RX - pin3
    TX - pin2
    GND - GND
    
    Written by Fawzi Mudawwar 
    May 30, 2016
 


#include SoftwareSerial.h

const byte rxPin = 2;
const byte txPin = 3;
SoftwareSerial angleModulePort (rxPin, txPin);

unsigned char returnBuffer[8],counter=0; buffer where the received data will be stored
unsigned char fullBuffer=0;              0 buffer not full, 1 buffer is full
int yawPitchRoll[3];                     An array of three integers. Index 0 Yaw, Index 1 Pitch, Index 2 roll.

 the setup function runs once when you press reset
void setup()
{
  angleModulePort.begin(115200);         initialize serial communication between the Arduino and tilt angle module
  delay(2000);    
  angleModulePort.write(0XA5);           request the data
  angleModulePort.write(0X52); 
  Serial.begin(115200);                  initialize serial communication between the Arduino and the PC
}

 the loop function runs over and over again forever
void loop()
{
     
     if(fullBuffer == 1 && returnBuffer[0]==0xAA && returnBuffer[7]==0x55)   Check if buffer is full and if packet is correct
     {      
            Convert raw data to angle     
            yawPitchRoll[0]=(returnBuffer[1]8returnBuffer[2])100;
            yawPitchRoll[1]=(returnBuffer[3]8returnBuffer[4])100;
            yawPitchRoll[2]=(returnBuffer[5]8returnBuffer[6])100;

            Print angles
            Serial.print(yawPitchRoll[0]);
            Serial.print(, );
            Serial.print(yawPitchRoll[1]);
            Serial.print(, );
            Serial.println(yawPitchRoll[2]);
     }
     
     fullBuffer=0;
     angleModulePortEvent();          Checks if data has been received
}

void angleModulePortEvent()
{
  while (angleModulePort.available())
  {   
    returnBuffer[counter]=(unsigned char)angleModulePort.read();
    if(counter==0&&returnBuffer[0]!=0xAA) return;            wait for start character
    counter++;       
    if(counter==8)                
    {    
       counter=0;                                           
       fullBuffer=1;                                        mark buffer as full
    }      
  }
}