#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

const int robotspeed = 200; // vitesse du robot

/* WATCHDOG */
unsigned long dernierMessagePi = 0;
const long timeoutPi = 2000;

/* ODOMETRIE*/
const float CM_PAR_SECONDE = 30.0;  // A mesurer physiquement : Vitesse du robot (cm/s) a PWM 200
const float DEG_PAR_SECONDE = 90.0; // A mesurer physiquement : Vitesse de rotation (deg/s) a PWM 200
const float DIST_SECUR = 40.0;

#endif