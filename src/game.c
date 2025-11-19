#include "game.h"

#include "screen.h"
#include "timer.h"
#include "fighters.h"
#include "render.h"
#include "utils.h"

void runFight(void) {
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
}