/**
 * @author GHOUILEM ABDELHAFIDH
 * code pour communication entre le PC et le RBI avec le protocole TCP
 * TODO:
 * socket() : Demande à l'OS Linux de créer un point de communication réseau (IPv4, TCP).
 * bind() : Attache ce socket au port de ton choix (ex: 8080).
 * listen() : Met la Raspberry Pi en mode "écoute passive". Le programme se fige ici en attendant le GUI.
 * accept() : Le GUI de Joan se connecte. La connexion est validée (Handshake). Un nouveau canal sécurisé est créé.
 * recv() : La Raspberry Pi lit la chaîne de caractères brute envoyée par le réseau (ex: "AVANCE DE 100").
 * Traitement Métier : Tu injectes ce texte dans ta fonction tokenize_sentence (Cutter), tu l'analyses, et tu extrais la command.
 * Exécution UART : Tu envoies "FORWARD:100" à l'Arduino via le code de Victor.
 * close() : Fermeture de la ligne une fois la commande exécutée.
 */

#include "communication/tcp_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

/**
 * @brief Configuration de l'adresse IP et du port
 */
struct sockaddr_in configurationAdresse(int port)
{
    struct sockaddr_in address;
    address.sin_family = AF_INET;         // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // Écoute sur n'importe quelle IP locale (dont la RPi)
    address.sin_port = htons(port);       // Conversion au format réseau (Big Endian)
    return address;
}

/**
 * @brief LIAISON (SOCKET <=> PORT)
 */
void liaisonBind(int server_socket, struct sockaddr_in address)
{
    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("ERREUR: echec du bind (liaison socket <=> port)");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Initialise le port (socket, bind, listen)
 * @return l'ID de la prise (File Descriptor)
 */
int init_tcp_server(int port)
{
    int server_socket;

    // Création du socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("ERREUR: impossible de creer la socket");
        exit(EXIT_FAILURE);
    }

    // sécurité anti-blocage de port apres le ctrl+C
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("ERREUR: setsockopt a echoue");
        exit(EXIT_FAILURE);
    }

    // Configuration de l'adresse
    struct sockaddr_in address = configurationAdresse(port);

    // LIAISON (SOCKET <=> PORT)
    liaisonBind(server_socket, address);

    // MISE EN ÉCOUTE PASSIVE
    // '3' est la taille de la file d'attente système (backlog)
    if (listen(server_socket, 3) < 0)
    {
        perror("ERREUR: [init_tcp_serveur()] echec du listen du serveur");
        exit(EXIT_FAILURE);
    }

    printf("[SERVEUR] En ecoute sur le port %d...\n", port);

    return server_socket;
}

/**
 * @brief Bloque le programme, attend le GUI, lit la phrase reçue et la stocke dans 'buffer'
 * @param serveur_socket fait precedement ecoute avec init_tcp_server qui crée un pointeur vers un 'buffer'
 */
void wait_and_read_message(int serveur_socket, char *buffer, int buffer_size)
{
    // ne rien faire tant que le GUI nest pas connecter
    int client_socket = accept(serveur_socket, NULL, NULL);
    if (client_socket < 0)
    {
        perror("ERREUR: [wait_and_read_message()] probleme de connexion");
        exit(EXIT_FAILURE);
    }

    // nettoyer la memoir
    memset(buffer, 0, buffer_size); // forcer les caractére du buffer a \0

    // lire les messages recu en TCP
    read(client_socket, buffer, buffer_size); // on va ecrire la phrase recu dans le buffer

    // fermer la ligne TCP apres reception
    close(client_socket);
}