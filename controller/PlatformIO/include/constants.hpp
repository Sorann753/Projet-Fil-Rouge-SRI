#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <Arduino.h>

// Vitesse PWM (0-255)
constexpr int FORWARD_SPEED = 180;
constexpr int TURN_SPEED = 240;

/* TIMEOUT */
constexpr long timeoutPi = 2000;

/* ODOMETRIE*/
const float CM_PAR_SECONDE = 30.0;  // A mesurer physiquement : Vitesse du robot (cm/s) a PWM 200
const float DEG_PAR_SECONDE = 90.0; // A mesurer physiquement : Vitesse de rotation (deg/s) a PWM 200
const float DIST_SECUR = 40.0;

#endif