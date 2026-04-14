#ifndef MOTOR_HPP
#define MOTOR_HPP

#include <Arduino.h>
#include <AFMotor.h>

extern AF_DCMotor moteurAvG;
extern AF_DCMotor moteurAvD;
extern AF_DCMotor moteurArG;
extern AF_DCMotor moteurArD;

/**
 * @brief Configuration de la vitesse (0-255)
 */
void setspeedroue(int vitesse);

/**
 * @brief Direction : Tout droit
 */
void moteurAvancer();

/**
 * @brief Direction : Arrière
 */
void moteurReculer();

/**
 * @brief Direction : Pivot Gauche (sur place)
 */
void moteurGauche();

/**
 * @brief Direction : Pivot Droite (sur place)
 */
void moteurDroite();

/**
 * @brief Arrêt moteur (roue libre)
 */
void moteurStop();

#endif