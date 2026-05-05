#include "components/motor.hpp"
#include "constants.hpp"

AF_DCMotor moteurAvD(1);
AF_DCMotor moteurAvG(2);
AF_DCMotor moteurArG(3);
AF_DCMotor moteurArD(4);

void vitesseAvantApres(int vitesse, int vitesseG)
{
  Serial.print("[MOTEUR] PWM Droit: ");
  Serial.print(vitesse);
  Serial.print(" | PWM Gauche (Corrigé): ");
  Serial.println(vitesseG);
}
void setspeedroue(int vitesse)
{
  int vitesseG = constrain(vitesse + K_CORRECTION_GAUCHE, 0, 255); // pour ne pas depasser 255

  // log vitesse avant et apres correction
  vitesseAvantApres(vitesse, vitesseG);

  int vitesseD = constrain(vitesse, 0, 255);

  moteurAvG.setSpeed(vitesseG);
  moteurAvD.setSpeed(vitesse);
  moteurArG.setSpeed(vitesseG);
  moteurArD.setSpeed(vitesse);
}

void moteurAvancer()
{
  moteurAvG.run(FORWARD);
  moteurAvD.run(FORWARD);
  moteurArG.run(FORWARD);
  moteurArD.run(FORWARD);
}

void moteurReculer()
{
  moteurAvG.run(BACKWARD);
  moteurAvD.run(BACKWARD);
  moteurArG.run(BACKWARD);
  moteurArD.run(BACKWARD);
}

void moteurGauche()
{
  moteurAvG.run(BACKWARD);
  moteurArG.run(BACKWARD);
  moteurAvD.run(FORWARD);
  moteurArD.run(FORWARD);
}

void moteurDroite()
{
  moteurAvG.run(FORWARD);
  moteurArG.run(FORWARD);
  moteurAvD.run(BACKWARD);
  moteurArD.run(BACKWARD);
}

void moteurStop()
{
  moteurAvG.run(RELEASE);
  moteurAvD.run(RELEASE);
  moteurArG.run(RELEASE);
  moteurArD.run(RELEASE);
}

void moteurTest()
{
  moteurAvD.run(FORWARD); // 1
  delay(1000);
  moteurAvD.run(RELEASE);
  moteurAvG.run(FORWARD); // 2
  delay(1000);
  moteurAvG.run(RELEASE);
  moteurArG.run(FORWARD); // 3
  delay(1000);
  moteurArG.run(RELEASE);
  moteurArD.run(FORWARD); // 4
  delay(1000);
  moteurArD.run(RELEASE);
}