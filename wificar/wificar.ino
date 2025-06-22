#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
// Insert your network credentials
#define WIFI_SSID "esp32"
#define WIFI_PASSWORD "1012171234"

#define API_KEY "AIzaSyAJ4BNXx6_w0iTRN-teRrAH6W_B1__8cJU"
#define DATABASE_URL "https://wificar-dfeeb-default-rtdb.firebaseio.com/"

#define ENA_PIN 14
#define IN1_PIN 27
#define IN2_PIN 26
#define IN3_PIN 25
#define IN4_PIN 33
#define ENB_PIN 32



LiquidCrystal_I2C lcd(0x27, 16, 2);

FirebaseData fbdo;
FirebaseAuth auth;

FirebaseConfig config;
bool signupOK = false;



String sValue;
void setup() {
  Wire.begin(22, 23);  // SDA = GPIO 22, SCL = GPIO 23

  pinMode(ENA_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);
  pinMode(ENB_PIN, OUTPUT);


  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("haloo, loadingg..");

  delay(1000);
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println(DATABASE_URL);
    signupOK = true;
    lcd.setCursor(0, 1);
    lcd.print("Firebase: AMAN");
  } else {

    Serial.printf("%s\n", config.signer.signupError.message.c_str());
    lcd.setCursor(0, 1);
    lcd.print("Firebase: EROR");
  }
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}


void loop() {
  String data;
  if (Firebase.ready() && signupOK) {
    lcd.setCursor(0, 1);
    lcd.print("Firebase: NEWAMAN");
    if (Firebase.RTDB.getString(&fbdo, "kontrol/gerak")) {
      if (fbdo.dataType() == "string") {
        data = fbdo.stringData();
        data.trim();             // bersihin whitespace
        data.replace("\"", "");  // hapus kutip dua
        data.replace("\\", "");  // hapus backslash
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Data: " + data);
        Serial.print("Data: ");
        Serial.println(data);
        if (data == "F") {
          maju();
        } else if (data == "S") {
          stop();
          Serial.println("STOP KEBACA!");
        } else if (data == "L") {
          belokKiri();
        } else if (data == "R") {
          belokKanan();
        } else if (data == "B") {
          mundur();
        } else {
          Serial.println("Perintah tidak dikenali: " + data);
        }
      }
    } else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
      lcd.setCursor(0, 1);
      lcd.print("Firebase: NEWEROR");
    }
    Serial.println("______________________________");
  }
}
// === Fungsi gerak ===
void maju() {
  Serial.println("MAJU DIPANGGIL");
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
  analogWrite(ENA_PIN, 255);
  analogWrite(ENB_PIN, 255);
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