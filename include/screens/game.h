#ifndef GAME_H
#define GAME_H

#include "fighters.h"
#include "config.h"
typedef enum {
    MODE_VS_CPU,
    MODE_VS_PLAYER
} GameMode;

typedef struct {
    Fighter player;
    Fighter cpu;

    int round;
    int playerWins;
    int cpuWins;
    int timeLeft;

    int totalTimeSeconds;   // acumula tempo gasto em todos os rounds
    int totalHpLost;        // soma de vida perdida em cada round

    int roundsPlayed;       // quantos rounds realmente rolaram
    int roundResult[MAX_ROUNDS]; // 0 = empate, 1 = player, 2 = cpu/p2
} GameState;

void initGame(GameState *game);
void runFight(GameMode mode);

#endif
