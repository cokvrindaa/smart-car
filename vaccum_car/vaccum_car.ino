#define ENA_PIN 14
#define IN1_PIN 27
#define IN2_PIN 26
#define IN3_PIN 25
#define IN4_PIN 33
#define ENB_PIN 32

#define relayVacum 13

// Ultrasonik 1
#define TRIG_PIN1 16
#define ECHO_PIN1 17

// Ultrasonik 2
#define TRIG_PIN2 5
#define ECHO_PIN2 18

// Ultrasonik 3
#define TRIG_PIN3 19
#define ECHO_PIN3 21

int speed = 100;

void setup() {
  Serial.begin(115200);

  pinMode(ENA_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);
  pinMode(ENB_PIN, OUTPUT);
  pinMode(relayVacum, OUTPUT);

  pinMode(TRIG_PIN1, OUTPUT);
  pinMode(ECHO_PIN1, INPUT);

  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);

  pinMode(TRIG_PIN3, OUTPUT);
  pinMode(ECHO_PIN3, INPUT);
}

void loop() {
  float distance1 = bacaUltrasonik(TRIG_PIN1, ECHO_PIN1);
  float distance2 = bacaUltrasonik(TRIG_PIN2, ECHO_PIN2);
  float distance3 = bacaUltrasonik(TRIG_PIN3, ECHO_PIN3);

  Serial.print("Depan: ");
  Serial.print(distance1);
  Serial.print(" cm | Kanan: ");
  Serial.print(distance2);
  Serial.print(" cm | Kiri: ");
  Serial.println(distance3);

  if (distance1 < 15) {
    stop();
    mundur();
    delay(600); // Tambahkan delay setelah mundur agar cukup menjauh

    // Perbarui pembacaan sensor setelah mundur
    distance2 = bacaUltrasonik(TRIG_PIN2, ECHO_PIN2);
    distance3 = bacaUltrasonik(TRIG_PIN3, ECHO_PIN3);

    // jika kiri sempit dan kanan longgar, belok kanan
    if (distance2 <= 10 && distance3 > 10) {
      belokKanan();
      delay(600);
      stop();
    }
    // jika kanan sempit dan kiri longgar, belok kiri
    else if (distance3 <= 10 && distance2 > 10) {
      belokKiri();
      delay(600);
      stop();
    }
    // Jika sama-sama longgar, bandingkan mana yang lebih longgar
    else if (distance2 > 10 && distance3 > 10) {
      if (distance2 > distance3) {
        belokKiri();
      } else {
        belokKanan();
      }
      delay(500);
      stop();
    }

  } else if (distance2 < 4) {
    belokKanan();
    delay(300);
    stop();
  } else if (distance3 < 4) {
    belokKiri();
    delay(300);
    stop();
  } else {
    maju();
  }

  delay(200);
}
// Fungsi baca jarak ultrasonik
float bacaUltrasonik(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  float distance = duration * 0.034 / 2;

  if (distance == 0 || distance > 400) return 999;
  return distance;
}

// Fungsi gerak
void maju() {
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
  analogWrite(ENA_PIN, speed);
  analogWrite(ENB_PIN, speed);
}

void stop() {
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
  analogWrite(ENA_PIN, speed);
  analogWrite(ENB_PIN, speed);
}

void belokKiri() {
  digitalWrite(IN1_PIN, HIGH);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
  analogWrite(ENA_PIN, speed);
  analogWrite(ENB_PIN, speed);
}

void belokKanan() {
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN4_PIN, LOW);
  analogWrite(ENA_PIN, speed);
  analogWrite(ENB_PIN, speed);
}

void mundur() {
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, HIGH);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, HIGH);
  analogWrite(ENA_PIN, speed);
  analogWrite(ENB_PIN, speed);
}
