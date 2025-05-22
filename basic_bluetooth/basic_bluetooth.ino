#include "BluetoothSerial.h"

#define ENA_PIN 14
#define IN1_PIN 27
#define IN2_PIN 26
#define IN3_PIN 25
#define IN4_PIN 33
#define ENB_PIN 32

#define ratingKanan 2
#define ratingKiri 4

#define buzzer 19

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

char lastCommand = 0;
bool blinkingKanan = false;
bool blinkingKiri = false;
bool blinkingHazard = false;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test");
  Serial.println("The device started, now you can pair it with Bluetooth!");
  pinMode(buzzer, OUTPUT);
  pinMode(ratingKanan, OUTPUT);
  pinMode(ratingKiri, OUTPUT);
  pinMode(ENA_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);
  pinMode(ENB_PIN, OUTPUT);

  digitalWrite(ENA_PIN, HIGH);
  digitalWrite(ENB_PIN, HIGH);
}

void loop() {
  if (SerialBT.available()) {
    char data = SerialBT.read();
    Serial.print("Received: ");
    Serial.println(data);
    lastCommand = data;

    if (data == 'F') {
      maju();
      // Jangan matikan blinkingHazard di sini!
      blinkingKiri = false;
      blinkingKanan = false;
    } else if (data == 'S') {
      stop();
      blinkingKiri = false;
      blinkingKanan = false;
    } else if (data == 'L') {
      belokKiri();
      blinkingKiri = false;
      blinkingKanan = false;
    } else if (data == 'R') {
      belokKanan();
      blinkingKiri = false;
      blinkingKanan = false;
    } else if (data == 'B') {
      mundur();
      blinkingKiri = false;
      blinkingKanan = false;
    } else if (data == '1') {
      blinkingKiri = true;
      blinkingKanan = false;
      blinkingHazard = false;
    } else if (data == '4') {
      blinkingKanan = true;
      blinkingKiri = false;
      blinkingHazard = false;
    } else if (data == 'X') {
      blinkingHazard = true;
      blinkingKanan = false;
      blinkingKiri = false;
    } else if (data == '2' || data == 'x') {
      blinkingHazard = false;
      blinkingKanan = false;
      blinkingKiri = false;
      digitalWrite(ratingKanan, LOW);
      digitalWrite(ratingKiri, LOW);
    } else if (data == 'Y') {
      digitalWrite(buzzer, HIGH);
      delay(100);
      digitalWrite(buzzer, LOW);
    } else if (data == 'Z') {
      rem();
      blinkingKiri = false;
      blinkingKanan = false;
    }
  }

  if (blinkingHazard) {
    fungsiHazard();
  } else {
    if (blinkingKanan) fungsiRatingKanan();
    if (blinkingKiri) fungsiRatingKiri();
  }

  delay(20);
}

// === Fungsi gerak ===
void maju() {
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
}

void stop() {
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
}

void belokKiri() {
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
}

void belokKanan() {
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
}

void mundur() {
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, HIGH);
}
void rem() {
  // Rem untuk motor kiri
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, HIGH);
  // Rem untuk motor kanan
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, HIGH);
}

// === Fungsi Lampu ===
void fungsiRatingKanan() {
  static unsigned long lastToggle = 0;
  static bool ledState = false;
  if (millis() - lastToggle >= 500) {
    ledState = !ledState;
    digitalWrite(ratingKanan, ledState);
    lastToggle = millis();
  }
}

void fungsiRatingKiri() {
  static unsigned long lastToggle = 0;
  static bool ledState = false;
  if (millis() - lastToggle >= 500) {
    ledState = !ledState;
    digitalWrite(ratingKiri, ledState);
    lastToggle = millis();
  }
}

void fungsiHazard() {
  static unsigned long lastToggle = 0;
  static bool ledState = false;
  if (millis() - lastToggle >= 500) {
    ledState = !ledState;
    digitalWrite(ratingKanan, ledState);
    digitalWrite(ratingKiri, ledState);
    lastToggle = millis();
  }
}
