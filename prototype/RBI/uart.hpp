#ifndef UART_HPP
#define UART_HPP

#include <string>

// Ouvre le port série et attend le boot Arduino (2s)
// Retourne true si connexion réussie
bool uartInit(const std::string &port, int baud);

// Ferme le port proprement
void uartClose();

// Envoie "CMD:VALEUR\n" et écoute les réponses pendant duree_ms millisecondes
void envoieCommande(const std::string &cmd, int valeur, int duree_ms);

#endif