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
const long intervalle = 500;
AF_DCMotor moteur(1);

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

void moteurAvancer()
{
  Serial.println("Moteur AVANT");
  moteur.run(FORWARD);
}

void moteurReculer()
{
  Serial.println("Moteur ARRIERE");
  moteur.run(BACKWARD);
}

void moteurStop()
{
  Serial.println("Moteur STOP");
  moteur.run(RELEASE);
}

void setup()
{
  pinMode(trig1Pin, OUTPUT);
  pinMode(echo1Pin, INPUT);

  Serial.begin(115200);
  Serial1.begin(230400);

  moteur.setSpeed(200);
  moteur.run(RELEASE);

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
  if (actuelMillis - precedentMillis >= intervalle)
  {
    precedentMillis = actuelMillis;

    Serial.println(lectureUltrasonsAvant());
  }
}
