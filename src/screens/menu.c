#include <stdio.h>
#include "lib/screen.h"
#include "lib/keyboard.h"

void drawMenuBackground(void)
{
    int midX = (SCRSTARTX + SCRENDX) / 2 - 8;

    // CÉU
    screenSetColor(LIGHTBLUE, LIGHTCYAN);
    for (int x = SCRSTARTX + 1; x < SCRENDX; x++) {
        screenGotoxy(x, SCRSTARTY + 2);
        printf((x % 7 == 0) ? "." : " ");

        screenGotoxy(x, SCRSTARTY + 3);
        printf((x % 11 == 0) ? "." : " ");
    }

    // NUVENS
    screenSetColor(WHITE, LIGHTCYAN);

    // esquerda
    screenGotoxy(SCRSTARTX + 8, SCRSTARTY + 2);
    printf("  _ _");
    screenGotoxy(SCRSTARTX + 7, SCRSTARTY + 3);
    printf(" _(   )__");

    // direita
    screenGotoxy(SCRENDX - 18, SCRSTARTY + 2);
    printf(" __");
    screenGotoxy(SCRENDX - 20, SCRSTARTY + 3);
    printf("_(    )___");

    // LETREIRO LOGIC CITY
    screenSetColor(LIGHTMAGENTA, LIGHTCYAN);
    screenGotoxy(midX, SCRSTARTY + 5);
    printf("[ LOGIC CITY ]");

    // PRÉDIOS
    screenSetColor(DARKGRAY, LIGHTCYAN);
    for (int y = SCRSTARTY + 7; y < SCRENDY - 3; y++) {

        // prédio esquerdo
        screenGotoxy(SCRSTARTX + 5, y);
        printf("||");
        screenGotoxy(SCRSTARTX + 9, y);
        printf("||");

        // prédio direito
        screenGotoxy(SCRENDX - 10, y);
        printf("||");
        screenGotoxy(SCRENDX - 6, y);
        printf("||");
    }

    // janelas
    screenSetColor(CYAN, LIGHTCYAN);
    for (int y = SCRSTARTY + 7; y < SCRENDY - 4; y += 2) {
        screenGotoxy(SCRSTARTX + 6, y);
        printf("[]");
        screenGotoxy(SCRSTARTX + 10, y);
        printf("[]");
        screenGotoxy(SCRENDX - 9, y);
        printf("[]");
        screenGotoxy(SCRENDX - 5, y);
        printf("[]");
    }

    // HORIZONTE / GRAMA
    screenSetColor(GREEN, LIGHTCYAN);
    for (int x = SCRSTARTX + 1; x < SCRENDX; x++) {
        screenGotoxy(x, SCRENDY - 3);
        printf((x % 5 == 0) ? "^" : "_");
    }

}


int runMenu(void)
{
    int option = -1;

    clearGameArea();
    drawMenuBackground();

    // =========================
    // TÍTULO
    // =========================
    screenSetColor(CYAN, LIGHTCYAN);

    screenGotoxy(24, 4);
    screenSetBold();
    printf("╔══════════════════════════════╗");
    screenGotoxy(24, 5);
    printf("║        LOGIC FIGHTERS        ║");
    screenGotoxy(24, 6);
    printf("╚══════════════════════════════╝");
    screenSetNormal();

    // OPÇÕES DO MENU
    screenSetColor(WHITE, LIGHTCYAN);
    screenGotoxy(32, 8);
    printf("[ 1 ] Vs Bot");

    screenGotoxy(32, 10);
    printf("[ 2 ] Vs Player");

    screenGotoxy(32, 12);
    printf("[ 3 ] Tutorial");

    screenGotoxy(32, 15);
    printf("[ 0 ] Sair");

    // INSTRUÇÃO
    screenSetColor(YELLOW, LIGHTCYAN);
    screenGotoxy(31, 18);
    printf("Escolha uma opção: ");

    screenUpdate();

    // INPUT
    while (option == -1)
    {
        if (keyhit())
        {
            int key = readch();
            switch (key)
            {
                case '1': option = 1; break;
                case '2': option = 2; break;
                case '3': option = 3; break;
                case '0': option = 0; break;
            }
        }
    }

    return option;
}
