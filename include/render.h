#ifndef RENDER_H
#define RENDER_H

#include "fighters.h"

void drawFloor(void);
void drawFighter(const Fighter *f, int y, int isPlayer);
void drawHealthBar(int x, int y, int hp);
void drawHUD(const Fighter *player, const Fighter *cpu);
#endif