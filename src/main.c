#include <stdio.h>
#include <time.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#include "fighters.h"
#define FPS 30

/* ================== UTILITÁRIOS ================== */

int clamp(int v, int a, int b) {
    return v < a ? a : (v > b ? b : v);
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

/* ================== RENDER ================== */

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

void drawHUD(const Fighter *player, const Fighter *cpu) {
    drawHealthBar(SCRSTARTX + 1, SCRSTARTY + 2, player->hp);
    drawHealthBar(SCRENDX - 24, SCRSTARTY + 2, cpu->hp);

    drawFloor();

    screenGotoxy(SCRSTARTX + 1, SCRENDY);
    printf("[A/D] mover  [J] atacar  [Q] sair");
}

void drawGame(const Fighter *player, const Fighter *cpu) {
    clearGameArea();
    drawHUD(player, cpu);

    // y dos lutadores (linha "do chão")
    int fighterY = SCRENDY - 2;

    drawFighter(player, fighterY, 1);
    drawFighter(cpu,    fighterY, 0);

    screenUpdate();
}

/* ================== TELA FINAL ================== */

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

/* ================== MAIN / GAME LOOP ================== */

int main(void) {
    srand((unsigned) time(NULL));

    screenInit(1);
    keyboardInit();
    timerInit(1000 / FPS);

    Fighter player;
    Fighter cpu;

    initFighter(&player, SCRSTARTX + 5,  1);
    initFighter(&cpu,    SCRENDX   - 5, -1);

    int running = 1;

    while (running) {
        // INPUT
        handlePlayerInput(&running, &player);

        // limita posição do player dentro da arena
        player.x = clamp(player.x, SCRSTARTX + 1, SCRENDX - 1);

        // UPDATE (somente em tick de timer)
        if (timerTimeOver()) {
            updateCPU(&cpu, &player);

            // ataques (aplicam dano e atualizam timers)
            updateAttack(&player, &cpu);
            updateAttack(&cpu,    &player);

            // verifica fim de jogo
            if (player.hp <= 0 || cpu.hp <= 0) {
                running = 0;
            }

            // DRAW
            drawGame(&player, &cpu);
        }
    }

    drawEndScreen(&player, &cpu);

    keyboardDestroy();
    screenDestroy();
    timerDestroy();
    return 0;
}