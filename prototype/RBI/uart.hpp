#ifndef UART_HPP
#define UART_HPP

#include <string>
#include <chrono>

/**
 * @file uart.hpp
 * @brief Interface de communication UART entre la Raspberry Pi et l'Arduino.
 *
 * Gère l'ouverture du port série, l'envoi de commandes au format "ACTION:VALEUR\n",
 * l'écoute des réponses de l'Arduino pendant une durée définie, et la fermeture propre.
 *
 * Pensé pour être utilisé avec une file de commandes (TCP → file → UART) :
 * remplacer le contenu de main() par un lecteur de file quand la GUI sera prête.
 */

// ─── Configuration ────────────────────────────────────────────────────────────

/** @brief Port série de l'Arduino (vérifier avec : ls /dev/ttyACM*) */
constexpr const char *UART_PORT = "/dev/ttyACM0";

/** @brief Vitesse de communication en bauds */
constexpr int UART_BAUD = 115200;

/** @brief Délai d'attente après connexion pour laisser l'Arduino rebooter (ms) */
constexpr int UART_BOOT_DELAY_MS = 2000;

/** @brief Délai d'écoute par défaut après une commande (ms) */
constexpr int UART_ECOUTE_DEFAUT_MS = 2000;

/** @brief Délai entre deux commandes successives (ms) */
constexpr int UART_INTER_CMD_MS = 2000;

// ─── Fonctions ────────────────────────────────────────────────────────────────

/**
 * @brief Ouvre et configure le port série UART.
 *
 * Configure le port en mode raw (pas d'interprétation des caractères spéciaux),
 * vitesse 115200 bauds, 8N1. Vide le tampon d'entrée après connexion pour
 * éviter de lire des données résiduelles de la session précédente.
 *
 * @param port Chemin du port série (ex: "/dev/ttyACM0")
 * @return Descripteur de fichier (fd >= 0) en cas de succès, -1 en cas d'erreur.
 */
int uart_ouvrir(const std::string &port);

/**
 * @brief Ferme proprement le port série.
 *
 * @param fd Descripteur de fichier retourné par uart_ouvrir().
 */
void uart_fermer(int fd);

/**
 * @brief Envoie une commande à l'Arduino et écoute sa réponse.
 *
 * Formate et envoie "ACTION:VALEUR\n" sur le port série, puis lit et affiche
 * tous les messages retournés par l'Arduino pendant duree_ecoute_ms millisecondes.
 * Utilise select() pour une lecture non-bloquante (pas de busy-wait).
 *
 * @param fd             Descripteur de fichier du port série ouvert.
 * @param action         Commande à envoyer (ex: "FORWARD", "LEFT", "STOP").
 * @param valeur         Valeur associée (distance en cm ou angle en degrés). 0 pour STOP.
 * @param duree_ecoute_ms Durée d'écoute des réponses Arduino en millisecondes.
 */
void uart_envoyer_commande(int fd, const std::string &action, int valeur, int duree_ecoute_ms = UART_ECOUTE_DEFAUT_MS);

/**
 * @brief Lit et affiche toutes les lignes disponibles sur le port série.
 *
 * Fonction interne appelée par uart_envoyer_commande(). Utilise select()
 * pour détecter les données disponibles sans bloquer le programme.
 *
 * @param fd         Descripteur de fichier du port série.
 * @param timeout_ms Durée maximale d'écoute en millisecondes.
 */
void uart_ecouter(int fd, int timeout_ms);

#endif