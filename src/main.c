#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lib/screen.h"
#include "lib/keyboard.h"
#include "lib/timer.h"

#include "game.h"
#include "menu.h"      

#define FPS 30

int main(void) {
    srand((unsigned) time(NULL));

    screenInit(1);
    keyboardInit();
    timerInit(1000 / FPS);

    int choice = runMenu();
    clearGameArea();

    switch (choice) {
        case 1:
            runFight();     
            break;

        case 2:
            break;

        case 3:
            // runTutorial();     
            break;

        case 0:
            break;

        default:
            // opção inválida
            break;
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();
    return 0;
}