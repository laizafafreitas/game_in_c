#ifndef RENDER_H
#define RENDER_H

#include "fighters.h"
#include "game.h"

void drawBackground(void);
void drawFloor(void);
void drawFighter(const Fighter *f, int y, int isPlayer);
void drawTimer(int x, int y, int timeLeft);
void drawHealthBar(int x, int y, int hp);
void drawHUD(const Fighter *player,
             const Fighter *cpu,
             int timeLeft,
             int playerWins,
             int cpuWins,
             GameMode mode);
void drawGame(const Fighter *player,
              const Fighter *cpu,
              int timeLeft,
              int playerWins,
              int cpuWins,
              GameMode mode);
void drawEndScreen(const Fighter *player, const Fighter *cpu);
void drawLogicQuizScreen(int timeLeft, int questionId);
void drawQuizResultScreen(int acertou, int questionId);
void drawScoreScreen(GameMode mode,
                     int score,
                     const char *player1Name,
                     const char *player2Name,
                     int playerWonMatch,
                     int maxHpTotal,
                     int hpTotalFinal,
                     int roundsPlayed,
                     const int roundResults[]);

#endif