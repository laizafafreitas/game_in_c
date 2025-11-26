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

#define MAX_SCORE 1000
#define SCORE_HP_PENALTY 5   // pontos perdidos por 1 HP perdido
#define SCORE_TIME_PENALTY 2 // pontos perdidos por 1 segundo de luta

static int calculateScore(const GameState *game);
static char *askPlayerName(const char *title, const char *label);

int playerDamage = DAMAGE;
int playerBuffActive = 0;

// --- PROTÓTIPOS (internos a este arquivo) ---
static void resetRound(GameState *game);
static void playRound(GameState *game, GameMode mode);
static void handleBothPlayersInput(GameState *game, int *running);

// -----------------------------------------------------------------------------
// Inicializa o estado base do jogo (lutadores e contadores de round/vitórias)
// -----------------------------------------------------------------------------
void initGame(GameState *game)
{
    // posição inicial dos lutadores na arena
    initFighter(&game->player, SCRSTARTX + 5, 1); // 1 = direita
    initFighter(&game->cpu, SCRENDX - 5, -1);     // -1 = esquerda

    game->round = 1;
    game->playerWins = 0;
    game->cpuWins = 0;
    game->timeLeft = ROUND_TIME;

    // acumuladores pro score
    game->totalTimeSeconds = 0;
    game->totalHpLost = 0;
    game->roundsPlayed = 0;

    // resultados de cada round (ainda não jogados)
    for (int i = 0; i < MAX_ROUNDS; i++)
        game->roundResult[i] = -1; // -1 = não houve round ainda

    playerDamage = DAMAGE;
}

// -----------------------------------------------------------------------------
// Reseta os dados de UM round (vidas, posições, tempo)
// -----------------------------------------------------------------------------
static void resetRound(GameState *game)
{
    // Reposiciona e reseta ataques/HP
    initFighter(&game->player, SCRSTARTX + 5, 1);
    initFighter(&game->cpu, SCRENDX - 5, -1);

    game->player.hp = MAX_HP;
    game->cpu.hp = MAX_HP;
    game->timeLeft = ROUND_TIME;
}

// -----------------------------------------------------------------------------
// Função auxiliar para ler input dos DOIS players
// -----------------------------------------------------------------------------
// Lê um input e aplica em P1 ou P2 (modo multiplayer)
static void handleBothPlayersInput(GameState *game, int *running)
{
    if (!keyhit()) return;

    int k = readch();

    Fighter *p1 = &game->player;
    Fighter *p2 = &game->cpu;

    // Mesma tecla pra sair
    if (k == 'q' || k == 'Q') {
        *running = 0;
        return;
    }

    switch (k)
    {
        // ---- CONTROLES PLAYER 1 ----
        case 'a':
        case 'A':
            p1->x--;
            p1->facing = FACING_LEFT;
            break;

        case 'd':
        case 'D':
            p1->x++;
            p1->facing = FACING_RIGHT;
            break;

        case 'f':
        case 'F':
            startAttack(p1);
            break;

        // ---- CONTROLES PLAYER 2 ----
        case 'j':
        case 'J':
            p2->x--;
            p2->facing = FACING_LEFT;
            break;

        case 'k':
        case 'K':
            p2->x++;
            p2->facing = FACING_RIGHT;
            break;

        case 'p':
        case 'P':
            startAttack(p2);
            break;

        default:
            break;
    }

    // Garante que ninguém saia da arena
    p1->x = clamp(p1->x, SCRSTARTX + 1, SCRENDX - 1);
    p2->x = clamp(p2->x, SCRSTARTX + 1, SCRENDX - 1);
}

// -----------------------------------------------------------------------------
// Loop principal de UM round de luta
// -----------------------------------------------------------------------------
static void playRound(GameState *game, GameMode mode)
{
    int running      = 1;
    int frameCounter = 0;

    while (running &&
           game->player.hp > 0 &&
           game->cpu.hp    > 0 &&
           game->timeLeft  > 0)
    {
        // INPUT
        if (mode == MODE_VS_CPU)
        {
            // Só P1 é humano, P2 é CPU
            handlePlayerInput(&running, &game->player, &game->cpu);

            // Limita só o player 1 aqui
            game->player.x = clamp(game->player.x, SCRSTARTX + 1, SCRENDX - 1);
        }
        else
        {
            // Dois jogadores humanos no mesmo teclado
            handleBothPlayersInput(game, &running);
        }

        // UPDATE + RENDER
        if (timerTimeOver())
        {
            // Só usa IA se for vs CPU
            if (mode == MODE_VS_CPU)
            {
                updateCPU(&game->cpu, &game->player);
            }

            // ataques (funcionam igual pros dois modos)
            updateAttack(&game->player, &game->cpu, playerDamage);
            updateAttack(&game->cpu,    &game->player, DAMAGE);

            // FPS → decrementa timer do round
            frameCounter++;
            if (frameCounter >= FPS)
            {
                game->timeLeft--;
                frameCounter = 0;
            }

            // desenha tudo
            drawGame(&game->player,
                     &game->cpu,
                     game->timeLeft,
                     game->playerWins,
                     game->cpuWins,
                     mode);
        }
    }

    // Se o jogador saiu do round (ESC, etc.), só retorna
    if (!running)
        return;

    // --- ACUMULA TEMPO E VIDA PERDIDA PARA O SCORE ---

    // tempo gasto neste round
    int timeSpent = ROUND_TIME - game->timeLeft;
    if (timeSpent < 0)
        timeSpent = 0;
    game->totalTimeSeconds += timeSpent;

    // vida perdida neste round (considerando HP máximo)
    int hpLostThisRound = MAX_HP - game->player.hp;
    if (hpLostThisRound < 0)
        hpLostThisRound = 0;
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

    if (score < 0)
        score = 0;
    return score;
}

// -----------------------------------------------------------------------------
// Ler o nome do jogador com alocação dinâmica
// -----------------------------------------------------------------------------
static char *askPlayerName(const char *title, const char *label)
{
    clearGameArea();
    screenSetColor(WHITE, LIGHTCYAN);

    int midX = (SCRSTARTX + SCRENDX) / 2 - 10;
    int midY = (SCRSTARTY + SCRENDY) / 2;

    // linha de título (ex: "Partida encerrada!" ou "Modo Multiplayer")
    screenGotoxy(midX, midY - 1);
    printf("%s", title ? title : "Partida encerrada!");

    // linha de instrução (ex: "Digite seu nome:" / "Nome do Jogador 1:")
    screenGotoxy(midX, midY + 0);
    printf("%s", label ? label : "Digite seu nome:");

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
void runFight(GameMode mode)
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
        playRound(&game, mode);

        game.roundsPlayed++;

        // ===========================
        // QUIZ ENTRE ROUNDS
        // ===========================
        if (mode == MODE_VS_CPU)
        {
            // LÓGICA ANTIGA: só quando o bot ganha o 1º round
            if (game.cpuWins == 1 && game.playerWins == 0)
            {
                int acertou = runLogicQuiz();

                if (acertou)
                {
                    // buff só pro player contra CPU
                    playerDamage = 15;
                    playerBuffActive = 1;
                }

                soundStopMusic();
                soundPlayFightMusic();
            }
        }
        else // MODE_MULTIPLAYER
        {
            // Se alguém ganhou o round e o outro perdeu (1 x 0),
            // mostra o quiz para o jogador que perdeu (na prática,
            // vcs combinam quem responde, já que dividem o teclado).
            if (game.round < MAX_ROUNDS &&   // ainda vai ter próximo round
                ((game.playerWins == 1 && game.cpuWins == 0) ||
                 (game.cpuWins == 1 && game.playerWins == 0)))
            {
                runLogicQuiz();   // só educativo, sem buff por enquanto

                soundStopMusic();
                soundPlayFightMusic();
            }
        }
    }

    // quem ganhou a melhor de 3?
    int playerWonMatch = (game.playerWins > game.cpuWins);

    drawEndScreen(&game.player, &game.cpu);

    int finalScore = calculateScore(&game);

    // --- LER NOMES DOS JOGADORES ---
    char *player1Name = NULL;
    char *player2Name = NULL;

    if (mode == MODE_VS_CPU)
    {
        // só pergunta o nome do player humano
        player1Name = askPlayerName("Partida encerrada!", "Digite seu nome:");
    }
    else
    {
        // modo multiplayer → pergunta nome dos dois
        player1Name = askPlayerName("Partida encerrada!", "Nome do Jogador 1:");
        player2Name = askPlayerName("Partida encerrada!", "Nome do Jogador 2:");
    }

    // vida máxima possível e vida "mantida"
    int maxHpTotal = MAX_HP * game.roundsPlayed;
    if (maxHpTotal <= 0)
        maxHpTotal = MAX_HP;

    int hpTotalFinal = maxHpTotal - game.totalHpLost;
    if (hpTotalFinal < 0)
        hpTotalFinal = 0;

    soundStopMusic();
    soundPlayScoreMusic();

    // drawScoreScreen(GameMode mode, int score, const char *player1Name, const char *player2Name, ...)
    drawScoreScreen(mode,
                    finalScore,
                    player1Name,
                    player2Name,
                    playerWonMatch,
                    maxHpTotal,
                    hpTotalFinal,
                    game.roundsPlayed,
                    game.roundResult);

    // libera memória
    free(player1Name);
    if (player2Name) free(player2Name);
}