#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lib/screen.h"
#include "lib/keyboard.h"
#include "lib/timer.h"

#include "screens/game.h"      // runFight()
#include "screens/menu.h"      // runMenu()
#include "screens/tutorial.h"  // runTutorial()
// se tiver um modo Vs Player depois, você pode incluir outro header

int main(void) {
    srand((unsigned) time(NULL));

    screenInit(1);
    keyboardInit();
    timerInit(1000 / FPS);

    int running = 1;

    while (running) {
        int choice = runMenu();  

        switch (choice) {
            case 1:
                clearGameArea();
                runFight();       
                break;

            case 2:
                break;

            case 3:
                clearGameArea();
                runTutorial();    
                break;

            case 0:
                running = 0;
                break;

            default:
                // opção inválida -> volta pro menu
                break;
        }
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();
    return 0;
}
