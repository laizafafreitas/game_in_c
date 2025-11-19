#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#include "game.h"      

#define FPS 30

int main(void) {
    srand((unsigned) time(NULL));

    screenInit(1);
    keyboardInit();
    timerInit(1000 / FPS);

    // aqui você chama a tela de luta
    runFight();

    // outras telas:
    // runMenu();
    // runTutorial();

    keyboardDestroy();
    screenDestroy();
    timerDestroy();
    return 0;
}