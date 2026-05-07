#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h> // pour MSG_DONTWAIT
#include "communication/tcp_server.h"
#include "rbiArduinoInterface/arduino_interface.h"

// TODO: Inclure le header UART de Victor quand il sera prêt

int main()
{
    char buffer[BUFFER_SIZE];
    printf("---CONNEXTION TCP ASYNCHRONE---\n");

    // verification connexion arduino
    if (initSerial("/dev/ttyACM0") != 0)
    {
        fprintf(stderr, "ERREUR de connexion arduino [initSerial] \n");
    }
    else
    {
        printf("[UART] Arduino connecte avec succes.\n");
    }

    sleep(2); // laisse le temp initialisation

    // initialisation du port Serveur
    int server_socket = init_tcp_server(PORT);
    RobotData data; // stock les donner recu par larduino

    // Maintien en vie du serveur (Daemon)
    while (1)
    {
        // Attente de la connexion du client
        int client_socket = accept_client(server_socket);

        // Lecture continue tant que le client es connecter
        while (1)
        {
            /*int octetslus = read_message(client_socket, buffer, BUFFER_SIZE);*/
            int octetslus = recv(client_socket, buffer, BUFFER_SIZE, MSG_DONTWAIT);

            // Gestion de la deconnexion ou erreur
            if (octetslus == 0)
            {
                printf("[SERVEUR] Fin de session avec ce client.\n");
                break; // On sort de la sous-boucle, on va fermer le socket
            }
            else if (octetslus > 0)
            {
                // On a reçu une vraie commande, on la traite
                buffer[strcspn(buffer, "\r\n")] = '\0';

                printf("\n[GUI DIT] : %s\n", buffer);
                fflush(stdout);

                char action[10];
                int valeur = 0;

                if (sscanf(buffer, "%s %d", action, &valeur) >= 1)
                {
                    sendCommand(action, valeur);
                    printf("[UART -> ARDUINO] : %s %d\n", action, valeur);
                }
            }

            // ECOUTE ARDUINO (on lit à chaque tour)
            char *msg = readArduino();
            if (msg != NULL && parseTelemetry(msg, &data))
            {
                // mémoriser l'état au tour de boucle précédent
                static int etat_precedent = 0;

                // Le robot vient de s'arrêter
                if (etat_precedent == 1 && data.state == 0)
                {
                    printf("[PIPELINE] Mouvement '%s' termine avec succes. Robot disponible.\n", data.cmd);
                }

                // Mouvement ou Danger
                if (data.state == 1 || data.distAv <= 20)
                {
                    printf("[ARDUINO -> PI] Etat: %d | Action: %s | Obstacle: %d cm\n",
                           data.state, data.cmd, data.distAv);
                }

                // Mise à jour de la mémoire
                etat_precedent = data.state;
            }

            // securite batement de coeur de l'arduino
            static int heartbeat_counter = 0;
            heartbeat_counter++;
            if (heartbeat_counter >= 5) // MODIFICATION: 5 au lieu de 20
            {
                sendCommand("PING", 0);
                heartbeat_counter = 0;
            }

            // Respiration du système (10 millisecondes)
            usleep(10000);
        }

        close(client_socket);
        printf("[SERVEUR] Ligne client fermee. Retour a l'ecoute...\n");
    }
    return 0;
}