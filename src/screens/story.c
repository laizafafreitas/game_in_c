#include <stdio.h>
#include <string.h>

#include "lib/screen.h"
#include "lib/keyboard.h"

#include "config.h"
#include "render.h"  
#include "sound.h"

static void centerText(int y, const char *text)
{
    int centerX = (SCRSTARTX + SCRENDX) / 2;
    int len     = (int)strlen(text);
    int x       = centerX - len / 2;

    screenGotoxy(x, y);
    printf("%s", text);
}

void showStoryScreen(void)
{
    clearGameArea();

    screenSetColor(WHITE, LIGHTCYAN);

    int y = SCRSTARTY + 5;

    centerText(y++, "A cidade futurista Logic City foi construída sobre os pilares");
    centerText(y++, "da logica formal. Ha decadas, um virus logico chamado");
    centerText(y++, "Contradictor comecou a corromper raciocinios, inverter");
    centerText(y++, "proposicoes e gerar paradoxos.");

    y += 1;
    centerText(y++, "Para restaurar a ordem, dois campeoes entram na arena do");
    centerText(y++, "raciocinio logico para duelar — nao apenas com golpes,");
    centerText(y++, "mas com conhecimento.");

    y += 1;
    centerText(y++, "Entre cada round, surge a figura mitica de George Boole,");
    centerText(y++, "fundador da logica moderna, oferecendo uma chance de");
    centerText(y++, "retornar mais forte: se o jogador acerta a pergunta,");
    centerText(y++, "ganha um buff de dano.");

    // instrucoes de avancar
    screenSetColor(LIGHTMAGENTA, LIGHTCYAN);
    centerText(SCRENDY - 2, "[ENTER] Avancar para o menu");

    screenUpdate();

    // espera o jogador apertar ENTER para avancar
    int key = 0;
    while (1)
    {
        if (keyhit())
        {
            key = readch();
            if (key == '\n' || key == '\r' || key == 13)
                break;
        }
    }
}
