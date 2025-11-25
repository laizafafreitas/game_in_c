#include <stdlib.h>

#include "screens/quiz.h"

#include "lib/timer.h"
#include "lib/keyboard.h"

#include "config.h"
#include "render.h"
#include "sound.h"

#define NUM_QUIZ_QUESTIONS 6
static const int QUIZ_CORRECT_OPTION[NUM_QUIZ_QUESTIONS] = {
    2, // questionId 0 -> correta: [2]
    1, // questionId 1 -> correta: [1]
    0, // questionId 2 -> correta: [0]
    0, // questionId 3 -> correta: [0]
    0, // questionId 4 -> correta: [0]
    1  // questionId 5 -> correta: [1]
};

int runLogicQuiz(void)
{
    soundStopMusic();
    soundPlayQuizTime();

    int timeLeft     = QUIZ_TIME;
    int frameCounter = 0;

    // Escolhe uma pergunta aleatoria
    int questionId = rand() % NUM_QUIZ_QUESTIONS;
    int correctOption = QUIZ_CORRECT_OPTION[questionId]; // 0, 1 ou 2

    while (timeLeft > 0)
    {
        if (timerTimeOver())
        {
            drawLogicQuizScreen(timeLeft, questionId);

            frameCounter++;
            if (frameCounter >= FPS)
            {
                timeLeft--;
                frameCounter = 0;
            }
        }

        if (keyhit())
        {
            int ch = readch();

            // Se apertou 0, 1 ou 2
            if (ch == '0' || ch == '1' || ch == '2')
            {
                int chosen = ch - '0'; // transforma '0','1','2' em 0,1,2
                int acertou = (chosen == correctOption);

                drawQuizResultScreen(acertou, questionId);
                return acertou ? 1 : 0;
            }
            else if (ch == 27) // ESC
            {
                drawQuizResultScreen(0, questionId);
                return 0;
            }
        }
    }

    // tempo acabou → errou
    drawQuizResultScreen(0, questionId);

    soundStopMusic();
    
    return 0;
}
