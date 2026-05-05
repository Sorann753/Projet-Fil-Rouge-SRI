#include "uart.hpp"

#include <iostream>
#include <string>
#include <cstring>

// Bibliothèques Linux pour le port série et la lecture non-bloquante
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include <sys/time.h>

// ─── Implémentation ───────────────────────────────────────────────────────────

int uart_ouvrir(const std::string &port)
{
    // Ouverture du port en lecture/écriture, sans terminal de contrôle, non-bloquant
    int fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd < 0)
    {
        std::cerr << "[ERREUR] Impossible d'ouvrir le port " << port
                  << " — vérifier avec : ls /dev/ttyACM*" << std::endl;
        return -1;
    }

    // Configuration du port série via termios
    termios tty;
    memset(&tty, 0, sizeof(tty));

    if (tcgetattr(fd, &tty) != 0)
    {
        std::cerr << "[ERREUR] tcgetattr échoué" << std::endl;
        close(fd);
        return -1;
    }

    // Vitesse d'entrée et de sortie à 115200 bauds
    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    // Mode raw : désactive l'interprétation des caractères spéciaux (Ctrl+C, etc.)
    cfmakeraw(&tty);

    // 8 bits de données, pas de parité, 1 bit de stop (8N1)
    tty.c_cflag |= (CLOCAL | CREAD); // Active la réception et ignore modem
    tty.c_cflag &= ~CRTSCTS;         // Désactive le contrôle de flux matériel

    // Lecture bloquante avec timeout de 1s (pour les appels read() bloquants)
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 10; // 1 seconde (en dixièmes de secondes)

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        std::cerr << "[ERREUR] tcsetattr échoué" << std::endl;
        close(fd);
        return -1;
    }

    std::cout << "[LOG] Port " << port << " ouvert et configuré (115200 bauds, 8N1)" << std::endl;

    // Attente du reboot Arduino (il reboot à chaque connexion USB)
    std::cout << "[LOG] Attente du réveil de l'Arduino (" << UART_BOOT_DELAY_MS << "ms)..." << std::endl;
    usleep(UART_BOOT_DELAY_MS * 1000);

    // Vide le tampon d'entrée pour ne pas lire des données résiduelles
    tcflush(fd, TCIFLUSH);
    std::cout << "[LOG] Tampon vidé — liaison prête" << std::endl;

    return fd;
}

void uart_fermer(int fd)
{
    if (fd >= 0)
    {
        close(fd);
        std::cout << "[LOG] Port série fermé proprement" << std::endl;
    }
}

void uart_ecouter(int fd, int timeout_ms)
{
    std::string ligne_en_cours;

    // Calcul de l'heure de fin d'écoute
    auto t_fin = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout_ms);

    while (std::chrono::steady_clock::now() < t_fin)
    {
        // Calcul du temps restant pour le select()
        auto restant = std::chrono::duration_cast<std::chrono::microseconds>(
                           t_fin - std::chrono::steady_clock::now())
                           .count();

        if (restant <= 0)
            break;

        // Prépare le fd_set pour select()
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(fd, &read_fds);

        timeval tv;
        tv.tv_sec = restant / 1'000'000;
        tv.tv_usec = restant % 1'000'000;

        int ret = select(fd + 1, &read_fds, nullptr, nullptr, &tv);

        if (ret < 0)
        {
            std::cerr << "[WARN] select() échoué" << std::endl;
            break;
        }

        if (ret == 0)
        {
            // Timeout select → fin de la fenêtre d'écoute
            break;
        }

        // Données disponibles → on lit caractère par caractère pour reconstruire les lignes
        if (FD_ISSET(fd, &read_fds))
        {
            char c;
            ssize_t n = read(fd, &c, 1);

            if (n > 0)
            {
                if (c == '\n')
                {
                    // Ligne complète reçue
                    if (!ligne_en_cours.empty())
                    {
                        std::cout << "[ARDUINO] " << ligne_en_cours << std::endl;
                        ligne_en_cours.clear();
                    }
                }
                else if (c != '\r')
                {
                    ligne_en_cours += c;
                }
            }
        }
    }
}

void uart_envoyer_commande(int fd, const std::string &action, int valeur, int duree_ecoute_ms)
{
    // Formatage de la trame : "ACTION:VALEUR\n"
    std::string trame = action + ":" + std::to_string(valeur) + "\n";

    ssize_t n = write(fd, trame.c_str(), trame.size());

    if (n < 0)
    {
        std::cerr << "[ERREUR] Échec de l'envoi de la commande : " << action << std::endl;
        return;
    }

    std::cout << "[PI -> ARDUINO] " << action << ":" << valeur << std::endl;

    // Écoute des réponses de l'Arduino pendant duree_ecoute_ms
    uart_ecouter(fd, duree_ecoute_ms);
}