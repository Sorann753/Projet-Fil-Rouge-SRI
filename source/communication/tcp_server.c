/**
 * @author GHOUILEM ABDELHAFIDH
 */
#include "communication/tcp_server.h"

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
bool liaisonBind(int server_socket, struct sockaddr_in address)
{
    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("ERREUR: echec du bind (liaison socket <=> port)");
        return false;
    }

    return true;
}

/*verification securié*/
bool testServeurSocket(int server_socket)
{
    if (server_socket < 0)
    {
        perror("ERREUR: impossible de creer la socket");
        return false;
    }

    return true;
}
bool testAntiBlocage(int server_socket)
{
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("ERREUR: setsockopt a echoue");
        return false;
    }

    return true;
}
bool testlisten(int server_socket)
{
    // '3' est la taille de la file d'attente système (backlog)
    if (listen(server_socket, 3) < 0)
    {
        perror("ERREUR: [init_tcp_serveur()] echec du listen du serveur");
        return false;
    }

    return true;
}
bool testClientSocket(int client_socket)
{
    if (client_socket < 0)
    {
        perror("ERREUR: [wait_and_read_message()] probleme de connexion");
        return false;
    }

    return true;
}
bool testNagle(int client_socket)
{
    int flag = 1;
    if (setsockopt(client_socket, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int)) < 0)
    {
        perror("ERREUR: Impossible de desactiver Nagle (TCP_NODELAY)");
        return false;
    }

    return true;
}

/**
 * @brief Initialise le port (socket, bind, listen)
 * @return la socket du serveur
 */
int init_tcp_server(int port)
{
    int server_socket;

    // Création du socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    testServeurSocket(server_socket);

    // TODO : add checkup for errors
    // sécurité anti-blocage de port apres le ctrl+C
    testAntiBlocage(server_socket);

    // Configuration de l'adresse
    struct sockaddr_in address = configurationAdresse(port);

    // LIAISON (SOCKET <=> PORT)
    liaisonBind(server_socket, address);

    // MISE EN ÉCOUTE PASSIVE
    testlisten(server_socket);

    printf("[SERVEUR] En ecoute sur le port %d...\n", port);

    return server_socket;
}

/**
 * @brief Bloque le programme jusqu'à ce que le GUI se connecte
 * @return le socket du client
 */
int accept_client(int server_socket)
{
    printf("[SERVEUR] En attente de connexion du GUI...\n");
    int client_socket = accept(server_socket, NULL, NULL);
    testClientSocket(client_socket);
    printf("[SERVEUR] GUI Connecte ! Ligne ouverte.\n");

    testNagle(client_socket);
    return client_socket;
}

/**
 * @brief Lit un seul message sur la ligne
 * @return le nombre d'octets lus (0 = deconnexion, <0 = erreur)
 */
int read_message(int client_socket, char *buffer, int buffer_size)
{
    memset(buffer, 0, buffer_size);
    return recv(client_socket, buffer, buffer_size, 0);
}

/**
 * @brief envoie un message
 */
int send_message(int client_socket, const char *message)
{
    // On envoie la chaîne de caractères sur la socket du client
    return send(client_socket, message, strlen(message), 0);
}