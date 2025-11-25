#include <time.h>

#include "game.h"
#include "lib/screen.h"
#include "lib/timer.h"
#include "lib/keyboard.h"  
#include "fighters.h"
#include "render.h"
#include "utils.h"

#define ROUND_TIME 70
#define QUIZ_TIME  30
#define MAX_ROUNDS 2

// --- PROTÓTIPOS ---
void resetRound(GameState *game);
void playRound(GameState *game);
void runLogicQuiz(void);

// --- INIT DO JOGO ---
void initGame(GameState *game) {
    game->round      = 1;
    game->playerWins = 0;
    game->cpuWins    = 0;
    resetRound(game);
}

// --- RESET DO ROUND ---
void resetRound(GameState *game) {
    initFighter(&game->player, SCRSTARTX + 5,  1);
    initFighter(&game->cpu,    SCRENDX   - 5, -1);
    game->timeLeft = ROUND_TIME;
}

// --- 1 ROUND DE LUTA ---
void playRound(GameState *game) {
    int running = 1;
    time_t startTime = time(NULL);

    while (running) {
        handlePlayerInput(&running, &game->player);

        game->player.x = clamp(game->player.x, SCRSTARTX + 1, SCRENDX - 1);

        if (timerTimeOver()) {
            time_t now   = time(NULL);
            int elapsed  = (int)(now - startTime);
            game->timeLeft = ROUND_TIME - elapsed;

            updateCPU(&game->cpu, &game->player);
            updateAttack(&game->player, &game->cpu);
            updateAttack(&game->cpu,    &game->player);

            if (game->player.hp <= 0 || game->cpu.hp <= 0 || game->timeLeft <= 0) {
                running = 0;
            }

            drawGame(&game->player, &game->cpu, game->timeLeft);
        }
    }

    // Decide o vencedor do round
    if (game->player.hp > game->cpu.hp) game->playerWins++;
    else if (game->cpu.hp > game->player.hp) game->cpuWins++;
}

// --- QUIZ ENTRE OS ROUNDS ---
void runLogicQuiz(void) {
    int remaining = QUIZ_TIME;
    time_t startTime = time(NULL);

    while (keyhit()) readch();

    while (1) {
        if (timerTimeOver()) {
            time_t now  = time(NULL);
            int elapsed = now - startTime;
            remaining   = QUIZ_TIME - elapsed;

            if (remaining <= 0) break;

            drawLogicQuizScreen(remaining);
        }

        if (keyhit()) {
            int ch = readch();

            if (ch == '0' || ch == '1' || ch == '2') {
                break;
            }
        }
    }
}

// --- LOOP PRINCIPAL DO JOGO ---
void runFight(void) {
    GameState game;
    initGame(&game);

    for (game.round = 1; game.round <= MAX_ROUNDS; game.round++) {

        resetRound(&game);
        playRound(&game);

        if (game.round < MAX_ROUNDS) {
            runLogicQuiz();
        }
    }

    drawEndScreen(&game.player, &game.cpu);
}