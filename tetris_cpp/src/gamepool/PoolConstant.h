#ifndef POOLCONSTANT_H
#define POOLCONSTANT_H

#include <QString>

const int CELL_WIDTH = 22;

const int DIV_INTERVAL = CELL_WIDTH / 2;

const int HINTPOOL_W = CELL_WIDTH * 5;
const int HINTPOOL_H = CELL_WIDTH * 4;
const int HINTPOOL_T = HINTPOOL_H + DIV_INTERVAL;

const int POOL_W = 12;
const int POOL_H = 21;

const int CELLPOOL_W = CELL_WIDTH * POOL_W;
const int CELLPOOL_H = CELL_WIDTH * POOL_H;
const int CELLPOOL_L = HINTPOOL_W + DIV_INTERVAL;
const int CELLPOOL_T = HINTPOOL_H + DIV_INTERVAL;

const int GAMEPOOL_W = HINTPOOL_W + CELLPOOL_W + DIV_INTERVAL;
const int GAMEPOOL_HEIGHT = HINTPOOL_H + CELLPOOL_H + DIV_INTERVAL;

const QString IMG_PATH = "src/gamepool/block/";
#endif
