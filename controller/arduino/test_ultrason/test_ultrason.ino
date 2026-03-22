// Définition des broches
const int trigPin = 22;
const int echoPin = 23;

long duree;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT); // Le Trig envoie l'onde
  pinMode(echoPin, INPUT);  // L'Echo reçoit l'onde
  Serial.begin(9600);       // On ouvre le moniteur série
}

void loop() {
  // 1. On envoie une onde ultra-son de 10 microsecondes
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // 2. On mesure le temps que l'onde met à revenir
  duree = pulseIn(echoPin, HIGH);

  // 3. Calcul de la distance : Vitesse du son = 340 m/s
  // Formule : Distance = (Temps * 0.034) / 2
  distance = duree * 0.034 / 2;

  // 4. On affiche le résultat sur le PC
  Serial.print("Distance : ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(200); // Petite pause pour la lisibilité
}