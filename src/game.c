#include <time.h>

#include "game.h"
#include "lib/screen.h"
#include "lib/timer.h"
#include "fighters.h"
#include "render.h"
#include "utils.h"

#define ROUND_TIME 70;

void initGame(GameState *game) {
    // aqui você só inicializa os lutadores
    initFighter(&game->player, SCRSTARTX + 5,  1);
    initFighter(&game->cpu,    SCRENDX   - 5, -1);
    game-> timeLeft = ROUND_TIME;
}

void runFight(void) {
    GameState game;
    initGame(&game);

    int running  = 1;
    time_t startTime = time(NULL);

    while (running) {
        // INPUT
        handlePlayerInput(&running, &game.player);

        // limita posição do player dentro da arena
        game.player.x = clamp(game.player.x, SCRSTARTX + 1, SCRENDX - 1);

        // UPDATE (somente em tick de timer)
        if (timerTimeOver()) {
            
            time_t now = time(NULL);
            int elapsed = (int)(now - startTime);
            game.timeLeft = 70 - elapsed;
            
            updateCPU(&game.cpu, &game.player);

            // ataques (aplicam dano e atualizam timers)
            updateAttack(&game.player, &game.cpu);
            updateAttack(&game.cpu,    &game.player);

            // verifica fim de jogo
            if (game.player.hp <= 0 || game.cpu.hp <= 0 || game.timeLeft <= 0) {
                running = 0;
            }

            // DRAW
            drawGame(&game.player, &game.cpu, game.timeLeft);
        }
    }

    drawEndScreen(&game.player, &game.cpu);
}