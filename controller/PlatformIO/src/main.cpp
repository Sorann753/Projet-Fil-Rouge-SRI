#include <Arduino.h>
#include <AFMotor.h>

const int robotspeed = 200; // vitesse du robot

/* WATCHDOG */
unsigned long dernierMessagePi = 0;
const long timeoutPi = 2000;

/*ULTRASON 1 */
const int trig1Pin = 47;
const int echo1Pin = 46;

/*ULTRASON 2 */
const int trig2Pin = 45;
const int echo2Pin = 44;

/*ULTRASON 3 */
const int trig3Pin = 43;
const int echo3Pin = 42;

/* ODOMETRIE*/
const float CM_PAR_SECONDE = 30.0;  // A mesurer physiquement : Vitesse du robot (cm/s) a PWM 200
const float DEG_PAR_SECONDE = 90.0; // A mesurer physiquement : Vitesse de rotation (deg/s) a PWM 200

unsigned long tempsFinAction = 0; // Chronometre d'arret
bool robotMouvement = false;

unsigned long precedentMillis = 0;
const long intervalle = 100;

/* MOTEURS*/
AF_DCMotor moteurAvG(1); // M1 (avant gauche)
AF_DCMotor moteurAvD(2); // M2 (avant droite)
AF_DCMotor moteurArG(3); // M3 (arriére gauche)
AF_DCMotor moteurArD(4); // M4 (arriére droite)

/*Distances */
int distAv = 9999, distG = 9999, distD = 9999;

/**
 * @brief lecture de lultrason
 * @param numero (int) : numero de lultrason a lire (1, 2 ou 3)
 * @return duree (int)
 */
int lectureUltrasons(int trigPin, int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duree = pulseIn(echoPin, HIGH, 20000);
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

  pinMode(trig2Pin, OUTPUT);
  pinMode(echo2Pin, INPUT);

  pinMode(trig3Pin, OUTPUT);
  pinMode(echo3Pin, INPUT);

  Serial.begin(115200);
  Serial1.begin(230400);

  setspeedroue(robotspeed);

  moteurStop();

  Serial.println("Fin Setup - Pret pour test");
}

void loop()
{

  // Ultrasons
  unsigned long actuelMillis = millis();

  /*si la mesure millis ateint un multiple de intervalle on fait l'action*/
  if (actuelMillis - precedentMillis >= intervalle)
  {

    precedentMillis = actuelMillis;

    distAv = lectureUltrasons(trig3Pin, echo3Pin);
    distG = lectureUltrasons(trig2Pin, echo2Pin);
    distD = lectureUltrasons(trig1Pin, echo1Pin);

    /*affichage dans le serial1 (data de lultrason)*/
    Serial1.print("Av:");
    Serial1.print(distAv);
    Serial1.print(" G:");
    Serial1.print(distG);
    Serial1.print(" D:");
    Serial1.println(distD);

    /*DETECTION DOBSTACLE*/
    if (distAv <= 40)
    {
      moteurAvG.run(RELEASE);
      moteurAvD.run(RELEASE);
      moteurArG.run(RELEASE);
      moteurArD.run(RELEASE);
    }
  }

  if (Serial.available() > 0)
  {

    /*test du watchdog */
    dernierMessagePi = millis();

    /*lire la commande recu*/
    String cmd = Serial.readStringUntil('\n'); /*lire la ligne*/
    cmd.trim();                                /*netoyage*/

    int index_separateur = cmd.indexOf(':');
    String action = cmd;
    int valeur = 0;

    /*si le separateur existe*/
    if (index_separateur != -1)
    {
      /*couper la phrase*/
      action = cmd.substring(0, index_separateur);          /*recupere laction*/
      valeur = cmd.substring(index_separateur + 1).toInt(); /*recupére la valeur (cast en int)*/
    }

    if (cmd == "FORWARD")
    {
      if (distAv > 40)
      {
        // calcule du temp daction en fonction de la valeur distance
        // CM_PAR_SECONDE est fixée à 25 cm/s
        // Temps = Distance / Vitesse

        long duree = (valeur / CM_PAR_SECONDE) * 1000.0;

        tempsFinAction = millis() + duree;
        robotMouvement = true;

        // debug
        Serial.print("moteur avant | Valeur: ");
        Serial.print(valeur);

        // Envoie une trace complète de debug
        Serial.print("Moteur AVANT | Dist capteur: ");
        Serial.println(distAv);

        moteurAvG.run(FORWARD);
        moteurAvD.run(FORWARD);
        moteurArG.run(FORWARD);
        moteurArD.run(FORWARD);
      }
      else
      {
        Serial.print("OBSTACLE REFUSE | Dist capteur: ");
        Serial.println(distAv);
      }
    }
    else if (cmd == "BACKWARD")
    {
      moteurReculer();
      Serial.println("le moteur recule ...");
    }
    else if (cmd == "STOP")
    {
      moteurStop();
      Serial.println("Moteur STOP ...");
    }
  }

  if (actuelMillis - dernierMessagePi >= timeoutPi)
  {
    /* On coupe le courant aux roues sans utiliser Serial.println */
    moteurAvG.run(RELEASE);
    moteurAvD.run(RELEASE);
    moteurArG.run(RELEASE);
    moteurArD.run(RELEASE);
  }
}
