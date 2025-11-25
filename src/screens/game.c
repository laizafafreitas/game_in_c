#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "lib/screen.h"
#include "lib/timer.h"
#include "lib/keyboard.h"

#include "config.h"
#include "screens/quiz.h"
#include "screens/game.h"
#include "fighters.h"
#include "render.h"
#include "utils.h"
#include "sound.h"

#define MAX_SCORE          1000
#define SCORE_HP_PENALTY   5   // pontos perdidos por 1 HP perdido
#define SCORE_TIME_PENALTY 2   // pontos perdidos por 1 segundo de luta

static int   calculateScore(const GameState *game);
static char *askPlayerName(void);

int playerDamage = DAMAGE;
int playerBuffActive = 0;

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

    // acumuladores pro score
    game->totalTimeSeconds = 0;
    game->totalHpLost      = 0;
    game->roundsPlayed     = 0;   
    
    // resultados de cada round (ainda não jogados)
    for (int i = 0; i < MAX_ROUNDS; i++)
        game->roundResult[i] = -1;   // -1 = não houve round ainda

    playerDamage = DAMAGE;
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
            updateAttack(&game->player, &game->cpu, playerDamage);
            updateAttack(&game->cpu,    &game->player, DAMAGE);

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

    // --- ACUMULA TEMPO E VIDA PERDIDA PARA O SCORE ---

    // tempo gasto neste round
    int timeSpent = ROUND_TIME - game->timeLeft;
    if (timeSpent < 0) timeSpent = 0;
    game->totalTimeSeconds += timeSpent;

    // vida perdida neste round (considerando HP máximo)
    int hpLostThisRound = MAX_HP - game->player.hp;
    if (hpLostThisRound < 0) hpLostThisRound = 0;
    game->totalHpLost += hpLostThisRound;

    // Decide quem venceu o round (ou se empatou)
    int result = 0; // 0 = empate

    if (game->player.hp > game->cpu.hp)
    {
        game->playerWins++;
        result = 1; // player
    }
    else if (game->cpu.hp > game->player.hp)
    {
        game->cpuWins++;
        result = 2; // cpu
    }

    // guarda o resultado desse round no array
    int idx = game->round - 1; // round começa em 1 no runFight
    if (idx >= 0 && idx < MAX_ROUNDS)
    {
        game->roundResult[idx] = result;
    }
}

// -----------------------------------------------------------------------------
// Função para calcular o score final
// -----------------------------------------------------------------------------
static int calculateScore(const GameState *game)
{
    int score = MAX_SCORE;

    // penaliza pela vida perdida
    score -= game->totalHpLost * SCORE_HP_PENALTY;

    // penaliza pelo tempo total
    score -= game->totalTimeSeconds * SCORE_TIME_PENALTY;

    if (score < 0) score = 0;
    return score;
}


// -----------------------------------------------------------------------------
// Ler o nome do jogador com alocação dinâmica
// -----------------------------------------------------------------------------
static char *askPlayerName(void)
{
    clearGameArea();
    screenSetColor(WHITE, LIGHTCYAN);

    int midX = (SCRSTARTX + SCRENDX) / 2 - 10;
    int midY = (SCRSTARTY + SCRENDY) / 2;

    screenGotoxy(midX, midY - 1);
    printf("Partida encerrada!");

    screenGotoxy(midX, midY + 0);
    printf("Digite seu nome:");

    screenSetColor(YELLOW, LIGHTCYAN);

    // buffer de entrada
    char buffer[64];
    int len = 0;
    buffer[0] = '\0';

    int running = 1;

    screenUpdate();

    while (running)
    {
        if (keyhit())
        {
            int k = readch();

            // ENTER → finaliza
            if (k == '\n' || k == '\r')
                running = 0;

            // BACKSPACE
            else if (k == 8 || k == 127)
            {
                if (len > 0)
                {
                    len--;
                    buffer[len] = '\0';
                }
            }

            // caracteres imprimíveis
            else if (k >= 32 && k <= 126)
            {
                if (len < (int)sizeof(buffer) - 1)
                {
                    buffer[len++] = (char)k;
                    buffer[len] = '\0';
                }
            }

            // redesenha a linha de entrada
            screenSetColor(CYAN, LIGHTCYAN);
            screenGotoxy(midX, midY + 2);
            printf("Nome: %-60s", buffer); // limpa restos
            screenUpdate();
        }
    }

    // se não digitou nada, usa "Player"
    if (len == 0)
    {
        char *fallback = malloc(7);
        strcpy(fallback, "Player");
        return fallback;
    }

    // alocar dinamicamente o nome final
    char *name = malloc(len + 1);
    strcpy(name, buffer);
    return name;
}


// -----------------------------------------------------------------------------
// Função chamada pelo main: controla a sequência de rounds + quiz + tela final
// -----------------------------------------------------------------------------
void runFight(void)
{
    soundStopMusic();
    soundPlayFightMusic();

    GameState game;
    initGame(&game);

    for (game.round = 1;
         game.round <= MAX_ROUNDS && game.playerWins < 2 && game.cpuWins < 2;
         game.round++)
    {
        resetRound(&game);
        playRound(&game);

        game.roundsPlayed++;

        // quiz entre os rounds
        if (game.cpuWins == 1 && game.playerWins == 0)
        {
            int acertou = runLogicQuiz();

            if (acertou) {
                playerDamage     = 15;
                playerBuffActive = 1;
            }
            soundStopMusic();      
        soundPlayFightMusic();  
        }
    }

    // quem ganhou a melhor de 3?
    int playerWonMatch = (game.playerWins > game.cpuWins);

        drawEndScreen(&game.player, &game.cpu);

        int finalScore   = calculateScore(&game);
        char *playerName = askPlayerName();

        // vida máxima possível e vida "mantida"
        int maxHpTotal = MAX_HP * game.roundsPlayed;
        if (maxHpTotal <= 0) maxHpTotal = MAX_HP;

        int hpTotalFinal = maxHpTotal - game.totalHpLost;
        if (hpTotalFinal < 0) hpTotalFinal = 0;

        soundStopMusic();
        soundPlayScoreMusic();

        drawScoreScreen(finalScore,
                        playerName,
                        playerWonMatch,
                        maxHpTotal,
                        hpTotalFinal,
                        game.roundsPlayed,
                        game.roundResult);

        free(playerName);
}
