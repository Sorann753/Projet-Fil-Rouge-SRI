#include "uart.hpp"

#include <iostream>
#include <string>
#include <chrono>
#include <thread>

// POSIX — disponible sur Raspberry Pi (Linux)
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>

static int fd = -1; // descripteur du port série

// ---------- fonctions internes ----------

static bool setPortOptions(int baud)
{
    struct termios tty;
    if (tcgetattr(fd, &tty) != 0)
    {
        std::cerr << "[ERREUR] tcgetattr : " << strerror(errno) << std::endl;
        return false;
    }

    // vitesse
    speed_t speed = (baud == 115200) ? B115200 : B9600;
    cfsetispeed(&tty, speed);
    cfsetospeed(&tty, speed);

    // 8N1, pas de contrôle de flux
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= CREAD | CLOCAL;

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_lflag = 0;
    tty.c_oflag = 0;

    // timeout : 100ms, lecture d'au moins 0 caractère
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        std::cerr << "[ERREUR] tcsetattr : " << strerror(errno) << std::endl;
        return false;
    }
    return true;
}

// lit une ligne depuis le port (bloquant jusqu'à '\n' ou timeout interne)
static std::string lireLigne()
{
    std::string ligne;
    char c;
    while (true)
    {
        int n = read(fd, &c, 1);
        if (n <= 0)
            break; // timeout VTIME écoulé sans donnée
        if (c == '\n')
            break;
        if (c != '\r')
            ligne += c;
    }
    return ligne;
}

// vide le tampon de réception
static void viderTampon()
{
    tcflush(fd, TCIFLUSH);
}

// ---------- API publique ----------

bool uartInit(const std::string &port, int baud)
{
    std::cout << "[LOG] Démarrage UART sur " << port << "..." << std::endl;

    // vérification existence du port
    if (access(port.c_str(), F_OK) != 0)
    {
        std::cerr << "[ERREUR] Port " << port << " introuvable. Lance 'ls /dev/ttyACM*'" << std::endl;
        return false;
    }
    std::cout << "[LOG] Port " << port << " détecté" << std::endl;

    // ouverture
    fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0)
    {
        std::cerr << "[ERREUR] Impossible d'ouvrir " << port << " : " << strerror(errno) << std::endl;
        return false;
    }

    if (!setPortOptions(baud))
    {
        close(fd);
        fd = -1;
        return false;
    }

    std::cout << "[LOG] Connexion physique réussie" << std::endl;

    // attente boot Arduino
    std::cout << "[LOG] Attente du réveil de l'Arduino (2s)..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));

    viderTampon();
    std::cout << "[LOG] Tampon vidé, prêt" << std::endl;

    return true;
}

void uartClose()
{
    if (fd >= 0)
    {
        close(fd);
        fd = -1;
        std::cout << "[LOG] Port fermé proprement" << std::endl;
    }
}

void envoieCommande(const std::string &cmd, int valeur, int duree_ms)
{
    if (fd < 0)
    {
        std::cerr << "[ERREUR] Port non ouvert" << std::endl;
        return;
    }

    // formatage et envoi
    std::string trame = cmd + ":" + std::to_string(valeur) + "\n";
    write(fd, trame.c_str(), trame.size());
    std::cout << "[PI -> ARDUINO] Envoi de " << cmd << ":" << valeur << std::endl;

    // écoute pendant duree_ms
    auto t_fin = std::chrono::steady_clock::now() + std::chrono::milliseconds(duree_ms);

    while (std::chrono::steady_clock::now() < t_fin)
    {
        std::string ligne = lireLigne();

        if (!ligne.empty())
            std::cout << "[ARDUINO] " << ligne << std::endl;
        else
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}