#include <Servo.h>
#include <SoftwareSerial.h>

Servo myservo;  // Crea un objeto servo
int trigPin2 = 2;
int echoPin2 = 3;
int trigPin1 = 7;
int echoPin1 = 8;
int servoState = 0;  // 0 para 0 grados, 1 para 90 grados
int ledState = LOW;  // Estado inicial del LED

SoftwareSerial bluetooth(10, 11);  // RX, TX

void setup() {
  myservo.attach(9);  // Adjunta el servo al pin 9
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  Serial.begin(9600);
  bluetooth.begin(9600);
}

void loop() {
  long duration1, distance1;
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = (duration1 / 2) / 29.1;  // Calcula la distancia en centímetros

  if (distance1 < 30) {  // Si la distancia es menor de 30 cm, gira el servo a 90 grados
    if (servoState == 0) {
      myservo.write(120);
      servoState = 1;
      Serial.println("Objeto a menos de 30 cm de distancia. Servo a 90 grados.");
    }
  } else {  // Si no hay objeto, gira el servo a 0 grados
    if (servoState == 1) {
      myservo.write(0);
      servoState = 0;
      Serial.println("No se detecta objeto. Servo a 0 grados.");
    }
  }

  long duration2, distance2;
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 * 0.0343 / 2;

  Serial.print("Distancia: ");
  Serial.print(distance2);
  Serial.println(" cm");

  // Si el sensor ultrasónico 2 detecta 10 cm, detiene el servo
  if (distance2 <= 10) {
    if (servoState == 1) {
      myservo.write(0);
      servoState = 0;
      Serial.println("Objeto a 10 cm o menos de distancia. Servo a 0 grados.");
    }
  }

  // Lee el estado del LED desde el Bluetooth
  if (bluetooth.available()) {
    char estado = bluetooth.read();
    if (estado == '1') {
      ledState = HIGH;
    } else if (estado == '0') {
      ledState = LOW;
    }
  }

  // Escribe el estado del LED en el Bluetooth
  bluetooth.print(ledState);

  delay(500);
}
