import serial
import time

# Configuration
PORT = '/dev/ttyACM0' # À vérifier avec ls /dev/tty*
BAUD = 115200

print(f"[LOG] Démarrage du test UART sur {PORT}...")

try:
    # Initialisation
    ser = serial.Serial(PORT, BAUD, timeout=1)
    print("[LOG] Connexion physique établie.")
    # L'Arduino reboot à la connexion, on attend qu'elle soit réveillée
    print("[LOG] Attente du réveil de l'Arduino (2s)...")
    time.sleep(2)
    # 1. On vide le tampon (nettoyage)
    ser.reset_input_buffer()
    

    # 2. Envoi de la commande
    """
    commande = "M:150,150\n"
    print(f"[PI -> ARDUINO] Envoi : {commande.strip()}")
    ser.write(commande.encode('utf-8'))

    # 3. Réception de la réponse
    print("[LOG] Attente de la réponse...")
    reponse = ser.readline().decode('utf-8').strip()

    if reponse:
        print(f"[ARDUINO -> PI] Réponse reçue : {reponse}")
        if "ACK" in reponse:
            print("[RESULTAT] Succès ! Le robot a bien compris l'ordre.")
    else:
        print("[ERREUR] L'Arduino ne répond pas. Vérifie le code C++ ou le câble.")

    ser.close()
    """

    #envoie de commande avance
    print("envoie de 'FORWARD'")
    ser.write(b"FORWARD\n") #envoie de "FORWARD" en binaire
    time.sleep(2) #attendre laction physique
    
    #envoie de commande avance
    print("envoie de 'STOP'")
    ser.write(b"STOP\n")
    time.sleep(2) #attendre laction physique

    #traitement de la reponse stop
    reponse_stop = ser.readline().decode('utf-8').strip()
    print(f"[ARDUINO -> PI] Réponse reçue : {reponse_stop}")

    ser.close()
    print("fin du test")

    
    
except Exception as e:
    print(f"[ERREUR CRITIQUE] : {e}")

    