#include <stdio.h>

#include "lib/screen.h"
#include "lib/keyboard.h"
#include "lib/screen.h"

#include "fighters.h"
#include "render.h"

static const char *PLAYER_IDLE_LEFT[3] = {
    " O  ",
    "/|>",
    "/ \\"};

// Player parado (virado pra esquerda)
static const char *PLAYER_IDLE_RIGHT[3] = {
    " O  ",
    "<|\\",
    "/ \\"};

// Player atacando para a direita
static const char *PLAYER_ATTACK_RIGHT[3] = {
    " O ",
    "<|‾‾",
    "/ \\"};

// Player atacando para a esquerda
static const char *PLAYER_ATTACK_LEFT[3] = {
    "  O ",
    "‾‾|>",
    " / \\"};

// Bot parado (virado pra direita)
static const char *BOT_IDLE_RIGHT[3] = {
    " @  ",
    "<|\\",
    "/ \\"};

// Bot parado (virado pra esquerda)
static const char *BOT_IDLE_LEFT[3] = {
    " @  ",
    "/|>",
    "/ \\"};

// Bot atacando para a direita
static const char *BOT_ATTACK_LEFT[3] = {
    "  @ ",
    "‾‾|>",
    " / \\"};

// Bot atacando para a esquerda
static const char *BOT_ATTACK_RIGHT[3] = {
    " @  ",
    "<|‾‾",
    "/ \\ "};

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
    screenGotoxy(SCRSTARTX - 1, SCRENDY - 1.5);
    for (int i = 0; i < MAXX - 3; i++)
    {
        printf("=");
    }
}

void drawHealthBar(int x, int y, int hp)
{
    int width = 20;
    if (hp < 0)
        hp = 0;
    if (hp > MAX_HP)
        hp = MAX_HP;

    int filled = (hp * width) / MAX_HP;

    screenGotoxy(x, y);
    printf("[");
    for (int i = 0; i < width; i++)
    {
        printf(i < filled ? "█" : " ");
    }
    printf("] %3d", hp);
}

void drawTimer(int x, int y, int timeLeft)
{
    screenGotoxy(x, y);
    printf("TIME: %02d", timeLeft);
}

void drawBackground(void)
{
    //  CÉU / FUNDO
    // faixas de "céu" com pontinhos
    screenSetColor(LIGHTBLUE, LIGHTCYAN);
    for (int x = SCRSTARTX + 1; x < SCRENDX; x++)
    {
        // linha mais alta de céu
        screenGotoxy(x, SCRSTARTY + 3);
        if (x % 7 == 0)
            printf(".");
        else
            printf(" ");

        // segunda linha de céu
        screenGotoxy(x, SCRSTARTY + 4);
        if (x % 11 == 0)
            printf(".");
        else
            printf(" ");
    }

    //  NUVENS
    screenSetColor(CYAN, LIGHTCYAN);
    // nuvem esquerda
    screenGotoxy(SCRSTARTX + 8, SCRSTARTY + 3);
    printf("  _ _");
    screenGotoxy(SCRSTARTX + 7, SCRSTARTY + 4);
    printf(" _(   )__");
    // nuvem direita
    screenGotoxy(SCRENDX - 18, SCRSTARTY + 3);
    printf(" __");
    screenGotoxy(SCRENDX - 20, SCRSTARTY + 4);
    printf("_(    )___");

    //  "LETREIRO" AO FUNDO
    screenSetColor(LIGHTMAGENTA, LIGHTCYAN);
    int midX = (SCRSTARTX + SCRENDX) / 2 - 8;
    screenGotoxy(midX, SCRSTARTY + 6);
    printf("[ LOGIC CITY ]");

    //  PRÉDIOS / ESTRUTURAS
    screenSetColor(DARKGRAY, LIGHTCYAN);
    for (int y = SCRSTARTY + 8; y < SCRENDY - 3; y++)
    {
        // prédio da esquerda
        screenGotoxy(SCRSTARTX + 5, y);
        printf("||");
        screenGotoxy(SCRSTARTX + 9, y);
        printf("||");

        // prédio da direita
        screenGotoxy(SCRENDX - 10, y);
        printf("||");
        screenGotoxy(SCRENDX - 6, y);
        printf("||");
    }

    // janelinhas
    screenSetColor(CYAN, LIGHTCYAN);
    for (int y = SCRSTARTY + 8; y < SCRENDY - 4; y += 2)
    {
        screenGotoxy(SCRSTARTX + 6, y);
        printf("[]");
        screenGotoxy(SCRSTARTX + 10, y);
        printf("[]");
        screenGotoxy(SCRENDX - 9, y);
        printf("[]");
        screenGotoxy(SCRENDX - 5, y);
        printf("[]");
    }

    //  HORIZONTE
    screenSetColor(GREEN, LIGHTCYAN);
    for (int x = SCRSTARTX + 1; x < SCRENDX; x++)
    {
        screenGotoxy(x, SCRENDY - 3);
        if (x % 5 == 0)
            printf("^"); // graminha
        else
            printf("_");
    }

    // pequeno “totem” lógico no fundo
    screenSetColor(LIGHTMAGENTA, LIGHTCYAN);
    screenGotoxy(midX - 12, SCRENDY - 10);
    printf("[ A v B ]");
    screenGotoxy(midX + 20, SCRENDY - 10);
    printf("[ ¬P -> Q ]");

    // volta pra cor neutra
    screenSetColor(WHITE, LIGHTCYAN);
}

void drawFighter(const Fighter *f, int topY, int isPlayer)
{
    const char **idleLeft, **idleRight, **atkLeft, **atkRight;

    if (isPlayer)
    {
        idleLeft = PLAYER_IDLE_LEFT;
        idleRight = PLAYER_IDLE_RIGHT;
        atkLeft = PLAYER_ATTACK_LEFT;
        atkRight = PLAYER_ATTACK_RIGHT;
        screenSetColor(LIGHTCYAN, LIGHTCYAN);
    }
    else
    {
        idleLeft = BOT_IDLE_LEFT;
        idleRight = BOT_IDLE_RIGHT;
        atkLeft = BOT_ATTACK_LEFT;
        atkRight = BOT_ATTACK_RIGHT;
        screenSetColor(LIGHTRED, LIGHTCYAN);
    }

    const char **sprite;
    int facingRight = (f->facing == 1);

    if (f->attacking)
    {
        sprite = facingRight ? atkRight : atkLeft;
    }
    else
    {
        sprite = facingRight ? idleRight : idleLeft;
    }

    drawSprite(f->x, topY, sprite);

    screenSetColor(WHITE, LIGHTCYAN);
}

void drawHUD(const Fighter *player, const Fighter *cpu, int timeLeft)
{
    screenSetColor(GREEN, LIGHTCYAN);
    drawHealthBar(SCRSTARTX + 1, SCRSTARTY + 2, player->hp);

    screenSetColor(GREEN, LIGHTCYAN);
    drawHealthBar(SCRENDX - 24, SCRSTARTY + 2, cpu->hp);

    int centerX = (SCRSTARTX + SCRENDX) / 2 - 4;
    screenSetColor(YELLOW, LIGHTCYAN);
    drawTimer(centerX, SCRSTARTY + 1, timeLeft);

    screenSetColor(YELLOW, LIGHTCYAN);
    drawFloor();

    screenSetColor(LIGHTBLUE, LIGHTCYAN);
    screenGotoxy(SCRSTARTX + 1, SCRENDY);
    printf("[A/D] mover  [J] atacar  [Q] sair");

    screenSetColor(WHITE, LIGHTCYAN);
}

void drawGame(const Fighter *player, const Fighter *cpu, int timeLeft)
{
    clearGameArea();
    drawBackground();
    drawHUD(player, cpu, timeLeft);

    // y dos lutadores (linha "do chão")
    int floorY = SCRENDY - 1;

    // topo do sprite (5 linhas de altura → sobe 4)
    int fighterTopY = floorY - 3;

    drawFighter(player, fighterTopY, 1);
    drawFighter(cpu, fighterTopY, 0);

    screenUpdate();
}

void drawLogicQuizScreen(int timeLeft)
{
    clearGameArea();

    // Título / tempo lá em cima
    screenSetColor(WHITE, BLACK);
    screenGotoxy(40, 2);
    printf("%d", timeLeft);

    screenSetColor(GREEN, BLACK);
    screenGotoxy(20, 4);
    printf("==George Boole te da a chance de voltar mais forte==");

    screenSetColor(WHITE, BLACK);
    screenGotoxy(10, 7);
    printf("Quais conectivos logicos estao presentes na frase ?");

    screenGotoxy(15, 9);
    printf("\"Se chover entao eu não vou para a praia\"");

    screenSetColor(RED, BLACK);
    screenGotoxy(20, 12);
    printf("[ 0 ] apenas ->");
    screenGotoxy(20, 13);
    printf("[ 1 ] apenas ~");
    screenGotoxy(20, 14);
    printf("[ 2 ] -> + ~");

    screenSetColor(GREEN, BLACK);
    screenGotoxy(25, 17);
    printf("key code:");

    screenUpdate();
}

void drawEndScreen(const Fighter *player, const Fighter *cpu)
{
    clearGameArea();

    int result = 0; // 0 = empate, 1 = player vence, 2 = cpu vence

    if (player->hp > 0 && cpu->hp <= 0)
    {
        result = 1;
    }
    else if (cpu->hp > 0 && player->hp <= 0)
    {
        result = 2;
    }

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
    screenGotoxy(MAXX / 2 - 16, MAXY / 2 + 2);
    printf("Pressione qualquer ESC para sair...");

    // Primeiro desenha na tela
    screenUpdate();

    // Agora espera ESC
    int key = 0;
    while (key != 27)
    {
        if (keyhit())
        {
            key = readch();
        }
    }
}