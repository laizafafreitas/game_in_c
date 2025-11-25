#ifndef GAME_H
#define GAME_H

#include "fighters.h"

typedef struct {
    Fighter player;
    Fighter cpu;

    int round;
    int playerWins;
    int cpuWins;
    int timeLeft;
} GameState;

void initGame(GameState *game);
void runFight(void);

#endif
