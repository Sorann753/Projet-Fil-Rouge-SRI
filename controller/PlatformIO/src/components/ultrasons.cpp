#include "components/ultrasons.hpp"

int distAv = 9999, distG = 9999, distD = 9999;

static unsigned long precedentMillisUltrasons = 0;
const long intervalleUltrasons = 1000;

void initUltrasons() {
    pinMode(TRIG1_PIN, OUTPUT);
    pinMode(ECHO1_PIN, INPUT);
    pinMode(TRIG2_PIN, OUTPUT);
    pinMode(ECHO2_PIN, INPUT);
    pinMode(TRIG3_PIN, OUTPUT);
    pinMode(ECHO3_PIN, INPUT);
}

int lectureUltrasons(int trigPin, int echoPin) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duree = pulseIn(echoPin, HIGH, 20000); // Timeout de 20ms
    if (duree == 0) return 9999;
    return duree * 0.034 / 2;
}

void updateUltrasons() {
    unsigned long actuelMillis = millis();

    if (actuelMillis - precedentMillisUltrasons >= intervalleUltrasons) {
        precedentMillisUltrasons = actuelMillis;

        distAv = lectureUltrasons(TRIG3_PIN, ECHO3_PIN);
        distG = lectureUltrasons(TRIG2_PIN, ECHO2_PIN);
        distD = lectureUltrasons(TRIG1_PIN, ECHO1_PIN);

        // Affichage systématique sur Serial1 (Debug/Data)
        Serial1.print("Av:");
        Serial1.print(distAv);
        Serial1.print(" G:");
        Serial1.print(distG);
        Serial1.print(" D:");
        Serial1.println(distD);
    }
}