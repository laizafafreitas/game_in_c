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
                case '2':
                    // resposta correta
                    drawQuizResultScreen(1);
                    return 1;

                case '0':
                case '1':
                    // resposta errada
                    drawQuizResultScreen(0);
                    return 0;

                case 27:  // ESC → considera erro
                    drawQuizResultScreen(0);
                    return 0;

                default:
                    break;
            }
        }
    }

    // tempo acabou → errou
    drawQuizResultScreen(0);
    return 0;
}
