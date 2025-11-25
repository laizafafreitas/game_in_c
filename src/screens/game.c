#include <time.h>

#include "lib/screen.h"
#include "lib/timer.h"
#include "lib/keyboard.h"

#include "config.h"
#include "screens/quiz.h"
#include "screens/game.h"
#include "fighters.h"
#include "render.h"
#include "utils.h"

// --- PROTÓTIPOS (internos a este arquivo) ---
static void resetRound(GameState *game);
static void playRound(GameState *game);

// -----------------------------------------------------------------------------
// Inicializa o estado base do jogo (lutadores e contadores de round/vitórias)
// -----------------------------------------------------------------------------
void initGame(GameState *game)
{
    // posição inicial dos lutadores na arena
    initFighter(&game->player, SCRSTARTX + 5,  1);   // 1 = direita
    initFighter(&game->cpu,    SCRENDX   - 5, -1);   // -1 = esquerda

    game->round      = 1;
    game->playerWins = 0;
    game->cpuWins    = 0;
    game->timeLeft   = ROUND_TIME;
}

// -----------------------------------------------------------------------------
// Reseta os dados de UM round (vidas, posições, tempo)
// -----------------------------------------------------------------------------
static void resetRound(GameState *game)
{
    // Reposiciona e reseta ataques/HP
    initFighter(&game->player, SCRSTARTX + 5,  1);
    initFighter(&game->cpu,    SCRENDX   - 5, -1);

    game->player.hp = MAX_HP;
    game->cpu.hp    = MAX_HP;
    game->timeLeft  = ROUND_TIME;
}

// -----------------------------------------------------------------------------
// Loop principal de UM round de luta
// -----------------------------------------------------------------------------
static void playRound(GameState *game)
{
    int running      = 1;
    int frameCounter = 0;   // conta quantos frames já passaram (pra virar segundos)

    while (running &&
           game->player.hp > 0 &&
           game->cpu.hp    > 0 &&
           game->timeLeft  > 0)
    {
        // INPUT (sempre pode ler input)
        handlePlayerInput(&running, &game->player);

        // limita posição do player dentro da arena
        game->player.x = clamp(game->player.x, SCRSTARTX + 1, SCRENDX - 1);

        // UPDATE + RENDER só quando o timer "vira" (≈ 1 / FPS segundos)
        if (timerTimeOver())
        {
            // lógica da CPU
            updateCPU(&game->cpu, &game->player);

            // ataques (aplicam dano e atualizam timers)
            updateAttack(&game->player, &game->cpu);
            updateAttack(&game->cpu,    &game->player);

            // conta frames para transformar em segundos de round
            frameCounter++;
            if (frameCounter >= FPS)
            {
                game->timeLeft--;
                frameCounter = 0;
            }

            // RENDER (arena + HUD + lutadores)
            drawGame(&game->player,
                     &game->cpu,
                     game->timeLeft,
                     game->playerWins,
                     game->cpuWins);
        }
    }

    // Se o jogador saiu do round (ESC, etc.), só retorna
    if (!running) return;

    // Decide quem venceu o round (ou se empatou)
    if (game->player.hp > game->cpu.hp)
    {
        game->playerWins++;
    }
    else if (game->cpu.hp > game->player.hp)
    {
        game->cpuWins++;
    }
    // empate: ninguém ganha ponto
}

// -----------------------------------------------------------------------------
// Função chamada pelo main: controla a sequência de rounds + quiz + tela final
// -----------------------------------------------------------------------------
void runFight(void)
{
    GameState game;
    initGame(&game);

    for (game.round = 1; game.round <= MAX_ROUNDS && game.playerWins < 2 && game.cpuWins < 2; game.round++)
    {
        resetRound(&game);
        playRound(&game);

        // entre os rounds (exceto depois do último), roda o quiz
        if (game.cpuWins == 1 && game.playerWins == 0)
        {
            runLogicQuiz();
        }
    }

    // ao final de todos os rounds, mostra a tela de fim de jogo
    drawEndScreen(&game.player, &game.cpu);
}