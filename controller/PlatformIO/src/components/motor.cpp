#include "components/motor.hpp"

AF_DCMotor moteurAvD(1);
AF_DCMotor moteurAvG(2);
AF_DCMotor moteurArG(3);
AF_DCMotor moteurArD(4);

void setspeedroue(int vitesse)
{
  moteurAvG.setSpeed(vitesse);
  moteurAvD.setSpeed(vitesse);
  moteurArG.setSpeed(vitesse);
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