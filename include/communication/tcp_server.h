#ifndef TCP_SERVER_H
#define TCP_SERVER_H

/**
 * @brief Initialise le port (socket, bind, listen)
 * @return l'ID de la prise (File Descriptor)
 */
int init_tcp_server(int port);

/**
 * @brief Bloque le programme, attend le GUI, lit la phrase reçue et la stocke dans 'buffer'
 */
void wait_and_read_message(int server_fd, char *buffer, int buffer_size);

#endif