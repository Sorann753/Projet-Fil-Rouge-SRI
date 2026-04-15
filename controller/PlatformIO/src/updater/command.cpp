#include "updater/command.hpp"
#include "constants.hpp"

Command currentCmd = {"", 0, false};
Command listCmd[10]; 
int indexRead = 0;   
int indexWrite = 0;  


unsigned long dernierMessagePi = 0;
void readCmd() {
  if (Serial.available() > 0) {
    dernierMessagePi = millis();

    String raw = Serial.readStringUntil('\n');
    raw.trim();

    if (raw.length() > 0) {
      int index_separateur = raw.indexOf(':');
      Command nouvelleCmd;
      
      if (index_separateur != -1) {
        nouvelleCmd.action = raw.substring(0, index_separateur);
        nouvelleCmd.valeur = raw.substring(index_separateur + 1).toInt();
      } else {
        nouvelleCmd.action = raw;
        nouvelleCmd.valeur = 0;
      }
      nouvelleCmd.active = true;

      writeCmd(nouvelleCmd);
    }
  }
}

void writeCmd(Command commande) {
  int prochainIndex = (indexWrite + 1) % 10;

  if (prochainIndex == indexRead) {
    Serial.println("ERREUR : Buffer plein, commande ignoree");
  } 
  else {
    listCmd[indexWrite] = commande;
    indexWrite = prochainIndex;

    Serial.print("Commande ajoutee a l'index: ");
    Serial.println(indexWrite);
  }
}