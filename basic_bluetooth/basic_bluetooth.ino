#include "BluetoothSerial.h"

#define ENA_PIN 14
#define IN1_PIN 27
#define IN2_PIN 26
#define IN3_PIN 25
#define IN4_PIN 33
#define ENB_PIN 32

#define ratingKanan 2
#define ratingKiri 4

#define TRIG_PIN 12
#define ECHO_PIN 13
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


  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}
void loop() {
  // Periksa jarak terlepas dari input Bluetooth
  long duration;
  float distance;

  // Kirim sinyal trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");

  char currentCommand = 0;  // Variabel sementara untuk perintah saat ini

  if (SerialBT.available()) {
    currentCommand = SerialBT.read();
    Serial.print("Received: ");
    Serial.println(currentCommand);
    lastCommand = currentCommand;  // Simpan perintah terakhir yang diterima
  } else {
    currentCommand = lastCommand;  // Gunakan perintah terakhir jika tidak ada yang baru
  }

  if (distance < 10) {
    mundur();
    delay(500);
    stop();
    digitalWrite(buzzer, HIGH);
  } else {

    digitalWrite(buzzer, LOW);

    if (currentCommand == 'F') {
      maju();
      blinkingKiri = false;
      blinkingKanan = false;
    } else if (currentCommand == 'S') {
      stop();
      blinkingKiri = false;
      blinkingKanan = false;
    } else if (currentCommand == 'L') {
      belokKiri();
      blinkingKiri = false;
      blinkingKanan = false;
    } else if (currentCommand == 'R') {
      belokKanan();
      blinkingKiri = false;
      blinkingKanan = false;
    } else if (currentCommand == 'B') {
      mundur();
      blinkingKiri = false;
      blinkingKanan = false;
    }
  }

  if (currentCommand == '1') {
    blinkingKiri = true;
    blinkingKanan = false;
    blinkingHazard = false;
  } else if (currentCommand == '4') {
    blinkingKanan = true;
    blinkingKiri = false;
    blinkingHazard = false;
  } else if (currentCommand == 'X') {
    blinkingHazard = true;
    blinkingKanan = false;
    blinkingKiri = false;
  } else if (currentCommand == '2' || currentCommand == 'x') {
    blinkingHazard = false;
    blinkingKanan = false;
    blinkingKiri = false;
    digitalWrite(ratingKanan, LOW);
    digitalWrite(ratingKiri, LOW);
  } else if (currentCommand == 'Y') {
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
  } else if (currentCommand == 'Z') {
    rem();
    blinkingKiri = false;
    blinkingKanan = false;
  }

  // Jalankan fungsi blinking jika aktif
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

  
  analogWrite(ENA_PIN, 128);
  analogWrite(ENB_PIN, 128);
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

    analogWrite(ENA_PIN, 255);
  analogWrite(ENB_PIN, 255);
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
