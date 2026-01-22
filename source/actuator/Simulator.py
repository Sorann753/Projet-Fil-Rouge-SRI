import turtle
import os
import time
import tkinter

PATH = "SimulatorController.txt"
isRunning = True

# création du Robot
def init_bot():
    robot = turtle.Turtle()
    robot.color("orange")
    robot.shape("square")
    robot.speed(1)
    robot.width(5)
    return robot

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
        robot.down()

# fonction pour lire les nouvelles commandes
def ReadAction(robot, last_index):
    global isRunning
    if not os.path.exists(PATH):
        return last_index
    try:
        with open(PATH, "r") as f:
            lines = f.readlines()
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
    bot = init_bot()
    screen = turtle.Screen()
    #screen.tracer(0)  
    last_read_index = 0

    while isRunning:
        try:
            last_read_index = ReadAction(bot, last_read_index)
            screen.update()
        except turtle.Terminator:
            isRunning = False
        time.sleep(0.1)
