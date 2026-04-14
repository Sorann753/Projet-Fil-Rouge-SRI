#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <Arduino.h>

// Vitesse PWM (0-255)
constexpr int robotspeed = 200; 

<<<<<<< Updated upstream
/* ODOMETRIE*/
const float CM_PAR_SECONDE = 30.0;  // A mesurer physiquement : Vitesse du robot (cm/s) a PWM 200
const float DEG_PAR_SECONDE = 90.0; // A mesurer physiquement : Vitesse de rotation (deg/s) a PWM 200
const float DIST_SECUR = 40.0;
=======
// WATCHDOG
constexpr long timeoutPi = 2000;

// ODOMETRIE (Valeurs à calibrer)
constexpr float CM_PAR_SECONDE = 30.0;  
constexpr float DEG_PAR_SECONDE = 90.0; 
>>>>>>> Stashed changes

#endif