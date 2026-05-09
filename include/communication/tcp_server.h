#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <netinet/tcp.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 5000
#define BUFFER_SIZE 1024

// TODO : Augmenter légèrement le BUFFER_SIZE si tu prévois d'envoyer des chaînes plus longues pour le débug du mode auto.

/**
 * @brief Initialise le port (socket, bind, listen)
 * @return la socket du serveur
 */
int init_tcp_server(int port);

/**
 * @brief Bloque le programme jusqu'à ce que Joan se connecte
 * @return le socket du client
 */
int accept_client(int server_socket);

/**
 * @brief Lit un seul message sur la ligne TCP
 * @return le nombre d'octets lus (0 = deconnexion, <0 = erreur)
 */
int read_message(int client_socket, char *buffer, int buffer_size);

/**
 * @brief envoie un message
 */
int send_message(int client_socket, const char *message);

#endif