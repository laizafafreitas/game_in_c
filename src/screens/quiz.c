#include "screens/quiz.h"

#include "lib/timer.h"
#include "lib/keyboard.h"

#include "config.h"
#include "render.h"

void runLogicQuiz(void)
{
    int timeLeft     = QUIZ_TIME;
    int frameCounter = 0;
    int answered     = 0;

    while (timeLeft > 0 && !answered)
    {
        // UPDATE + RENDER sincronizados no timer (≈ FPS vezes por segundo)
        if (timerTimeOver())
        {
            // desenha a tela do quiz com o tempo restante
            drawLogicQuizScreen(timeLeft);

            // controla "segundos" do quiz
            frameCounter++;
            if (frameCounter >= FPS)
            {
                timeLeft--;
                frameCounter = 0;
            }
        }

        // lê resposta do jogador (não depende do timer)
        if (keyhit())
        {
            int ch = readch();

            switch (ch)
            {
                case '0':
                case '1':
                case '2':
                case '3':
                    // aqui depois dá pra checar se acertou ou não
                    answered = 1;
                    break;

                case 27: // ESC sai do quiz
                    answered = 1;
                    break;

                default:
                    break;
            }
        }
    }

    // por enquanto o resultado não afeta nada;
    // futuramente dá pra dar bônus/malus aqui.
}