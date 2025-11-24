#include <stdio.h>
#include "screen.h"
#include "keyboard.h"

int runMenu(void)
{
    int option = -1;

    clearGameArea();  // limpa só a área dentro das bordas
    screenSetColor(GREEN, BLACK);

    screenGotoxy(30, 3);
    printf("==== Logic Fighters ====");

    screenSetColor(WHITE, BLACK);
    screenGotoxy(32, 7);
    printf("[ 1 ] Vs Bot");

    screenGotoxy(32, 9);
    printf("[ 2 ] Vs Player");

    screenGotoxy(32, 11);
    printf("[ 0 ] Sair");

    screenSetColor(GREEN, BLACK);
    screenGotoxy(32, 15);
    printf("key code: ");

    screenUpdate();   // <<< IMPORTANTE !!! Mostra tudo na tela AGORA

    while (option == -1)
    {
        if (keyhit())
        {
            int key = readch();

            switch (key)
            {
                case '1': option = 1; break;
                case '2': option = 2; break;
                case '0': option = 0; break;
            }
        }
    }

    return option;
}
