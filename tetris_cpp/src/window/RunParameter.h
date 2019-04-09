#ifndef RUNPARAMETER_H
#define RUNPARAMETER_H

#include "../gamepool/PoolConstant.h"
#include <QString>

extern QString poolMap[POOL_H][POOL_W];
extern bool gameover;
extern bool OPEN_AUTODOWN;
extern int AUTO_DOWNBLOCK;
extern int AUTO_RISELINE;

extern int STAT_GAMESTART_TIME;
extern int STAT_TOTALCOUNT;
extern int STAT_GAMETIMES;
extern int STAT_SUCCESSTIMES;

#endif
