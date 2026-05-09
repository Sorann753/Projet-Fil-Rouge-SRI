#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h> // pour MSG_DONTWAIT
#include "communication/tcp_server.h"
#include "rbiArduinoInterface/arduino_interface.h"

enum MODE mode = MANUEL; // 0 = MANUEL (par défaut), 1 = AUTONOME

/**
 * @brief envoi des données de telemetrie au GUI en trame (DATA;...)
 * @param data Structure contenant l'état actuel, la commande, la distance et le temps restant.
 *
 */
void envoieTelemetrie(RobotData data, int client_socket)
{
    char feedback[128]; // buffer denvoi
    sprintf(feedback, "DATA;%d;%s;%d;%d\n", data.state, data.cmd, data.distAv, data.duration);
    send_message(client_socket, feedback);
}

/**
 * @brief autonome : si pas dobstacle avance de 30 si obstacle recule de 30 et tourne droite 90°
 */
void modeAutonome(RobotData *data)
{
    if (mode == AUTONOME && data->state == 0)
    {
        if (data->distAv > DIST_LIMITE_AVANT)
        {
            sendCommand("F", 30);
            printf("[AUTONOME] Avance de 30cm (Obstacle: %d cm)\n", data->distAv);
        }
        else
        {
            // Evitement dobstacle
            sendCommand("B", 30); // recule
            sendCommand("R", 90); // tourne
            printf("[AUTONOME] Obstacle à %d cm ! Recul puis rotation droite.\n", data->distAv);
        }

        // modifie la vraie variable du main grâce au pointeur
        data->state = 1;
    }
}

/**
 * @brief traitement de la chaîne TCP reçue du GUI
 * Intercepte les commandes de securité (S) et changement de mode (M)
 * en mode MANUEL : envoie les ordres de mouvement (F, B, R, L) à l'Arduino
 * @param buffer La chaîne de caractères brute reçue sur le port 5000 en TCP
 */
void traitementCommande(char *buffer)
{
    buffer[strcspn(buffer, "\r\n")] = '\0';

    printf("\n[GUI DIT] : %s\n", buffer);
    fflush(stdout);

    char action[10];
    int valeur = 0;

    // parsing du buffer recu par le GUI
    if (sscanf(buffer, "%s %d", action, &valeur) >= 1)
    {
        // si on recoi un "S"
        if (strcmp(action, "S") == 0)
        {
            mode = MANUEL;       // forcer le passage en mode manuelle
            sendCommand("S", 0); // envoie dun stop
            printf("[STOP] Arrêt d'urgence et retour MANUEL.\n");
        }
        // si on recoie un "M"
        else if (strcmp(action, "M") == 0)
        {
            // mode autonome
            if (valeur == 1)
            {
                mode = AUTONOME;
                printf("[AUTONOME] mode autonome activé\n");
            }
            // mode manuel
            else
            {
                mode = MANUEL;
                printf("[MANUEL] mode manuel activé\n");
            }
            sendCommand("S", 0); // on stop lors du changement de mode
        }
        else if (mode == MANUEL)
        {
            sendCommand(action, valeur);
            printf("[UART -> ARDUINO] : %s %d\n", action, valeur);
        }
        else
        {
            printf("[AUTONOME] commande Ignoré : Le robot est en mode AUTONOME.\n");
        }
    }
}

/**
 * @brief ecoute le retour (ARDUINO => RBI) et rempli la structure data
 * Détecte les fins de mouvements (Transition 1 -> 0), affiche les logs critiques (Obstacles)
 * transfert des données vers le GUI.
 */
void ecouteArduino(RobotData *data, int client_socket)
{
    char *msg = readArduino();
    if (msg != NULL && parseTelemetry(msg, data))
    {

        // mémoriser l'état au tour de boucle précédent
        static int etat_precedent = 0;

        // Le robot vient de s'arrêter
        if (etat_precedent == 1 && data->state == 0)
        {
            printf("[PIPELINE] Mouvement '%s' termine avec succes. Robot disponible.\n", data->cmd);
        }

        // Mouvement ou Danger
        if (data->state == 1 || data->distAv <= 20)
        {
            printf("[ARDUINO -> PI] Etat: %d | Action: %s | Obstacle: %d cm\n",
                   data->state, data->cmd, data->distAv);
        }

        // Mise à jour de la mémoire
        etat_precedent = data->state;

        // ENVOIE des data au GUI
        envoieTelemetrie(*data, client_socket);
    }
}

/**
 * @brief envoie un PING tout les 50ms pour reinitialisation du watchdog arduino
 */
void heartbeat()
{
    static int heartbeat_counter = 0;
    heartbeat_counter++;
    if (heartbeat_counter >= 5) // MODIFICATION: 5 au lieu de 20
    {
        sendCommand("PING", 0);
        heartbeat_counter = 0;
    }
}

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

                // securité arret et retour en MANUEL
                mode = MANUEL;
                sendCommand("S", 0);

                break; // On sort de la sous-boucle, on va fermer le socket
            }
            else if (octetslus > 0)
            {
                // On a reçu une vraie commande on la traite
                traitementCommande(buffer);
            }

            // ECOUTE ARDUINO (on lit à chaque tour)
            ecouteArduino(&data, client_socket);

            // securite batement de coeur de l'arduino
            heartbeat();

            // DECISION AUTONOME
            modeAutonome(&data);

            // Respiration du système (10 millisecondes)
            usleep(10000);
        }

        close(client_socket);
        printf("[SERVEUR] Ligne client fermee. Retour a l'ecoute...\n");
    }
    return 0;
}