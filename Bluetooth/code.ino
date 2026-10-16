
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLEAdvertising.h>
#include <Adafruit_NeoPixel.h>

// ======================================
// 1. CAU HINH LED RGB
// ======================================

#define LED_PIN 48
#define NUM_LEDS 1

Adafruit_NeoPixel led(
  NUM_LEDS,
  LED_PIN,
  NEO_GRB + NEO_KHZ800
);

// ======================================
// 2. CAU HINH BLUETOOTH
// ======================================

// Ten Bluetooth hien thi tren dien thoai
#define BLE_NAME "HC"

// UUID giao tiep BLE
#define SERVICE_UUID \
  "12345678-1234-1234-1234-1234567890ab"

#define CHAR_UUID \
  "12345678-1234-1234-1234-1234567890ac"

// Lenh dieu khien:
// 0: Tat den
// 1: Mau trang
// 2: Mau do
// 3: Mau xanh la
// 4: Mau xanh duong
// 5: Mau vang
// 6: Mau tim
// 7: Rainbow

volatile char lenh = '0';
volatile bool restartAdvertising = false;

char lenhCu = 'X';

unsigned long lastUpdate = 0;
uint16_t hue = 0;

// ======================================
// 3. HAM DOI MAU LED
// ======================================

void doiMau(int r, int g, int b) {
  led.setPixelColor(
    0,
    led.Color(r, g, b)
  );
  led.show();
}

// ======================================
// 4. NHAN LENH TU DIEN THOAI
// ======================================

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(
    BLECharacteristic *pCharacteristic
  ) override {

    auto value = pCharacteristic->getValue();

    if (value.length() == 0) {
      return;
    }

    char cmd = value[0];

    Serial.print("Nhan lenh: ");
    Serial.println(cmd);

    if (cmd >= '0' && cmd <= '7') {
      lenh = cmd;
    }
    else {
      Serial.println("Lenh khong hop le!");
    }
  }
};

// ======================================
// 5. XU LY KET NOI BLUETOOTH
// ======================================

class ServerCallbacks : public BLEServerCallbacks {

  void onConnect(
    BLEServer *pServer
  ) override {
    Serial.println("Dien thoai da ket noi!");
  }

  void onDisconnect(
    BLEServer *pServer
  ) override {
    Serial.println("Dien thoai da ngat ket noi!");
    restartAdvertising = true;
  }
};

// ======================================
// 6. KHOI TAO ESP32-S3
// ======================================

void setup() {

  Serial.begin(115200);
  delay(1000);

  // Khoi tao LED RGB
  led.begin();
  led.setBrightness(80);
  led.clear();
  led.show();

  Serial.println("====================");
  Serial.println("KHOI DONG ESP32-S3");
  Serial.println("====================");

  // Khoi tao Bluetooth voi ten HC
  BLEDevice::init(BLE_NAME);

  // Tao BLE Server
  BLEServer *server =
    BLEDevice::createServer();

  server->setCallbacks(
    new ServerCallbacks()
  );

  // Tao Service
  BLEService *service =
    server->createService(SERVICE_UUID);

  // Tao Characteristic
  BLECharacteristic *characteristic =
    service->createCharacteristic(
      CHAR_UUID,
      BLECharacteristic::PROPERTY_WRITE |
      BLECharacteristic::PROPERTY_WRITE_NR
    );

  characteristic->setCallbacks(
    new MyCallbacks()
  );

  // Bat dau Service
  service->start();

  // Cau hinh phat Bluetooth
  BLEAdvertising *advertising =
    BLEDevice::getAdvertising();

  BLEAdvertisementData advData;

  advData.setFlags(0x06);
  advData.setName(BLE_NAME);

  advertising->setAdvertisementData(advData);
  advertising->setScanResponse(true);

  // Bat dau phat Bluetooth
  advertising->start();

  Serial.println("====================");
  Serial.println("BLUETOOTH DA PHAT SONG");
  Serial.print("TEN: ");
  Serial.println(BLE_NAME);
  Serial.println("MO NRF CONNECT DE QUET");
  Serial.println("====================");
}

// ======================================
// 7. XU LY DIEU KHIEN LED
// ======================================

void loop() {

  // Phat lai Bluetooth khi mat ket noi
  if (restartAdvertising) {

    restartAdvertising = false;

    delay(200);

    BLEDevice::startAdvertising();

    Serial.println("Dang cho ket noi lai...");
  }

  char currentLenh = lenh;

  if (currentLenh != lenhCu) {

    lenhCu = currentLenh;

    switch (currentLenh) {

      case '0':
        doiMau(0, 0, 0);
        Serial.println("LED OFF");
        break;

      case '1':
        doiMau(255, 255, 255);
        Serial.println("LED WHITE");
        break;

      case '2':
        doiMau(255, 0, 0);
        Serial.println("LED RED");
        break;

      case '3':
        doiMau(0, 255, 0);
        Serial.println("LED GREEN");
        break;

      case '4':
        doiMau(0, 0, 255);
        Serial.println("LED BLUE");
        break;

      case '5':
        doiMau(255, 255, 0);
        Serial.println("LED YELLOW");
        break;

      case '6':
        doiMau(128, 0, 255);
        Serial.println("LED PURPLE");
        break;

      case '7':
        hue = 0;
        lastUpdate = 0;
        Serial.println("RAINBOW MODE");
        break;
    }
  }

  // ======================================
  // 8. HIEU UNG RAINBOW
  // ======================================

  if (currentLenh == '7') {

    if (millis() - lastUpdate >= 30) {

      lastUpdate = millis();

      led.setPixelColor(
        0,
        led.gamma32(
          led.ColorHSV(hue)
        )
      );

      led.show();

      hue += 256;
    }
  }

  delay(10);
}
