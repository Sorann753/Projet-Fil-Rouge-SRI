#include <Arduino.h>
#include <AFMotor.h>


const int trigPin = 47;
const int echoPin = 46;
unsigned long precedentMillis = 0;
const long intervalle = 500;
AF_DCMotor moteur(1);

void lectureUltrasons1() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
}

void moteurAvancer() {
  Serial.println("Moteur AVANT");
  moteur.run(FORWARD);
}

void moteurReculer() {
  Serial.println("Moteur ARRIERE");
  moteur.run(BACKWARD);
}

void moteurStop() {
  Serial.println("Moteur STOP");
  moteur.run(RELEASE);
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.begin(9600); 
  Serial1.begin(230400); 

  moteur.setSpeed(200);
  moteur.run(RELEASE);
  
  Serial.println("Fin Setup - Pret pour test");
}

void loop() {
  char c;

  //Bluetooth
  if (Serial.available()) {
    c = Serial.read();
    Serial1.print(c);
  }

  if (Serial1.available()) {
    c = Serial1.read();
    Serial.print(c);
  }

  //Ultrasons
  unsigned long actuelMillis = millis();
  if (actuelMillis - precedentMillis >= intervalle) {
    precedentMillis = actuelMillis;
    lectureUltrasons1();
  }
}

