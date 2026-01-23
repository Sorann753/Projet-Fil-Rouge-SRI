import turtle
import os
import time
import tkinter

PATH = "SimulatorController.txt"
CONFIG_PATH = "../../../../config/simulatorConfig.toml"
isRunning = True

# création du Robot
def init_bot():
    robot = turtle.Turtle()
    robot.color("orange")
    robot.shape("square")
    robot.speed(1)
    robot.width(5)
    return robot


def ReadScreenConfig(screen):
    # Valeurs par défaut si jamais le fichier n'existe pas
    width = 800
    height = 600
    bg_color = "white"
    
    if not os.path.exists(CONFIG_PATH):
        print("Simulator.py: ERREUR config could not been found (fenetre par default)\n")
    else:
        with open(CONFIG_PATH, "r") as f:
            for line in f:
                line = line.strip()
                
                # ignorer les lignes vides et les commentaires
                if not line or line.startswith("[") or line.startswith("#"):
                    continue
                
                # chercher les valeurs"
                if "=" in line:
                    key, value = line.split("=")
                    key = key.strip()
                    value = value.strip().strip('"')  # enlever espaces et guillemets
                    
                    if key == "width":
                        width = int(value)
                    elif key == "height":
                        height = int(value)
                    elif key == "background_color":
                        bg_color = value
    
    # Appliquer la configuration
    screen.setup(width=width, height=height)
    screen.bgcolor(bg_color)
    
    return width, height
    



# fonction de navigation
def navigation(action, value, robot, value2):
    value_float = float(value)
    if action == "FORWARD":
        robot.forward(value_float)
        time.sleep(1)
    elif action == "BACKWARD":
        robot.forward(-value_float)
        time.sleep(1)
    elif action == "TURN":
        robot.left(value_float)
        time.sleep(1)
    elif action == "INIT":
        x = value_float
        y = float(value2)
        robot.up()
        robot.goto(x, y)
        robot.clear()
        robot.down()
        

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
        new_lines = lines[last_index:]  # lire uniquement les lignes nouvelles
        for line in new_lines:
            line = line.strip()
            commande = line.split(" ")
            if len(commande) < 2:
                continue
            action = commande[0]
            value = commande[1]
            value2 = commande[2] if action == "INIT" and len(commande) > 2 else 0
            
            if action == "CLOSE":
                isRunning = False
                return len(lines) 
            
            elif action in ["FORWARD", "BACKWARD", "TURN", "INIT"]:
                navigation(action, value, robot, value2)
        return len(lines)  # mettre à jour l'index de dernière ligne lue
    except (turtle.Terminator, tkinter.TclError):
        isRunning = False
        return last_index

if __name__ == "__main__":

    # configuration de la fentre
    screen = turtle.Screen()
    window_size = ReadScreenConfig(screen)

    robot = init_bot()
    turtle.hideturtle()

    #screen.tracer(0)  
    last_read_index = 0

    while isRunning:
        try:
            last_read_index = ReadAction(robot, last_read_index)
            screen.update()
        except turtle.Terminator:
            isRunning = False
        time.sleep(0.1)
