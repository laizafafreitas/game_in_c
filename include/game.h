#ifndef GAME_H
#define GAME_H

#include "fighters.h"

// Representa o estado básico da luta
typedef struct {
    Fighter player;
    Fighter cpu;

    int round;
    int playerWins;
    int cpuWins;
    int timeLeft;
} GameState;

// Inicializa o estado do jogo (lutadores, posições, etc.)
void initGame(GameState *game);

// Continua sendo a função que o main chama
void runFight(void);

#endif
