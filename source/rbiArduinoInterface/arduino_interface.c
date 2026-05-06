#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "rbiArduinoInterface/arduino_interface.h"

int fd = -1;

int initSerial(const char* port) {
    fd = open(port, O_RDWR | O_NOCTTY); // O_NDELAY retiré
    if (fd == -1) return -1;
    struct termios tty;
    if (tcgetattr(fd, &tty) != 0) return -1;
    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
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
    int pos = 0;
    char c;
    while (pos < 255) {
        int n = read(fd, &c, 1);
        if (n <= 0) return NULL;
        if (c == '\n') break;
        if (c == '\r') continue;
        buffer[pos++] = c;
    }
    buffer[pos] = '\0';
    if (strncmp(buffer, "TELE;", 5) != 0) return NULL;
    return buffer;
}

int parseTelemetry(char* line, RobotData* data) {
    if (line == NULL) return 0;
    if (strncmp(line, "TELE;", 5) != 0) return 0;
    int parsed = sscanf(line, "TELE;%d;%[^;];%d;%d;%d;%d",
        &data->state,
        data->cmd,
        &data->duration,
        &data->distAv,
        &data->distG,
        &data->distD);
    return (parsed == 6);
}