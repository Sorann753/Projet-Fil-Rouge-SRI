#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // Pour la fonction close()
#include "communication/tcp_server.h"

// TODO: Inclure le header UART de Victor quand il sera prêt

int main_server()
{
    char buffer[BUFFER_SIZE];
    printf("---CONNEXTION TCP---\n");

    // initialisation du port Serveur
    int server_socket = init_tcp_server(PORT);

    // Maintien en vie du serveur (Daemon)
    while (1)
    {
        // Attente de la connexion du client
        int client_socket = accept_client(server_socket);

        // Lecture continue tant que le client es connecter
        while (1)
        {
            int octetslus = read_message(client_socket, buffer, BUFFER_SIZE);

            buffer[strcspn(buffer, "\r\n")] = 0; // retirer

            // Gestion de la deconnexion ou erreur
            if (octetslus <= 0)
            {
                printf("[SERVEUR] Fin de session avec ce client.\n");
                break; // On sort de la sous-boucle, on va fermer le socket
            }

            // Affichage et Relais
            printf("[GUI DIT] : %s\n", buffer);
            fflush(stdout); // forcer laffichage imediat

            // TODO : Envoyer la commande UART
            // envoyer_uart(buffer);
        }

        // Fermeture avant d'attendre une nouvelle connexion
        close(client_socket);
        printf("[SERVEUR] Ligne client fermee. Retour a l'ecoute...\n");
    }

    return 0;
}