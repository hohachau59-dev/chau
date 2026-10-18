
#include <SoftwareSerial.h>

// Khai bao chan ket noi Bluetooth HC-05
#define BT_RX 10
#define BT_TX 11

// Khoi tao giao tiep Bluetooth
SoftwareSerial bluetooth(BT_RX, BT_TX);

void setup() {
  // Khoi tao Virtual Terminal
  Serial.begin(9600);

  // Khoi tao Bluetooth HC-05
  bluetooth.begin(9600);

  delay(1000);

  Serial.println("==========================");
  Serial.println("Bluetooth HC-05 Ready!");
  Serial.println("Waiting for data...");
  Serial.println("==========================");
}

void loop() {

  // Nhan du lieu tu Bluetooth HC-05
  if (bluetooth.available() > 0) {

    char data = bluetooth.read();

    // Hien thi du lieu nhan duoc
    // len Virtual Terminal
    Serial.write(data);
  }

  // Nhan du lieu tu Virtual Terminal
  if (Serial.available() > 0) {

    char data = Serial.read();

    // Hien thi lai ky tu vua nhap
    Serial.write(data);

    // Gui du lieu sang Bluetooth HC-05
    bluetooth.write(data);
  }
}
