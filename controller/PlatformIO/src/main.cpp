#include <Arduino.h>
#include <AFMotor.h>

/*ULTRASON 1 */
const int trig1Pin = 47;
const int echo1Pin = 46;

/*ULTRASON 2 */
const int trig2Pin = 45;
const int echo2Pin = 44;

/*ULTRASON 3 */
const int trig3Pin = 43;
const int echo3Pin = 42;

unsigned long precedentMillis = 0;
const long intervalle = 100;

/* MOTEURS*/
AF_DCMotor moteurAvG(1); // M1 (avant gauche)
AF_DCMotor moteurAvD(2); // M2 (avant droite)
AF_DCMotor moteurArG(3); // M3 (arriére gauche)
AF_DCMotor moteurArD(4); // M4 (arriére droite)

/**
 * @brief lecture de lultrason
 * @return duree (int)
 */
int lectureUltrasonsAvant()
{
  digitalWrite(trig1Pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig1Pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig1Pin, LOW);

  long duree = pulseIn(echo1Pin, HIGH, 20000);
  if (duree == 0)
    return 9999;
  return duree * 0.034 / 2;
}

/**
 * @brief initialisation de la vitesse
 */
void setspeedroue(int vitesse)
{
  moteurAvG.setSpeed(vitesse);
  moteurAvD.setSpeed(vitesse);
  moteurArG.setSpeed(vitesse);
  moteurArD.setSpeed(vitesse);
}

/**
 * @brief avancer les 4 roues
 */
void moteurAvancer()
{
  Serial.println("Moteur AVANT");
  moteurAvG.run(FORWARD);
  moteurAvD.run(FORWARD);
  moteurArG.run(FORWARD);
  moteurArD.run(FORWARD);
}

/**
 * @brief reculer les 4 roues
 */
void moteurReculer()
{
  Serial.println("Moteur ARRIERE");
  moteurAvG.run(BACKWARD);
  moteurAvD.run(BACKWARD);
  moteurArG.run(BACKWARD);
  moteurArD.run(BACKWARD);
}

/**
 * @brief arreter les 4 roues
 */
void moteurStop()
{
  Serial.println("Moteur STOP");
  moteurAvG.run(RELEASE);
  moteurAvD.run(RELEASE);
  moteurArG.run(RELEASE);
  moteurArD.run(RELEASE);
}

void setup()
{
  pinMode(trig1Pin, OUTPUT);
  pinMode(echo1Pin, INPUT);

  Serial.begin(115200);
  Serial1.begin(230400);

  setspeedroue(200);

  moteurStop();

  Serial.println("Fin Setup - Pret pour test");
}

void loop()
{
  char c;

  // Bluetooth
  if (Serial.available())
  {
    c = Serial.read();
    Serial1.print(c);
  }

  if (Serial1.available())
  {
    c = Serial1.read();
    Serial.print(c);
  }

  // Ultrasons
  unsigned long actuelMillis = millis();

  /*si la mesure millis ateint un multiple de intervalle on fait l'action*/
  if (actuelMillis - precedentMillis >= intervalle)
  {

    precedentMillis = actuelMillis;

    Serial.println(lectureUltrasonsAvant());
  }
}
