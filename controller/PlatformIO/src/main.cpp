#include <Arduino.h>
#include <SoftwareSerial.h>

// Pins Capteur Ultrason
const int trigPin = 47;
const int echoPin = 43;
long duree;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Port USB (Moniteur série)
  Serial.begin(9600);
  
  // Port Bluetooth (Pins 18 et 19)
  Serial1.begin(9600); 
  Serial.println("Bluetooth prêt");
  
}

void loop() {
  // Mesure Ultrason
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duree = pulseIn(echoPin, HIGH);
  distance = duree * 0.034 / 2;
  
  // Affichage sur le PC
  /*
  // Affichage de la distance sur le Moniteur Série
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Envoi de la distance au Bluetooth
  Serial1.print("Dist: ");
  Serial1.println(distance);
*/
  // Communication Bluetooth <-> USB
  if (Serial1.available()) {
    Serial.write(Serial1.read());
  }
  if (Serial.available()) {
    Serial1.write(Serial.read());
  }

  delay(500);
}