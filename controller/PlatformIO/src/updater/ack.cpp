#include "updater/ack.hpp"
#include "updater/command.hpp"
#include "updater/state.hpp"
#include "components/ultrasons.hpp"
#include "constants.hpp"

extern int distAv, distG, distD;
extern Command currentCmd;

void ACKCmd(RobotState state)
{
    static unsigned long lastACK = 0;
    if (millis() - lastACK < 200) return;
    lastACK = millis();

    unsigned long dureeRestante = 0;
    if (state.state == MOVING && state.tempsFinAction > millis())
    {
        dureeRestante = state.tempsFinAction - millis();
    }

    Serial.print("TELE;");
    Serial.print(state.state);
    Serial.print(";");
    Serial.print(currentCmd.action);
    Serial.print(";");
    Serial.print(dureeRestante);
    Serial.print(";");
    Serial.print(distAv);
    Serial.print(";");
    Serial.print(distG);
    Serial.print(";");
    Serial.println(distD);
}