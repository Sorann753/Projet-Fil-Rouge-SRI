#include <stdio.h>
#include <stdlib.h>
#include "communication/tcp_server.h"

// TODO: Inclure tes headers PFR1 (cutter.h, parser.h, etc.)

#define PORT 5000
#define BUFFER_SIZE 1024

int main_server()
{
    // definir le buffer
    char buffer[BUFFER_SIZE];

    printf("CONNEXION TCP au PC\n");

    // initialisation du port et recup l'ID
    int server_socket = init_tcp_server(PORT);

    while (1)
    {

        wait_and_read_message(server_socket, buffer, BUFFER_SIZE);
        printf("[GUI DIT] : %s\n", buffer);

        /*TODO: envoyer le buffer a tokenize_sentence()*/
    }
    return 0;
}