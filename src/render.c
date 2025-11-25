#include <stdio.h>

#include "lib/screen.h"
#include "lib/keyboard.h"

#include "fighters.h"
#include "render.h"
#include "config.h"

// -----------------------------------------------------------------------------
// SPRITES
// -----------------------------------------------------------------------------
static const char *PLAYER_IDLE_LEFT[3] = {
    " O  ",
    "/|>",
    "/ \\"
};

static const char *PLAYER_IDLE_RIGHT[3] = {
    " O  ",
    "<|\\",
    "/ \\"
};

// Player atacando
static const char *PLAYER_ATTACK_RIGHT[3] = {
    " O ",
    "<|‾‾",
    "/ \\"
};

static const char *PLAYER_ATTACK_LEFT[3] = {
    "  O ",
    "‾‾|>",
    " / \\"
};

// Bot parado
static const char *BOT_IDLE_RIGHT[3] = {
    " @  ",
    "<|\\",
    "/ \\"
};

static const char *BOT_IDLE_LEFT[3] = {
    " @  ",
    "/|>",
    "/ \\"
};

// Bot atacando
static const char *BOT_ATTACK_LEFT[3] = {
    "  @ ",
    "‾‾|>",
    " / \\"
};

static const char *BOT_ATTACK_RIGHT[3] = {
    " @  ",
    "<|‾‾",
    "/ \\ "
};

// -----------------------------------------------------------------------------
// Funções internas de render
// -----------------------------------------------------------------------------

static void drawSprite(int x, int topY, const char *sprite[3])
{
    for (int i = 0; i < 3; i++)
    {
        screenGotoxy(x, topY + i);
        printf("%s", sprite[i]);
    }
}

void drawFloor(void)
{
    screenGotoxy(SCRSTARTX - 1, SCRENDY - 1);
    for (int i = 0; i < MAXX - 3; i++)
        printf("=");
}

void drawHealthBar(int x, int y, int hp)
{
    int width = 20;

    if (hp < 0) hp = 0;
    if (hp > MAX_HP) hp = MAX_HP;

    int filled = (hp * width) / MAX_HP;

    screenGotoxy(x, y);
    printf("[");

    for (int i = 0; i < width; i++)
        printf(i < filled ? "█" : " ");

    printf("] %3d", hp);
}

void drawTimer(int x, int y, int timeLeft)
{
    screenGotoxy(x+3, y+1);
    printf("%02d", timeLeft);
}

// -----------------------------------------------------------------------------
// Background
// -----------------------------------------------------------------------------

void drawBackground(void)
{
    // Céu
    screenSetColor(LIGHTBLUE, LIGHTCYAN);

    for (int x = SCRSTARTX + 1; x < SCRENDX; x++)
    {
        screenGotoxy(x, SCRSTARTY + 3);
        printf((x % 7 == 0) ? "." : " ");

        screenGotoxy(x, SCRSTARTY + 4);
        printf((x % 11 == 0) ? "." : " ");
    }

    // Nuvens
    screenSetColor(CYAN, LIGHTCYAN);

    screenGotoxy(SCRSTARTX + 8, SCRSTARTY + 3);
    printf("  _ _");
    screenGotoxy(SCRSTARTX + 7, SCRSTARTY + 4);
    printf(" _(   )__");

    screenGotoxy(SCRENDX - 18, SCRSTARTY + 3);
    printf(" __");
    screenGotoxy(SCRENDX - 20, SCRSTARTY + 4);
    printf("_(    )___");

    // Letreiro
    screenSetColor(LIGHTMAGENTA, LIGHTCYAN);
    int midX = (SCRSTARTX + SCRENDX) / 2 - 8;
    screenGotoxy(midX, SCRSTARTY + 6);
    printf("[ LOGIC CITY ]");

    // Prédios
    screenSetColor(DARKGRAY, LIGHTCYAN);
    for (int y = SCRSTARTY + 8; y < SCRENDY - 3; y++)
    {
        screenGotoxy(SCRSTARTX + 5, y);  printf("||");
        screenGotoxy(SCRSTARTX + 9, y);  printf("||");
        screenGotoxy(SCRENDX - 10, y);   printf("||");
        screenGotoxy(SCRENDX - 6,  y);   printf("||");
    }

    // Janelas
    screenSetColor(CYAN, LIGHTCYAN);
    for (int y = SCRSTARTY + 8; y < SCRENDY - 4; y += 2)
    {
        screenGotoxy(SCRSTARTX + 6, y); printf("[]");
        screenGotoxy(SCRSTARTX + 10, y); printf("[]");
        screenGotoxy(SCRENDX - 9, y); printf("[]");
        screenGotoxy(SCRENDX - 5, y); printf("[]");
    }

    // Horizonte
    screenSetColor(GREEN, LIGHTCYAN);

    for (int x = SCRSTARTX + 1; x < SCRENDX; x++)
    {
        screenGotoxy(x, SCRENDY - 3);
        printf((x % 5 == 0) ? "^" : "_");
    }

    // Totens lógicos
    screenSetColor(LIGHTMAGENTA, LIGHTCYAN);
    screenGotoxy(midX - 12, SCRENDY - 10); printf("[ A v B ]");
    screenGotoxy(midX + 20, SCRENDY - 10); printf("[ ¬P -> Q ]");

    // volta cor padrão
    screenSetColor(WHITE, LIGHTCYAN);
}

// -----------------------------------------------------------------------------
// Draw fighter
// -----------------------------------------------------------------------------

void drawFighter(const Fighter *f, int topY, int isPlayer)
{
    const char **idleLeft, **idleRight, **atkLeft, **atkRight;

    if (isPlayer)
    {
        idleLeft  = PLAYER_IDLE_LEFT;
        idleRight = PLAYER_IDLE_RIGHT;
        atkLeft   = PLAYER_ATTACK_LEFT;
        atkRight  = PLAYER_ATTACK_RIGHT;
        screenSetColor(LIGHTCYAN, LIGHTCYAN);
    }
    else
    {
        idleLeft  = BOT_IDLE_LEFT;
        idleRight = BOT_IDLE_RIGHT;
        atkLeft   = BOT_ATTACK_LEFT;
        atkRight  = BOT_ATTACK_RIGHT;
        screenSetColor(LIGHTRED, LIGHTCYAN);
    }

    const char **sprite =
        f->attacking ? (f->facing == 1 ? atkRight : atkLeft)
                     : (f->facing == 1 ? idleRight : idleLeft);

    drawSprite(f->x, topY, sprite);

    screenSetColor(WHITE, LIGHTCYAN);
}

// -----------------------------------------------------------------------------
// HUD
// -----------------------------------------------------------------------------

void drawHUD(const Fighter *player,
             const Fighter *cpu,
             int timeLeft,
             int playerWins,
             int cpuWins)
{
    screenSetColor(GREEN, LIGHTCYAN);
    drawHealthBar(SCRSTARTX + 1, SCRSTARTY + 2, player->hp);
    drawHealthBar(SCRENDX - 24, SCRSTARTY + 2, cpu->hp);

    int centerX = (SCRSTARTX + SCRENDX) / 2 - 4;
    screenSetColor(YELLOW, LIGHTCYAN);
    drawTimer(centerX, SCRSTARTY + 1, timeLeft);

    // --- MARCADORES DE ROUND (pontinhos de vitória) ---
    // linha acima do timer
    int winsY = SCRSTARTY + 1;

    // Player (grupo à esquerda)
    screenSetColor(WHITE, LIGHTCYAN);
    screenGotoxy(centerX - 14, winsY);
    putchar(playerWins >= 1 ? 'v' : '.');
    putchar(' ');
    putchar(playerWins >= 2 ? 'v' : '.');

    // CPU (grupo à direita)
    screenGotoxy(centerX + 18, winsY);
    putchar(cpuWins >= 1 ? 'v' : '.');
    putchar(' ');
    putchar(cpuWins >= 2 ? 'v' : '.');

    drawFloor();

    screenSetColor(LIGHTBLUE, LIGHTCYAN);
    screenGotoxy(SCRSTARTX + 1, SCRENDY);
    printf("[A/D] mover  [J] atacar  [Q] sair");

    screenSetColor(WHITE, LIGHTCYAN);
}

// -----------------------------------------------------------------------------
// Tela principal da luta
// -----------------------------------------------------------------------------

void drawGame(const Fighter *player,
              const Fighter *cpu,
              int timeLeft,
              int playerWins,
              int cpuWins)
{
    clearGameArea();       // limpa só a área de jogo (sem piscar a tela inteira)
    drawBackground();
    drawHUD(player, cpu, timeLeft, playerWins, cpuWins);

    int floorY      = SCRENDY - 1;
    int fighterTopY = floorY - 3;

    drawFighter(player, fighterTopY, 1);
    drawFighter(cpu,    fighterTopY, 0);

    screenUpdate();
}

// -----------------------------------------------------------------------------
// Tela do Quiz
// -----------------------------------------------------------------------------

void drawLogicQuizScreen(int timeLeft)
{
    clearGameArea();

    screenSetColor(WHITE, BLACK);
    screenGotoxy(40, 3);
    printf("%d", timeLeft);

    screenSetColor(GREEN, BLACK);
    screenGotoxy(13, 5);
    printf("== George Boole te da a chance de voltar mais forte ==");

    screenSetColor(WHITE, BLACK);
    screenGotoxy(15, 8);
    printf("Quais conectivos logicos estao presentes na frase ?");

    screenSetColor(RED, BLACK);
    screenGotoxy(19, 10);
    printf("\"Se chover então eu não vou para a praia\"");

    screenSetColor(WHITE, BLACK);
    screenGotoxy(33, 12); printf("[ 0 ] apenas ->");
    screenGotoxy(33, 13); printf("[ 1 ] apenas ~");
    screenGotoxy(33, 14); printf("[ 2 ] -> + ~");

    screenUpdate();
}

// -----------------------------------------------------------------------------
// Tela final de vitória/derrota
// -----------------------------------------------------------------------------

void drawEndScreen(const Fighter *player, const Fighter *cpu)
{
    clearGameArea();

    int result = 0;

    if (player->hp > 0 && cpu->hp <= 0) result = 1;
    else if (cpu->hp > 0 && player->hp <= 0) result = 2;

    int msgX = MAXX / 2 - 6;
    int msgY = MAXY / 2;

    switch (result)
    {
        case 1:
            screenSetColor(LIGHTGREEN, LIGHTCYAN);
            screenGotoxy(msgX, msgY);
            printf("VOCE VENCEU!");
            break;

        case 2:
            screenSetColor(LIGHTRED, LIGHTCYAN);
            screenGotoxy(msgX, msgY);
            printf("VOCE PERDEU!");
            break;

        default:
            screenSetColor(YELLOW, LIGHTCYAN);
            screenGotoxy(msgX, msgY);
            printf("EMPATE!");
            break;
    }

    screenSetColor(WHITE, LIGHTCYAN);
    screenGotoxy(MAXX / 2 - 12, MAXY / 2 + 2);
    printf("Pressione ESC para sair...");

    screenUpdate();

    int key = 0;
    while (key != 27)
        if (keyhit()) key = readch();
}