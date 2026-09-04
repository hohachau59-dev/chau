#include <SoftwareSerial.h>

#define RX_PIN 10
#define TX_PIN 9

SoftwareSerial mySerial(RX_PIN, TX_PIN);

void setup() {
  Serial.begin(9600);    // Cổng Serial Monitor kết nối máy tính
  mySerial.begin(9600);  // Cổng giao tiếp với ESP32-S3
  
  Serial.println("=== DIEU KHIEN ESP32-S3 ===");
  Serial.println("0: Bat LED 12 | 1: Tat LED 12");
  Serial.println("2: Bat LED 13 | 3: Tat LED 13");
}

void loop() {
  // Nhập từ bàn phím Serial Monitor để gửi sang ESP32-S3
  if (Serial.available()) {
    char c = Serial.read();
    if (c != '\r' && c != '\n') {
      mySerial.write(c);
      Serial.print("Da gui ky tu: ");
      Serial.println(c);
    }
  }

  // Đọc phản hồi từ ESP32-S3 trả về
  if (mySerial.available()) {
    char reply = mySerial.read();
    Serial.write(reply);
  }
}
