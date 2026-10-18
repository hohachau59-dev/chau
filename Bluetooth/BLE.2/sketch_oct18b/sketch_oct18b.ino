
#include <SoftwareSerial.h>

// HC-05: TXD -> D2, RXD -> D3
SoftwareSerial bluetooth(2, 3);

void setup() {
  // Virtual Terminal
  Serial.begin(9600);

  // Bluetooth HC-05
  bluetooth.begin(9600);

  delay(1000);

  Serial.println("==========================");
  Serial.println("BLUETOOTH HC-05 - GUI LENH");
  Serial.println("==========================");
  Serial.println("Nhan 0: Hoat dong binh thuong");
  Serial.println("Nhan 1: Canh bao");
  Serial.println("Nhan 2: Tat tat ca LED");
  Serial.println("--------------------------");
  Serial.println("Nhap lenh:");
}

void loop() {

  // Doc lenh tu Virtual Terminal
  if (Serial.available() > 0) {

    char lenh = Serial.read();

    // Bo qua Enter
    if (lenh == '\n' || lenh == '\r') {
      return;
    }

    // Kiem tra lenh hop le
    if (lenh == '0' ||
        lenh == '1' ||
        lenh == '2') {

      // Gui lenh qua Bluetooth
      bluetooth.write(lenh);

      Serial.print("Da gui lenh: ");
      Serial.println(lenh);

    } else {
      Serial.println("Lenh khong hop le!");
    }
  }

  // Nhan phan hoi tu Arduino 2
  while (bluetooth.available() > 0) {
    char data = bluetooth.read();
    Serial.write(data);
  }
}

