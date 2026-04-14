#ifndef ULTRASONS_HPP
#define ULTRASONS_HPP

#include <Arduino.h>

/* Définition des Pins */
#define TRIG1_PIN 47
#define ECHO1_PIN 46
#define TRIG2_PIN 45
#define ECHO2_PIN 44
#define TRIG3_PIN 43
#define ECHO3_PIN 42

/* Variables globales de distance */
extern int distAv, distG, distD;

/**
 * @brief Initialise les pins des ultrasons
 */
void initUltrasons();

/**
 * @brief Mesure la distance pour un capteur spécifique
 */
int lectureUltrasons(int trigPin, int echoPin);

/**
 * @brief Gère la lecture périodique des 3 capteurs et l'affichage (non-bloquant)
 */
void updateUltrasons();

#endif