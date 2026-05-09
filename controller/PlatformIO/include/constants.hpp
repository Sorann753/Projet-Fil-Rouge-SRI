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
const float CM_PAR_SECONDE = 74.53;  // CM_PAR_SECONDE = CM_PAR_SECONDE * (distMesuree / distDemandee);
const float DEG_PAR_SECONDE = 46.49; // DEG_PAR_SECONDE = DEG_PAR_SECONDE * (angleMesure / angleDemande);
const float DIST_SECUR = 40.0;

#endif