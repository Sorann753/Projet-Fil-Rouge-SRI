#include "constants.hpp"
#include "updater/state.hpp"
#include "components/ultrasons.hpp"
#include "updater/command.hpp"
#include "components/motor.hpp"

/**
 * @brief calcule du temp de marche des moteur en fonction de la distance souhaité
 * @param speedratio (la vitesse CM/sec ou DEG/sec)
 */
static unsigned long CalculTempFinAction(int valeur, float Speedratio, unsigned long currentTime)
{
    // temp = (distace demander / constante vitesse) * 1000
    return currentTime + (valeur / Speedratio) * 1000.0;
}

/**
 * @brief afficher les log de transition
 * @param avant (état actuelle)
 * @param apres (état suivant)
 */
void logStateChange(State avant, State apres)
{
    if (avant == apres)
    {
        return;
    }
    // affichage des log sur le serial1
    Serial1.print("CHANGEMENT D'ETAT");
    Serial1.print(avant);
    Serial1.print(" => ");
    Serial1.println(apres);
}

// prototype pour que traitementCommandebuffer puisse lutiliser
static RobotState IDLETransition(RobotState currentState, Command cmd);

/**
 * @brief
 */
RobotState traitmentCommandebuffer(RobotState currentState)
{
    if (indexRead == indexWrite) // buffer vide
        return currentState;

    Command cmd = listCmd[indexRead];
    listCmd[indexRead].active = false;
    indexRead = (indexRead + 1) % 10;

    if (currentState.state == EMERGENCY)
        return currentState;

    currentCmd = cmd;
    currentCmd.active = true;

    RobotState nextState = IDLETransition(currentState, cmd);
    logStateChange(currentState.state, nextState.state);
    return nextState;
}

/**
 * @brief pour la perte de la conexion avec la Raspbery
 * @param dernierMessagePi ()
 */
RobotState watchdogVerification(RobotState currentState, unsigned long dernierMessagePi)
{
    if (currentState.state == MOVING && (millis() - dernierMessagePi > timeoutPi))
    {
        Serial1.println("Watchdog !! connexion RBI perdu");

        RobotState nextState = currentState;
        logStateChange(nextState.state, IDLE);
        nextState.state = IDLE;
        nextState.tempsFinAction = 0;
        return nextState;
    }
    return currentState;
}

//---FONCTION DE TRANSITION---
/**
 * @brief trasition vers etat IDLE
 * si action de la commande est "STOP" on reste sur IDLE
 * si action de la commmande est "forward" ou "backward" on passe a etat MOVING (et on set la vitesse des roue)
 */
RobotState IDLETransition(const RobotState currentState, const Command currentCmd)
{
    // currentState (contien le letat et le temp)
    RobotState nextState = currentState;
    nextState.tempsFinAction = 0;

    if (currentCmd.action == "STOP")
    {
        nextState.state = IDLE;
        return nextState;
    }

    // Speedratio en CM/sec ou DEG/sec en fonction si on avant/recule ou tourne
    float ratio = (currentCmd.action == "FORWARD" || currentCmd.action == "BACKWARD") ? CM_PAR_SECONDE : DEG_PAR_SECONDE;
    if (currentCmd.action == "FORWARD" || currentCmd.action == "BACKWARD")
        setspeedroue(FORWARD_SPEED);
    else
        setspeedroue(TURN_SPEED);

    nextState.state = MOVING;

    nextState.tempsFinAction = CalculTempFinAction(currentCmd.valeur, ratio, millis());
    return nextState;
}

/**
 * @brief transition vers etat moving et affichage etat actuelle et suivant
 */
RobotState MOVINGTransition(RobotState currentState)
{
    RobotState nextState = currentState;
    if (millis() >= currentState.tempsFinAction)
    {
        nextState.state = IDLE;
        nextState.tempsFinAction = 0;
        currentCmd = {"", 0, false}; // reinitialisation de la structure commande (action="" | valeur=0 | active = false)
        Serial1.println("CHANGEMENT D'ETAT => (IDLE)");
    }
    return nextState;
}

//---CHANGMENT D'ETAT---
/**
 * @brief changement d'état
 */
RobotState updateState(RobotState currentState, int distAv, unsigned long dernierMessagePi)
{
    RobotState nextState = currentState;

    // detection dobstacle => EMERGENCY
    if (distAv <= 20)
    {
        logStateChange(currentState.state, EMERGENCY);
        nextState.state = EMERGENCY;
        nextState.tempsFinAction = 0;
        return nextState;
    }

    // EMERGENCY et plus dobstacle => IDLE
    if (currentState.state == EMERGENCY && distAv > 40)
    {
        logStateChange(nextState.state, IDLE);
        nextState.state = IDLE;
        return nextState;
    }

    // watchdog pour la perte de la conexion avec la Raspbery
    nextState = watchdogVerification(currentState, dernierMessagePi);

    if (nextState.state == MOVING && indexRead != indexWrite)
    {
        Command peek = listCmd[indexRead];
        if (peek.action == "STOP")
        {
            indexRead = (indexRead + 1) % 10;
            currentCmd = {"", 0, false}; // reinitialisation de la structure commande (action="" | valeur=0 | active = false)

            logStateChange(nextState.state, IDLE);
            nextState.state = IDLE;
            nextState.tempsFinAction = 0;
            return nextState;
        }
    }

    if (nextState.state == IDLE)
        nextState = traitmentCommandebuffer(nextState);

    // Vérifier timer si MOVING
    if (nextState.state == MOVING)
    {
        RobotState apres = MOVINGTransition(nextState);
        logStateChange(nextState.state, apres.state);
        nextState = apres;
    }

    return nextState;
}