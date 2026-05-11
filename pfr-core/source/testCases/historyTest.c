#include "history/history.h"
#include "testCases/historyTest.h"

int testHistory(HistoryLevel level, const char* message)
{
    printf("----- TEST LOG HISTORIQUE -----\n");
    history_init();
    history_log(level,message);
    history_close();
    printf("--- FIN TEST LOG HISTORIQUE ---\n");

    return 0;
}