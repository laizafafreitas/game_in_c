#include "lib/screen.h"
#include "lib/keyboard.h"
#include <stdio.h>

void runTutorial(void)
{
    clearGameArea();

    // TÍTULO + NUVENS
    screenSetColor(CYAN, LIGHTCYAN);
    screenGotoxy(23, 5);
    printf("╔══════════════════════════════╗");
    screenGotoxy(23, 6);
    printf("║           TUTORIAL           ║");
    screenGotoxy(23, 7);
    printf("╚══════════════════════════════╝");

    // TABELA OPERADORES
    screenSetColor(WHITE, LIGHTCYAN);

    screenGotoxy(11, 9);
    printf("┌────────────────────────────┬────────────────────────────┐");
    screenGotoxy(11, 10);
    printf("│          OPERADOR          │         SIGNIFICADO        │");
    screenGotoxy(11, 11);
    printf("├────────────────────────────┼────────────────────────────┤");

    screenGotoxy(11, 12);
    printf("│   ~  /  ¬                  │   Negação (NOT)            │");

    screenGotoxy(11, 13);
    printf("│   ^  /  &  /  &&           │   Conjunção (AND)          │");

    screenGotoxy(11, 14);
    printf("│   v  /  |  /  ||           │   Disjunção (OR)           │");

    screenGotoxy(11, 15);
    printf("│   ⊕  /  ^                  │   XOR (OU exclusivo)       │");

    screenGotoxy(11, 16);
    printf("│   ->                       │   Implicação               │");

    screenGotoxy(11, 17);
    printf("│   <->  /  =                │   Bicondicional            │");

    screenGotoxy(11, 18);
    printf("└────────────────────────────┴────────────────────────────┘");

    // BOTÃO DE SAIR
    screenSetColor(YELLOW, LIGHTCYAN);
    screenGotoxy(33, 21);
    printf("[ 0 ] Voltar");

    screenUpdate();

    // Espera usuário apertar 0 ou ESC
    while (1)
    {
        if (keyhit())
        {
            int key = readch();
            if (key == '0' || key == 27)
                return;
        }
    }
}
