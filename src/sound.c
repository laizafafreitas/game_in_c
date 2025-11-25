// src/sound.c
#include <stdlib.h>
#include <stdio.h>
#include "sound.h"

// ---------------------------------------------------------------------
// Seleção de backend de áudio via macros do compilador
// ---------------------------------------------------------------------

#if defined(__APPLE__)          // macOS
    #define CMD_PLAY  "afplay \"%s\" &"
    #define CMD_KILL  "killall afplay >/dev/null 2>&1"
#elif defined(__linux__)        // Linux
    // -q = quiet, redireciona saída pra não sujar o terminal
    #define CMD_PLAY  "mpg123 -q \"%s\" >/dev/null 2>&1 &"
    #define CMD_KILL  "killall mpg123 >/dev/null 2>&1"
#else                           // outros (só pra não quebrar build)
    #define CMD_PLAY  "echo \"som não suportado neste SO\" >/dev/null 2>&1"
    #define CMD_KILL  "true"
#endif

// ---------------------------------------------------------------------
// helpers internos
// ---------------------------------------------------------------------

// som curto (hit, quiz etc.)
static void playOneShot(const char *file)
{
    char cmd[256];
    snprintf(cmd, sizeof(cmd), CMD_PLAY, file);
    system(cmd);
}

// para qualquer player de música que esteja tocando
void soundStopMusic(void)
{
    system(CMD_KILL);
}

// música de fundo: toca uma vez em background
static void playMusic(const char *file)
{
    soundStopMusic(); // garante que só uma música toca por vez

    char cmd[256];
    snprintf(cmd, sizeof(cmd), CMD_PLAY, file);
    system(cmd);
}

// ---------------------------------------------------------------------
// efeitos
// ---------------------------------------------------------------------

void soundPlayHit(void)
{
    playOneShot("assets/sounds/punch.mp3");
}

void soundPlayWin(void)
{
    playOneShot("assets/sounds/Win.mp3");
}

void soundPlayLose(void)
{
    playOneShot("assets/sounds/gameOver.mp3");
}

void soundPlayQuizCorrect(void)
{
    playOneShot("assets/sounds/quizCorrect.mp3");
}

void soundPlayQuizWrong(void)
{
    playOneShot("assets/sounds/quizWrong.mp3");
}

// ---------------------------------------------------------------------
// músicas de fundo
// ---------------------------------------------------------------------

void soundPlayMenuMusic(void)
{
    playMusic("assets/sounds/mainMenuTheme.mp3");
}

void soundPlayFightMusic(void)
{
    playMusic("assets/sounds/fightTheme.mp3");
}

void soundPlayQuizTime(void)
{
    playMusic("assets/sounds/quizGameTime.mp3");
}

void soundPlayScoreMusic(void)
{
    playMusic("assets/sounds/mainMenuTheme.mp3");
}
