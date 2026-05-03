import os
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
            
            try:
                msg_debug = ser.readline().decode('utf-8').strip()
                if msg_debug: 
                    print(f"[ARDUINO] {msg_debug}")
            except Exception as e_read:
                print(f"[WARN] Erreur lecture : {e_read}")
                
        else:
            time.sleep(0.02)
    
#test du port arduino
if not os.path.exists(PORT):
    print(f"[ERREUR] Port {PORT} introuvable. Lance 'ls /dev/ttyACM*' pour vérifier")
    exit(1)
print(f"[LOG] Port {PORT} détecté")

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
    envoie_commande("BACKWARD", 65)
    

    #envoie de la commande tourner gauche
    envoie_commande("LEFT",45)
    

    #envoie de la commande tourner gauche
    envoie_commande("RIGHT", 20)

    #envoie de la commande reculer
    envoie_commande("BACKWARD", 20)

    #envoie arret
    envoie_commande("STOP",0, 5.0)

    #envoie de commande avance
    envoie_commande("FORWARD", 70)

    ser.close()
    print("fin du test")

    
    
except serial.SerialException as e:
    print(f"[ERREUR SERIAL] Problème de connexion : {e}")
except Exception as e:
    print(f"[ERREUR] : {type(e).__name__} : {e}")
finally:
    if 'ser' in dir() and ser.is_open:
        ser.close()
        print("[LOG] Port fermé proprement")