#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <Arduino.h>

// Vitesse PWM (0-255)
constexpr int FORWARD_SPEED = 180;
constexpr int TURN_SPEED = 240;

const int K_CORRECTION_GAUCHE = 50;

/* TIMEOUT */
constexpr long timeoutPi = 500;

/* ODOMETRIE*/
const float CM_PAR_SECONDE = 90.16;  // A mesurer physiquement : Vitesse du robot (cm/s) a PWM 200
const float DEG_PAR_SECONDE = 173.2; // vitesse reel = vitesse_mesurer/T avec T= angle_demander/ancienneVitesseMesurer
const float DIST_SECUR = 40.0;

#endif