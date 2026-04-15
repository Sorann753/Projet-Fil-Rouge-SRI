import serial
import time

# Configuration
PORT = '/dev/ttyACM0' # À vérifier avec ls /dev/tty*
BAUD = 115200

print(f"[LOG] Démarrage du test UART sur {PORT}...")

def envoie_commande(cmd, valeur=0, duree_attente=2.0):
    ser.write(f"{cmd}:{valeur}\n".encode('utf-8'))
    print(f"[PI -> ARDUINO] Envoi de {cmd}:{valeur}")

    # ECOUTE PENDANT duree_attente
    t_fin = time.time() + duree_attente #on calcule le temp de fin dans x seconds
    while time.time() < t_fin: #si on ateind le temp de fin
        if ser.in_waiting > 0: #regarde sil y a des message en attente sur le cable
            msg_debug = ser.readline().decode('utf-8').strip()
            print(f"[ARDUINO] {msg_debug}")

    time.sleep(0.1)

try:
    # Initialisation
    ser = serial.Serial(PORT, BAUD, timeout=1)
    print("[LOG] Connexion physique réussi")
    # L'Arduino reboot à la connexion, on attend qu'elle soit réveillée
    print("[LOG] Attente du réveil de l'Arduino (2s)")
    time.sleep(2)

    # On vide le tampon (nettoyage)
    ser.reset_input_buffer()
    

    #envoie de commande avance
    envoie_commande("FORWARD", 50)
    
    #envoie de la commande reculer
    envoie_commande("BACKWARD", 60)
    

    #envoie de la commande tourner gauche
    envoie_commande("LEFT",45)
    

    #envoie de la commande tourner gauche
    envoie_commande("RIGHT", 20)
            
    #envoie arret
    envoie_commande("STOP",0, 5.0)


    ser.close()
    print("fin du test")

    
    
except Exception as e:
    print(f"[ERREUR] : {e}")

    