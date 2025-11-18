#include "screen.h"
#include "fighters.h"

void drawFloor(void) {
    screenGotoxy(SCRSTARTX - 1, SCRENDY - 1);
    for (int i = 0; i < MAXX - 3; i++) {
        printf("=");
    }
}

void drawFighter(const Fighter *f, int y, int isPlayer) {
    screenGotoxy(f->x, y);

    if (f->attacking) {
        if (f->facing == 1) {
            // direita
            printf(isPlayer ? "O-<" : "O-<");
        } else {
            // esquerda
            printf(isPlayer ? ">-O" : ">-O");
        }
    } else {
        printf(" O ");
    }
}

void drawHealthBar(int x, int y, int hp) {
    int width = 20;
    if (hp < 0) hp = 0;
    if (hp > MAX_HP) hp = MAX_HP;

    int filled = (hp * width) / MAX_HP;

    screenGotoxy(x, y);
    printf("[");
    for (int i = 0; i < width; i++) {
        printf(i < filled ? "█" : " ");
    }
    printf("] %3d", hp);
}

void drawHUD(const Fighter *player, const Fighter *cpu) {
    drawHealthBar(SCRSTARTX + 1, SCRSTARTY + 2, player->hp);
    drawHealthBar(SCRENDX - 24, SCRSTARTY + 2, cpu->hp);

    drawFloor();

    screenGotoxy(SCRSTARTX + 1, SCRENDY);
    printf("[A/D] mover  [J] atacar  [Q] sair");
}

