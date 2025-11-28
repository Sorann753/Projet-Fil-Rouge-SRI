import turtle
import os

PATH = "Projet-Fil-Rouge-SRI/build/linux/x86_64/debug/SimulatorController.txt"

#creation du Robot
def init_bot():
    robot = turtle.Turtle()
    robot.color("orange")
    robot.shape("square")
    return robot
    


#la fonction de navigation    
def navigation(action, value, robot):
    
    #naviger en fonction de laction
    if action == "FORWARD":
        print(f"NAV_FORWARD: {value} metre")
        turtle.forward(value)

    elif action == "BACKWARD":
        print(f"NAV_BACKWARD {value} metre")
        turtle.forward(-value)

    elif action == "TURN":
        print(f"NAV_TURN: {value}°")
        turtle.left(value)

    else:
        print("NAV_ERROR: unknown action !")
        return




def ReadAction(robot):
    if not (os.path.exists(PATH)):
        print(f"FILE_SIM: {PATH} hase note been opend in Simulator.py")
        return
    
    #ouvrire et lire le fichier SimulatorController.txt
    with open(PATH, "r") as my_file:
        
        #on recupére toutes les ligne du fichier
        lines = my_file.readlines()

        #on parcours chaque ligne
        for line in lines:
            #on va retirer les espace et les retour a la ligne
            line = line.strip() 

            #on separe [action, valeur] dans un tablaux 
            commande = line.split(" ")

            #on test si on as bien action et valeur 
            if (len(commande) < 2):
                print(f"FILE_SIM : the line : {line} is not valide ")
                continue 
            
            #recuperer action et valeur
            action = commande[0]
            value = commande[1]

            #apelle de la fonction de navigation
            navigation(action, value, robot)



if __name__ == "__main__":
    bot = init_bot()
    ReadAction(bot)

    print("navigation reussi !")