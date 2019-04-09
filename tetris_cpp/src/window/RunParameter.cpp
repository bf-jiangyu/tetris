#include "RunParameter.h"

QString poolMap[POOL_H][POOL_W];
bool gameover = false;
bool OPEN_AUTODOWN = false;
int AUTO_DOWNBLOCK = 3 * 1000;
int AUTO_RISELINE = 4 * 1000;

int STAT_GAMESTART_TIME = 0;
int STAT_TOTALCOUNT = 0;
int STAT_GAMETIMES = 0;
int STAT_SUCCESSTIMES = 0;
