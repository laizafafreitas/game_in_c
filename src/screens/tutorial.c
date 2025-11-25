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

    screenGotoxy(8, 9);
    printf("┌────────────────────────────┬──────────────────────────────────┐");
    screenGotoxy(8, 10);
    printf("│          OPERADOR          │         SIGNIFICADO              │");
    screenGotoxy(8, 11);
    printf("├────────────────────────────┼──────────────────────────────────┤");

    screenGotoxy(8, 12);
    printf("│   ~  /  ¬                  │   Negação (Não)                  │");

    screenGotoxy(8, 13);
    printf("│   ^  /  &  /  &&           │   Conjunção (E)                  │");

    screenGotoxy(8, 14);
    printf("│   v  /  |  /  ||           │   Disjunção (OU)                 │");

    screenGotoxy(8, 15);
    printf("│   ⊕  /  ^                  │   XOR (OU exclusivo)             │");

    screenGotoxy(8, 16);
    printf("│   ->                       │   Implicação  (SE, ENTÃO)        │");

    screenGotoxy(8, 17);
    printf("│   <->  /  =                │   Bicondicional (SE, SOMENTE SE) │");

    screenGotoxy(8, 18);
    printf("└────────────────────────────┴──────────────────────────────────┘");

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
