#include <AFMotor.h> // La fameuse bibliothèque

// On déclare notre moteur sur le port M1
AF_DCMotor moteur1(1); 

void setup() {
  Serial.begin(9600);           // Ouvre la communication avec le PC
  Serial.println("Test Moteur 1 : START");
  
  moteur1.setSpeed(200);        // Règle la puissance (de 0 à 255)
  moteur1.run(RELEASE);         // S'assure que le moteur est coupé au démarrage
}

void loop() {
  Serial.println("Moteur AVANT");
  moteur1.run(FORWARD);         // Envoie le courant dans un sens
  delay(2000);                  // Laisse tourner 2 secondes

  Serial.println("Moteur STOP");
  moteur1.run(RELEASE);         // Coupe le courant
  delay(1000);

  Serial.println("Moteur ARRIERE");
  moteur1.run(BACKWARD);        // Inverse les pôles
  delay(2000);

  Serial.println("Moteur STOP");
  moteur1.run(RELEASE);
  delay(2000);
}