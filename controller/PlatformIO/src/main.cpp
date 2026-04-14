#include <Arduino.h>
#include "constants.hpp"
#include "motor.hpp"
#include "ultrasons.hpp"

unsigned long tempsFinAction = 0;
bool robotMouvement = false;
unsigned long dernierMessagePi = 0;

void setup() {
  Serial.begin(115200);  // Communication avec le PC/Raspberry Pi
  Serial1.begin(230400); // Bluetooth

  initUltrasons();
  
  setspeedroue(robotspeed);
  moteurStop();

  dernierMessagePi = millis();
  Serial.println("Fin Setup - Pret pour test");
}

void loop() {
  unsigned long actuelMillis = millis();

  // Mise à jour périodique des distances (gérée par millis() dans le module)
  updateUltrasons();

  // SECURITE : Arrêt prioritaire si obstacle devant
  if (distAv <= 40 && robotMouvement) {
    moteurStop();
    robotMouvement = false;
    Serial.println("STOP: Obstacle detecte");
  }

  // SECURITE : Watchdog Raspberry Pi
  if (actuelMillis - dernierMessagePi >= timeoutPi) {
    moteurStop();
    robotMouvement = false;
  }

  // GESTION DU TIMING DES MOUVEMENTS
  if (robotMouvement && actuelMillis >= tempsFinAction) {
    moteurStop();
    robotMouvement = false;
    Serial.println("Action terminee");
  }

  // LECTURE DES COMMANDES
  if (Serial.available() > 0) {
    dernierMessagePi = actuelMillis;

    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    int index_separateur = cmd.indexOf(':');
    String action = cmd;
    int valeur = 0;

    if (index_separateur != -1) {
      action = cmd.substring(0, index_separateur);
      valeur = cmd.substring(index_separateur + 1).toInt();
    }

    if (action == "FORWARD") {
      if (distAv > 40) {
        // Temps = (Distance / Vitesse) * 1000 pour conversion en ms
        long duree = (valeur / (float)CM_PAR_SECONDE) * 1000.0;
        tempsFinAction = actuelMillis + duree;
        robotMouvement = true;

        Serial.print("Moteur AVANT | Distance demandee: ");
        Serial.print(valeur);
        Serial.print(" | Duree calculee: ");
        Serial.println(duree);

        moteurAvancer();
      } else {
        Serial.print("OBSTACLE REFUSE | Dist: ");
        Serial.println(distAv);
      }
    } 
    else if (action == "BACKWARD") {
      moteurReculer();
      robotMouvement = false; // Pas de timer sur le recul pour ce mode
      Serial.println("Moteur ARRIERE");
    } 
    else if (action == "LEFT") {
      moteurGauche();
      Serial.println("Pivot GAUCHE");
    }
    else if (action == "RIGHT") {
      moteurDroite();
      Serial.println("Pivot DROITE");
    }
    else if (action == "STOP") {
      moteurStop();
      robotMouvement = false;
      Serial.println("Moteur STOP");
    }
  }
}