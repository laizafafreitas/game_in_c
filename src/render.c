#include "screen.h"
#include "fighters.h"
#include "keyboard.h"

void drawFloor(void) {
    screenGotoxy(SCRSTARTX - 1, SCRENDY - 1);
    for (int i = 0; i < MAXX - 3; i++) {
        printf("=");
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

void drawTimer(int x, int y, int timeLeft) {
    screenGotoxy(x, y);
    printf("TIME: %02d", timeLeft);  // imprime como 70, 08, 07, etc
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

void drawHUD(const Fighter *player, const Fighter *cpu, int timeLeft) {
    drawHealthBar(SCRSTARTX + 1, SCRSTARTY + 2, player->hp);
    drawHealthBar(SCRENDX - 24, SCRSTARTY + 2, cpu->hp);

    // Timer no centro da parte de cima
    int centerX = (SCRSTARTX + SCRENDX) / 2 - 4;
    drawTimer(centerX, SCRSTARTY + 1, timeLeft);

    drawFloor();

    screenGotoxy(SCRSTARTX + 1, SCRENDY);
    printf("[A/D] mover  [J] atacar  [Q] sair");
}

void drawGame(const Fighter *player, const Fighter *cpu, int timeLeft) {
    clearGameArea();
    drawHUD(player, cpu, timeLeft);

    // y dos lutadores (linha "do chão")
    int fighterY = SCRENDY - 2;

    drawFighter(player, fighterY, 1);
    drawFighter(cpu,    fighterY, 0);

    screenUpdate();
}

void drawEndScreen(const Fighter *player, const Fighter *cpu) {
    clearGameArea();

    screenGotoxy(MAXX / 2 - 6, MAXY / 2);
    if (player->hp > 0 && cpu->hp <= 0) {
        printf("VOCE VENCEU!");
    } else if (cpu->hp > 0 && player->hp <= 0) {
        printf("VOCE PERDEU!");
    } else {
        printf("EMPATE!");
    }

    screenGotoxy(MAXX / 2 - 12, MAXY / 2 + 2);
    printf("Pressione qualquer tecla...");

    screenUpdate();

    while (!keyhit()) { /* espera */ }
    (void)readch();
}
