#ifndef FIGHTERS_H
#define FIGHTERS_H

#include <stdlib.h>

#define MAX_HP          100
#define ATTACK_RANGE    4
#define DAMAGE          10
#define MIN_DISTANCE    4
#define ATTACK_DURATION 6

typedef struct {
    int x;
    int hp;
    int facing;        // -1 = esquerda, 1 = direita
    int attacking;     // 0/1
    int attack_timer;  // frames restantes do ataque
} Fighter;

void initFighter(Fighter *f, int startX, int facing);
void startAttack(Fighter *f);
void updateAttack(Fighter *attacker, Fighter *defender);
void handlePlayerInput(int *running, Fighter *player);
void updateCPU(Fighter *cpu, Fighter *player);

#endif