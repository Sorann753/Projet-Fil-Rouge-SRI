#include "uart.hpp"
#include "file_commandes.hpp"

#include <iostream>
#include <queue>
#include <unistd.h>

// ── Configuration ──────────────────────────────────────────────────────────────

static const std::string PORT = "/dev/ttyACM0";
static const int BAUD = 115200;

/**
 * @brief Délai de réveil de l'Arduino après connexion USB (en secondes)
 * @details L'Arduino reboot automatiquement à chaque connexion USB.
 *          Sans cette attente, les premières commandes sont perdues.
 */
static const int DELAI_REVEIL_S = 2;

// ── Remplissage de la file ─────────────────────────────────────────────────────

/**
 * @brief Remplit la file avec les commandes de test
 * @details Calcul de duree_ecoute_ms :
 *          Pour un mouvement : (valeur / CM_PAR_SECONDE) * 1000 + 500 ms de marge
 *          Avec CM_PAR_SECONDE = 30  → FORWARD:50  ≈ 1667 ms → on met 2200
 *          Avec DEG_PAR_SECONDE = 90 → RIGHT:45    ≈  500 ms → on met 1000
 *
 * TODO: remplacer cette fonction par une lecture depuis le serveur TCP
 *       qui remplira cette file dynamiquement depuis la GUI sur le PC.
 *
 * @param file File à remplir
 */
static void remplirCommandes(std::queue<Commande> &file)
{
    ajouterCommande(file, "FORWARD", 50, 2200);
    ajouterCommande(file, "FORWARD", 100, 3800);
    ajouterCommande(file, "RIGHT", 45, 1000);
    ajouterCommande(file, "BACKWARD", 50, 2200);
    ajouterCommande(file, "STOP", 0, 1000);
}

// ── Point d'entrée ─────────────────────────────────────────────────────────────

int main()
{
    std::cout << "[LOG] Démarrage du pilote UART — " << PORT << "\n";

    // Ouverture du port série
    int fd = ouvrirPort(PORT, BAUD);
    if (fd == -1)
        return 1;

    // Attente du réveil de l'Arduino (reboot à la connexion USB)
    std::cout << "[LOG] Attente du réveil de l'Arduino (" << DELAI_REVEIL_S << "s)...\n";
    sleep(DELAI_REVEIL_S);

    // Nettoyage du tampon résiduel
    viderTampon(fd);

    // Remplissage et exécution de la file de commandes
    std::queue<Commande> file;
    remplirCommandes(file);
    executerFile(file, fd);

    // Fermeture propre
    fermerPort(fd);
    std::cout << "[LOG] Fin du programme\n";

    return 0;
}