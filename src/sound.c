#include <stdlib.h>
#include <stdio.h>
#include "sound.h"

// som curto (hit, quiz etc.)
static void playOneShot(const char *file)
{
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "afplay \"%s\" &", file);
    system(cmd);
}

// para qualquer afplay que esteja tocando
void soundStopMusic(void)
{
    system("killall afplay >/dev/null 2>&1");
}

// música de fundo: toca uma vez em background
static void playMusic(const char *file)
{
    soundStopMusic(); 

    char cmd[256];
    snprintf(cmd, sizeof(cmd), "afplay \"%s\" &", file);
    system(cmd);
}


// ---------- efeitos ----------

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

// ---------- músicas de fundo ----------

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
