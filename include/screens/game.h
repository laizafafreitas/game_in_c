#ifndef GAME_H
#define GAME_H

#include "fighters.h"
#include "config.h"

typedef struct {
    Fighter player;
    Fighter cpu;

    int round;
    int playerWins;
    int cpuWins;
    int timeLeft;

    int totalTimeSeconds;   // acumula tempo gasto em todos os rounds
    int totalHpLost;        // soma de vida perdida em cada round
    int roundsPlayed;       // quantos rounds foram jogados

    int roundResult[MAX_ROUNDS]; // 1 = player, 2 = cpu, 0 = empate
} GameState;

void initGame(GameState *game);
void runFight(void);

#endif
