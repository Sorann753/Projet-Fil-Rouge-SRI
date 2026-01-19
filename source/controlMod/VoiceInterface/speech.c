#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include "controlMod/VoiceInterface/speech.h"

char *get_speech(void) {
    // Buffer statique pour stocker la ligne reçue du script Python.
    // Statique = persiste après la fin de la fonction (nécessaire pour renvoyer un pointeur).
    static char line[1024];

    // Exécute le script Python et récupère sa sortie standard (stdout).
    // "2>/dev/null" permet de masquer les warnings ALSA/JACK générés par Python.
    FILE *python_file = popen("python3 voice_interpreter.py 2>/dev/null", "r");

    // Si popen échoue, on renvoie une chaîne d'erreur simple.
    if (!python_file) {
        return (char *)"ERROR";
    }

    // Lit la première ligne envoyée par le script Python.
    // Si aucune donnée n'est reçue, on copie "err" dans le buffer.
    if (fgets(line, sizeof(line), python_file) == NULL) {
        line[0] = 'e';
        line[1] = 'r';
        line[2] = 'r';
        line[3] = '\0';
    }

    // Ferme le processus Python.
    pclose(python_file);

    // Renvoie la ligne capturée (peut contenir une transcription ou un message d’erreur).
    return line;
}
