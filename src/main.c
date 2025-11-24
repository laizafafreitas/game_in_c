#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"

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

    if (choice != 0) {
        runFight();
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();
    return 0;
}