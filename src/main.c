#include <stdio.h>
#include <time.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#include "fighters.h"
#include "render.h"
#define FPS 30

/* ================== UTILITÁRIOS ================== */

int clamp(int v, int a, int b) {
    return v < a ? a : (v > b ? b : v);
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