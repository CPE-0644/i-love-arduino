#include<Wire.h>
#include <SPI.h>
#include "WiFiEsp.h"


#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

char ssid[] = "SaraT";            // your network SSID (name)
char pass[] = "keptyaw8inghuh";        // your network password
int status = WL_IDLE_STATUS;      // the Wifi radio's status

char server[] = "172.20.10.5";

// Initialize the Ethernet client object
WiFiEspClient client;

const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
int16_t gy_x , gy_y , gy_z , ac_x , ac_y , ac_z;

void setup()
{
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  Serial.begin(115200);
  Serial1.begin(9600);

  WiFi.init(&Serial1);
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }

  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");

  printWifiStatus();

  Serial.println();
  Serial.println("Starting connection to server...");
  do {
    if (client.connect(server, 9999)) {
      Serial.println("Connected to server");
      //client.println("Arduino is Connecting.. Ready to share data!");
    } else {
      Serial.println("Fail to connected");
    }
  } while (!client.connected());
  pinMode(LED_BUILTIN, OUTPUT);
}

char n1[10], n2[10], n3[10], n4[10], n5[10], n6[10];

void loop()
{
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
  char data[100];
  itoa(ac_x, n1, 10);
  itoa(ac_y, n2, 10);
  itoa(ac_z, n3, 10);
  itoa(gy_x, n4, 10);
  itoa(gy_y, n5, 10);
  itoa(gy_z, n6, 10);
  strcpy(data, n1);
  strcat(data, ",");
  strcat(data, n2);
  strcat(data, ",");
  strcat(data, n3);
  strcat(data, ",");
  strcat(data, n4);
  strcat(data, ",");
  strcat(data, n5);
  strcat(data, ",");
  strcat(data, n6);
  
  // Send data to server
  if (client.connected()) {
    Serial.println(data);
    client.print(data);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else if (!client.connected())
  {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println();
    Serial.println("Disconnecting from server...");
    client.stop();
    // do nothing forevermore
    while (true);
  }

  char recv[100] = {};
  
  // Receive data from server
  while (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  //Serial.println(recv);

  delay(500);
}


void printWifiStatus()
{
  // print the SSID of the network you're 0attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
