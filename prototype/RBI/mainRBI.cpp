#include "uart.hpp"
#include <iostream>

int main()
{
    const std::string PORT = "/dev/ttyACM0";
    const int BAUD = 115200;

    if (!uartInit(PORT, BAUD))
    {
        std::cerr << "[ERREUR] Initialisation échouée, abandon." << std::endl;
        return 1;
    }

    std::cout << "\n--- TEST 1 : CALIBRATION ---" << std::endl;
    envoieCommande("F", 100, 6000); // attend 6s = 100/30*1000 + marge

    envoieCommande("S", 0, 2000);

    uartClose();
    std::cout << "fin du test" << std::endl;
    return 0;
}