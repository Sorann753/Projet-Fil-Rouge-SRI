import turtle
import os
import time
import tkinter

PATH = "SimulatorController.txt"
CONFIG_PATH = "../../../../config/simulatorConfig.toml"
isRunning = True

# configuration des coordonnée de la balle
BALL_X = 100
BALL_Y = 100
TOUCHED = False

# creation du Robot
def init_bot():
    robot = turtle.Turtle()
    robot.color("orange")
    robot.shape("square")
    robot.speed(1)
    robot.width(5)
    return robot

#fonction pour dessiner la balle
def draw_target():
    target = turtle.Turtle()
    target.hideturtle() # On cache le curseur pour juste voir le dessin
    target.color("red")
    target.penup()

    # On se deplace vers les coordonnée de lbstacle
    target.goto(BALL_X, BALL_Y - 20) 
    target.begin_fill()
    target.circle(20)
    target.end_fill()

#fonction de détection de collision
def check_collision(robot):
    global TOUCHED
    
    # si on a déjà toucher alors on ne fait rien
    if TOUCHED: return

    # on calcule la distance (fonction de turtle)
    dist = robot.distance(BALL_X, BALL_Y)
    
    # si distance < rayon
    if dist < (30): 
        print(f"\nBalle trouver : ({BALL_X}, {BALL_Y})\n")
        
        # On écrit sur l'écran
        writer = turtle.Turtle()
        writer.hideturtle()
        writer.color("red")
        writer.penup()
        writer.goto(0, 0) # Au centre de l'écran
        writer.write("bravo ! BALLE TROUVER!", align="center", font=("Arial", 40, "bold"))
        
        TOUCHED = True #mettre a jour trouver

def ReadScreenConfig(screen):
    #valeur par default su screensize
    width = 800
    height = 600
    bg_color = "white"
    
    #ouverture du fichier pour lire la taille du screen
    if not os.path.exists(CONFIG_PATH):
        print("Simulator.py: ERREUR config could not been found (fenetre par default)\n")
    else:
        with open(CONFIG_PATH, "r") as f:
            for line in f:
                line = line.strip()
                if not line or line.startswith("[") or line.startswith("#"): continue
                if "=" in line:
                    key, value = line.split("=")
                    key = key.strip()
                    value = value.strip().strip('"')
                    if key == "width": width = int(value)
                    elif key == "height": height = int(value)
                    elif key == "background_color": bg_color = value


    #modifier la taille de la fenetre
    screen.setup(width=width, height=height)

    #modifier la couleur du bg de la fenetre
    screen.bgcolor(bg_color)

    return width, height

# fonction de navigation
def navigation(action, value, robot, value2):
    value_float = float(value)
    
    #si lit FORWARD apelle la fonction forward de turtle
    if action == "FORWARD":
        robot.forward(value_float)

        #on verifie la collision
        check_collision(robot) 
        time.sleep(1)
        
    elif action == "BACKWARD":
        robot.forward(-value_float) # Astuce : backward marche aussi
        
        #on verifie la collision
        check_collision(robot)
        time.sleep(1)
        
    elif action == "TURN":
        robot.left(value_float)
        time.sleep(1)
        
    #si il lit INIT alors il initialise la position du robot
    elif action == "INIT":
        x = value_float
        y = float(value2)
        robot.up()
        robot.goto(x, y)
        robot.clear()
        robot.down()

        # on fait lactualisation de la balle
        global TOUCHED
        TOUCHED = False 

# fonction pour lire les nouvelles commandes
def ReadAction(robot, last_index):
    global isRunning
    if not os.path.exists(PATH):
        return last_index
    try:
        with open(PATH, "r") as f:
            lines = f.readlines()
        if last_index > len(lines):
            last_index = 0
        new_lines = lines[last_index:]
        for line in new_lines:
            line = line.strip()
            commande = line.split(" ")
            if len(commande) < 2: continue
            action = commande[0]
            value = commande[1]
            value2 = commande[2] if action == "INIT" and len(commande) > 2 else 0
            
            if action == "CLOSE":
                isRunning = False
                return len(lines) 
            elif action in ["FORWARD", "BACKWARD", "TURN", "INIT"]:
                navigation(action, value, robot, value2)
        return len(lines)
    except (turtle.Terminator, tkinter.TclError):
        isRunning = False
        return last_index

if __name__ == "__main__":

    screen = turtle.Screen()
    window_size = ReadScreenConfig(screen)
    
    # On dessine la balle rouge
    draw_target()

    #on creer le robot
    robot = init_bot()
    turtle.hideturtle() # Cache le curseur par défaut


    last_read = 0

    while isRunning:
        try:

            last_read = ReadAction(robot, last_read)
            screen.update()
        except turtle.Terminator:
            isRunning = False
        time.sleep(0.1)