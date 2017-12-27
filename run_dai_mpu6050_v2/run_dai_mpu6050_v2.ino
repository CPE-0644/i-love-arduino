#include<Wire.h>
#include <SPI.h>
#include <SD.h>
const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
int16_t gy_x , gy_y , gy_z , ac_x , ac_y , ac_z;
File myFile;
void setup() {
    Wire.begin();
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x6B);  // PWR_MGMT_1 register
    Wire.write(0);     // set to zero (wakes up the MPU-6050)
    Wire.endTransmission(true);
    Serial.begin(9600);
    Serial.print("Initializing SD card...");
    if (!SD.begin(4)) {
      Serial.println("initialization failed!");
      return;
    }
    Serial.println("initialization done.");

       myFile = SD.open("AAA.TXT", FILE_WRITE);

    while(1) {
        Wire.beginTransmission(MPU_addr);
        Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
        Wire.endTransmission(false);
        Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
        AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
        AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
        AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
        Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
        GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
        GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
        GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
        gy_x = map(GyX, -17000, 17000, 0, 179);
        gy_y = map(GyY, -17000, 17000, 0, 179);
        gy_z = map(GyZ, -17000, 17000, 0, 179);
        ac_x = map(AcX, -17000, 17000, 0, 179);
        ac_y = map(AcY, -17000, 17000, 0, 179);
        ac_z = map(AcZ, -17000, 17000, 0, 179);

     
        // if the file opened okay, write to it:
        if (myFile) {
          Serial.print("Writing to data.txt...");
          myFile.print(ac_x);
          myFile.print(" ");
          myFile.print(ac_y);
          myFile.print(" ");
          myFile.print(ac_z);
          myFile.print(" ");
          myFile.print(gy_x);
          myFile.print(" ");
          myFile.print(gy_y);
          myFile.print(" ");
          myFile.println(gy_z);
        // close the file:
          Serial.println("done.");
        } else {
            // if the file didn't open, print an error:
              Serial.println("error opening data.txt");
            }
        Serial.print(ac_x);
        Serial.print(" ");
        Serial.print(ac_y);
        Serial.print(" ");
        Serial.print(ac_z);
        Serial.print(" ");
        Serial.print(gy_x);
        Serial.print(" ");
        Serial.print(gy_y);
        Serial.print(" ");
        Serial.println(gy_z);
        delay(333);
        }
        myFile.close();
    }
void loop() {

}
