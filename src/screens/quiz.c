#include "screens/quiz.h"

#include "lib/timer.h"
#include "lib/keyboard.h"

#include "config.h"
#include "render.h"

int runLogicQuiz(void)
{
    int timeLeft     = QUIZ_TIME;
    int frameCounter = 0;

    while (timeLeft > 0)
    {
        if (timerTimeOver())
        {
            drawLogicQuizScreen(timeLeft);

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

            switch (ch)
            {
                case '2': return 1; // correta
                case '0':
                case '1': return 0; // errada
                case 27:  return 0; // ESC → erro
                default: break;
            }
        }
    }

    // tempo acabou → errou
    return 0;
}