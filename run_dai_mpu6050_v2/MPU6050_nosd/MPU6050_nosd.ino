#include<Wire.h>
#include <SPI.h>
const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
int16_t gy_x , gy_y , gy_z , ac_x , ac_y , ac_z;
void setup() {

  
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  Serial.begin(9600);

  

}


void loop() {
  
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
