#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include "rbiArduinoInterface/arduino_interface.h"

int fd = -1;

/*initialisation du port*/
int initSerial(const char *port)
{
    fd = open(port, O_RDWR | O_NOCTTY); // ouverture du port lecture/ecriture
    if (fd == -1)
        return -1;
    struct termios tty;

    if (tcgetattr(fd, &tty) != 0)
        return -1;

    // definir le baudrate a 115200
    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);

    // CONFIGURATION
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    /*tty.c_lflag |= ICANON; //cest le mode canonique a remplacer par le RAW*/
    // Passage en mode RAW (Désactive le canonique, l'écho console et les signaux)
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    // Désactive les filtres logiciels cachés de Linux (optimisation de vitesse)
    tty.c_oflag &= ~OPOST;
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);

    // Le Chronomètre de Survie (Timeout matériel)
    tty.c_cc[VMIN] = 0;  // 0 = N'attendre aucun octet minimum pour débloquer la lecture
    tty.c_cc[VTIME] = 0; // 0 = Mode instantané

    tcsetattr(fd, TCSANOW, &tty);
    return 0;
}

/*envoie de commande a l'Arduino*/
void sendCommand(const char *action, int value)
{
    if (fd == -1)
        return;
    char buffer[64];
    int n;

    // Si la commande est "S" (Stop), on omet la valeur
    if (strcmp(action, "S") == 0)
    {
        n = snprintf(buffer, sizeof(buffer), "S\n");
    }
    // le batement de coeur
    else if (strcmp(action, "PING") == 0)
    {
        n = snprintf(buffer, sizeof(buffer), "\n");
    }
    else
    {
        // Remplacement du ':' par un espace ' '
        n = snprintf(buffer, sizeof(buffer), "%s %d\n", action, value); // la commande a envoyer
    }

    write(fd, buffer, n); // pousser les octets dans le cable USB vers larduino
}

/* Lit le retour de l'Arduino de manière robuste (Anti-fragmentation) */
char *readArduino(void)
{
    static char buffer[256]; // Stockage entre deux appels
    static int pos = 0;      // Mémorise où on en est dans la phrase
    char c;

    // Grâce au mode RAW (VTIME=1), ce read ne bloquera jamais plus de 100ms
    while (read(fd, &c, 1) > 0)
    {
        // Fin du message détectée
        if (c == '\n')
        {
            buffer[pos] = '\0';
            pos = 0; // On reset pour le prochain message

            // Si c'est bien de la télémetrie, on la renvoie
            if (strncmp(buffer, "TELE;", 5) == 0)
            {
                return buffer;
            }
            return NULL; // On ignore les bruits parasites
        }

        // Accumulation des caractères normaux
        if (c != '\r' && pos < 254)
        {
            buffer[pos] = c;
            pos++;
        }
    }

    // Si on arrive ici, c'est que le timeout (100ms) a frappé avant le '\n'

    return NULL;
}

/*stocket les donner recuperer depuis l'arduino*/
int parseTelemetry(const char *line, RobotData *data)
{
    if (line == NULL)
        return 0;
    if (strncmp(line, "TELE;", 5) != 0)
        return 0;

    // on range chaque data dans la structure RobotData
    int parsed = sscanf(line, "TELE;%d;%[^;];%d;%d;%d;%d",
                        &data->state,
                        data->cmd,
                        &data->duration,
                        &data->distAv,
                        &data->distG,
                        &data->distD);
    return (parsed == 6);
}


void closeArduino(void){
    if(close(fd) != 0){
        // weird case
        perror("[WARNING] - The arduino UART interface wasn't closed properly for some reason\n");
    }
}