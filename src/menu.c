#include <stdio.h>
#include "lib/screen.h"
#include "lib/keyboard.h"

int runMenu(void)
{
    int option = -1;

    clearGameArea();
    screenSetColor(CYAN, LIGHTCYAN);

    screenGotoxy(24, 3);
    screenSetBold();
    printf("╔══════════════════════════════╗");
    screenGotoxy(24, 4);
    printf("║        LOGIC FIGHTERS        ║");
    screenGotoxy(24, 5);
    printf("╚══════════════════════════════╝");
    screenSetNormal();

    screenSetColor(WHITE, LIGHTCYAN); 
    screenGotoxy(32, 7);
    printf("[ 1 ] Vs Bot"); 
    screenGotoxy(32, 9); 
    printf("[ 2 ] Vs Player"); 
    screenGotoxy(32, 11); 
    printf("[ 3 ] Tutorial "); 
    screenGotoxy(32, 14); 
    printf("[ 0 ] Sair"); 

    screenSetColor(LIGHTGREEN, LIGHTCYAN);
    screenGotoxy(31, 17);
    printf("Escolha uma opção: ");
    screenUpdate();

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
