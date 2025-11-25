#ifndef RENDER_H
#define RENDER_H

#include "fighters.h"

void drawFloor(void);
void drawFighter(const Fighter *f, int y, int isPlayer);
void drawTimer(int x, int y, int timeLeft);
void drawHealthBar(int x, int y, int hp);
void drawHUD(const Fighter *player,
             const Fighter *cpu,
             int timeLeft,
             int playerWins,
             int cpuWins);
void drawGame(const Fighter *player,
              const Fighter *cpu,
              int timeLeft,
              int playerWins,
              int cpuWins);
void drawEndScreen(const Fighter *player, const Fighter *cpu);
void drawLogicQuizScreen(int timeLeft);

#endif