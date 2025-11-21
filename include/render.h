#ifndef RENDER_H
#define RENDER_H

#include "fighters.h"

void drawFloor(void);
void drawFighter(const Fighter *f, int y, int isPlayer);
void drawTimer(int x, int y, int timeLeft);
void drawHealthBar(int x, int y, int hp);
void drawHUD(const Fighter *player, const Fighter *cpu, int timeLeft);
void drawGame(const Fighter *player, const Fighter *cpu, int timeLeft);
void drawEndScreen(const Fighter *player, const Fighter *cpu);

#endif