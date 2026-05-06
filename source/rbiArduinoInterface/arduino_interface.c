#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "rbiArduinoInterface/arduino_interface.h"

int fd = -1;

int initSerial(const char* port) {
    fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) return -1;

    struct termios tty;
    if (tcgetattr(fd, &tty) != 0) return -1;

    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);

    tty.c_cflag |= (CLOCAL | CREAD);    // Activer lecture
    tty.c_cflag &= ~PARENB;            // Pas de parité
    tty.c_cflag &= ~CSTOPB;            // 1 bit de stop
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;                // 8 bits de données

    tty.c_lflag |= ICANON;
    
    tcsetattr(fd, TCSANOW, &tty);
    return 0;
}

void sendCommand(const char* action, int value) {
    if (fd == -1) return;
    char buffer[64];
    int n = snprintf(buffer, sizeof(buffer), "%s:%d\n", action, value);
    write(fd, buffer, n);
}

char* readArduino() {
    static char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    int n = read(fd, buffer, sizeof(buffer) - 1);
    if (n > 0) return buffer;
    return NULL;
}