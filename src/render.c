#include <stdio.h>
#include <string.h>

#include "lib/screen.h"
#include "lib/keyboard.h"

#include "fighters.h"
#include "render.h"
#include "config.h"
#include "sound.h"


extern int playerBuffActive;

// -----------------------------------------------------------------------------
// SPRITES
// -----------------------------------------------------------------------------
static const char *PLAYER_IDLE_LEFT[3] = {
    " O  ",
    "/|>",
    "/ \\"};

static const char *PLAYER_IDLE_RIGHT[3] = {
    " O  ",
    "<|\\",
    "/ \\"};

// Player atacando
static const char *PLAYER_ATTACK_RIGHT[3] = {
    " O ",
    "<|‾‾",
    "/ \\"};

static const char *PLAYER_ATTACK_LEFT[3] = {
    "  O ",
    "‾‾|>",
    " / \\"};

// Bot parado
static const char *BOT_IDLE_RIGHT[3] = {
    " @  ",
    "<|\\",
    "/ \\"};

static const char *BOT_IDLE_LEFT[3] = {
    " @  ",
    "/|>",
    "/ \\"};

// Bot atacando
static const char *BOT_ATTACK_LEFT[3] = {
    "  @ ",
    "‾‾|>",
    " / \\"};

static const char *BOT_ATTACK_RIGHT[3] = {
    " @  ",
    "<|‾‾",
    "/ \\ "};

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

static void drawCentered(int y, const char *text)
{
    int centerX = (SCRSTARTX + SCRENDX) / 2;
    int len = (int)strlen(text);
    int x = centerX - len / 2;
    screenGotoxy(x, y);
    printf("%s", text);
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

    if (hp < 0)
        hp = 0;
    if (hp > MAX_HP)
        hp = MAX_HP;

    int filled = (hp * width) / MAX_HP;

    screenGotoxy(x, y);
    printf("[");

    for (int i = 0; i < width; i++)
        printf(i < filled ? "█" : " ");

    printf("] %3d", hp);
}

void drawTimer(int x, int y, int timeLeft)
{
    screenGotoxy(x + 3, y + 1);
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
        screenGotoxy(SCRSTARTX + 5, y);
        printf("||");
        screenGotoxy(SCRSTARTX + 9, y);
        printf("||");
        screenGotoxy(SCRENDX - 10, y);
        printf("||");
        screenGotoxy(SCRENDX - 6, y);
        printf("||");
    }

    // Janelas
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

    // Horizonte
    screenSetColor(GREEN, LIGHTCYAN);

    for (int x = SCRSTARTX + 1; x < SCRENDX; x++)
    {
        screenGotoxy(x, SCRENDY - 3);
        printf((x % 5 == 0) ? "^" : "_");
    }

    // Totens lógicos
    screenSetColor(LIGHTMAGENTA, LIGHTCYAN);
    screenGotoxy(midX - 12, SCRENDY - 10);
    printf("[ A v B ]");
    screenGotoxy(midX + 20, SCRENDY - 10);
    printf("[ ¬P -> Q ]");

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
        idleLeft = PLAYER_IDLE_LEFT;
        idleRight = PLAYER_IDLE_RIGHT;
        atkLeft = PLAYER_ATTACK_LEFT;
        atkRight = PLAYER_ATTACK_RIGHT;
        screenSetColor(LIGHTCYAN, LIGHTCYAN);

        extern int playerBuffActive;
        if (playerBuffActive)
        {
            screenSetColor(LIGHTBLUE, LIGHTCYAN);
        }
    }
    else
    {
        idleLeft = BOT_IDLE_LEFT;
        idleRight = BOT_IDLE_RIGHT;
        atkLeft = BOT_ATTACK_LEFT;
        atkRight = BOT_ATTACK_RIGHT;
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
             int cpuWins,
            GameMode mode)
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

    if (mode == MODE_VS_CPU)
    {
        // Só comandos do P1 + sair
        printf("[P1] A/D mover, F atacar  |  [Q] sair");
    }
    else
    {
        // Multiplayer: mostra os dois
        printf("[P1] A/D mover, F atacar  |  [P2] J/K mover, P atacar  |  [Q] sair");
    }

    screenSetColor(WHITE, LIGHTCYAN);
}

// -----------------------------------------------------------------------------
// Tela principal da luta
// -----------------------------------------------------------------------------

void drawGame(const Fighter *player,
              const Fighter *cpu,
              int timeLeft,
              int playerWins,
              int cpuWins,
            GameMode mode)
{
    clearGameArea(); // limpa só a área de jogo (sem piscar a tela inteira)
    drawBackground();
    drawHUD(player, cpu, timeLeft, playerWins, cpuWins, mode);

    int floorY = SCRENDY - 1;
    int fighterTopY = floorY - 3;

    drawFighter(player, fighterTopY, 1);
    drawFighter(cpu, fighterTopY, 0);

    screenUpdate();
}

// -----------------------------------------------------------------------------
// Tela do Quiz
// -----------------------------------------------------------------------------

void drawLogicQuizScreen(int timeLeft, int questionId)
{  
    clearGameArea();

    int logicCityY = SCRSTARTY + 7;
    int booleY     = logicCityY + 2;
    int questionY  = booleY + 3;


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

    // ==============================
    // LOGIC CITY (centralizado)
    // ==============================
    screenSetColor(LIGHTMAGENTA, LIGHTCYAN);
    drawCentered(logicCityY, "[ LOGIC CITY ]");


    // ==============================
    // TIMER (centralizado)
    // ==============================
    char timerText[8];
    sprintf(timerText, "%02d", timeLeft);

    int quizCenterX = (SCRSTARTX + SCRENDX) / 2;
    int timerX = quizCenterX - (int)(strlen(timerText) / 2);

    screenSetColor(WHITE, LIGHTCYAN);
    screenGotoxy(timerX, logicCityY - 2); 
    printf("%s", timerText);

    // ==============================
    // GEORGE BOOLE (centralizado)
    // ==============================
    screenSetColor(GREEN, LIGHTCYAN);
    drawCentered(booleY, "== George Boole te da a chance de voltar mais forte ==");

    // ==============================
    // PERGUNTA + FRASE + OPCOES (TODOS CENTRALIZADOS)
    // ==============================
    screenSetColor(WHITE, LIGHTCYAN);

    switch (questionId)
    {
    case 0:
        drawCentered(questionY, "Quais conectivos logicos estao presentes na frase?");
        screenSetColor(YELLOW, LIGHTCYAN);
        drawCentered(questionY + 2, "\"Se chover entao eu nao vou para a praia\"");
        screenSetColor(WHITE, LIGHTCYAN);
        drawCentered(questionY + 4, "[ 0 ] apenas ->");
        drawCentered(questionY + 5, "[ 1 ] apenas ~");
        drawCentered(questionY + 6, "[ 2 ] -> + ~");
        break;

    case 1:
        drawCentered(questionY, "Qual conectivo aparece na frase abaixo?");
        screenSetColor(YELLOW, LIGHTCYAN);
        drawCentered(questionY + 2, "\"Hoje e sabado e esta chovendo\"");
        screenSetColor(WHITE, LIGHTCYAN);
        drawCentered(questionY + 4, "[ 0 ] apenas v");
        drawCentered(questionY + 5, "[ 1 ] apenas ^");
        drawCentered(questionY + 6, "[ 2 ] apenas ->");
        break;

    case 2:
        drawCentered(questionY, "Qual conectivo representa a ideia da frase?");
        screenSetColor(YELLOW, LIGHTCYAN);
        drawCentered(questionY + 2, "\"Ou estudo logica ou reprovo na disciplina\"");
        screenSetColor(WHITE, LIGHTCYAN);
        drawCentered(questionY + 4, "[ 0 ] v");
        drawCentered(questionY + 5, "[ 1 ] ->");
        drawCentered(questionY + 6, "[ 2 ] <->");
        break;

    case 3:
        drawCentered(questionY, "A negacao de P e representada por:");
        screenSetColor(YELLOW, LIGHTCYAN);
        drawCentered(questionY + 2, "\"Nao P\"");
        screenSetColor(WHITE, LIGHTCYAN);
        drawCentered(questionY + 4, "[ 0 ] ~P");
        drawCentered(questionY + 5, "[ 1 ] P -> Q");
        drawCentered(questionY + 6, "[ 2 ] P v Q");
        break;

    case 4:
        drawCentered(questionY, "Qual conectivo representa \"P se, e somente se, Q\"?");
        screenSetColor(YELLOW, LIGHTCYAN);
        drawCentered(questionY + 2, "\"P se, e somente se, Q\"");
        screenSetColor(WHITE, LIGHTCYAN);
        drawCentered(questionY + 4, "[ 0 ] <->");
        drawCentered(questionY + 5, "[ 1 ] ^");
        drawCentered(questionY + 6, "[ 2 ] v");
        break;

    case 5:
    default:
        drawCentered(questionY, "Qual conectivo aparece na frase abaixo?");
        screenSetColor(YELLOW, LIGHTCYAN);
        drawCentered(questionY + 2, "\"Se eu passar no concurso, entao vou viajar\"");
        screenSetColor(WHITE, LIGHTCYAN);
        drawCentered(questionY + 4, "[ 0 ] ~");
        drawCentered(questionY + 5, "[ 1 ] ->");
        drawCentered(questionY + 6, "[ 2 ] <->");
        break;
    }

    screenUpdate();
}


void drawQuizResultScreen(int acertou, int questionId)
{
    clearGameArea();

    // Fundo igual ao do quiz (preto)
    screenSetColor(WHITE, LIGHTCYAN);

    int midX = MAXX / 2 - 22;
    int midY = MAXY / 2;

    if (acertou)
    {

        soundPlayQuizCorrect();

        screenSetColor(GREEN, LIGHTCYAN);
        screenGotoxy(midX, midY);
        printf("✔ RESPOSTA CORRETA! Voce voltou mais forte!");
    }
    else
    {
        soundPlayQuizWrong();

        screenSetColor(RED, LIGHTCYAN);
        screenGotoxy(midX, midY);
        printf("✘ RESPOSTA ERRADA!");

        screenSetColor(WHITE, LIGHTCYAN);
        screenGotoxy(midX, midY + 2);
        printf("Resposta correta: ");

        // Mostra a alternativa correta de acordo com a pergunta
        switch (questionId)
        {
        case 0:
            printf("[ 2 ] -> + ~");
            break;
        case 1:
            printf("[ 1 ] apenas ^");
            break;
        case 2:
            printf("[ 0 ] v");
            break;
        case 3:
            printf("[ 0 ] ~P");
            break;
        case 4:
            printf("[ 0 ] <->");
            break;
        case 5:
        default:
            printf("[ 1 ] ->");
            break;
        }
    }

    screenSetColor(WHITE, LIGHTCYAN);
    screenGotoxy(midX, midY + 4);
    printf("Pressione qualquer tecla para voltar...");

    screenUpdate();

    // Espera qualquer tecla
    while (!keyhit());
    readch();
}


void drawEndScreen(const Fighter *player, const Fighter *cpu)
{
    clearGameArea();

    int result = 0;

    if (player->hp > 0 && cpu->hp <= 0)
        result = 1;
    else if (cpu->hp > 0 && player->hp <= 0)
        result = 2;

    int msgX = MAXX / 2 - 6;
    int msgY = MAXY / 2;

    switch (result)
    {
    case 1:
        soundStopMusic();
        soundPlayWin();

        screenSetColor(LIGHTGREEN, LIGHTCYAN);
        screenGotoxy(msgX, msgY);
        printf("VOCE VENCEU!");
        break;

    case 2:
        soundStopMusic();
        soundPlayLose();

        screenSetColor(LIGHTRED, LIGHTCYAN);
        screenGotoxy(msgX, msgY);
        printf("VOCE PERDEU!");
        break;

    default:
        soundStopMusic();
        screenSetColor(YELLOW, LIGHTCYAN);
        screenGotoxy(msgX, msgY);
        printf("EMPATE!");
        break;
    }

    screenSetColor(WHITE, LIGHTCYAN);
    screenGotoxy(MAXX / 2 - 16, MAXY / 2 + 2);
    printf("Pressione ESC para continuar...");

    screenUpdate();

    int key = 0;
    while (key != 27)
        if (keyhit())
            key = readch();
}


void drawScoreScreen(int score,
                     const char *playerName,
                     int playerWonMatch,
                     int maxHpTotal,
                     int hpTotalFinal,
                     int roundsPlayed,
                     const int roundResults[])
{
    clearGameArea();
    drawBackground();  // mantém o mesmo fundo da luta

    // Caixa mais compacta
    int boxWidth  = 54;
    int boxHeight = 14;

    // >>> Centralizar dentro da ÁREA ÚTIL da arena, não no terminal inteiro
    int areaTop    = SCRSTARTY + 6;      // um pouco abaixo do letreiro
    int areaBottom = SCRENDY - 3;        // um pouco acima da grama
    int areaMidY   = (areaTop + areaBottom) / 2;

    int startX = MAXX / 2 - boxWidth  / 2;
    int startY = areaMidY - boxHeight / 2;

    // borda da caixa
    screenSetColor(LIGHTCYAN, LIGHTCYAN);
    for (int y = 0; y < boxHeight; y++)
    {
        screenGotoxy(startX, startY + y);
        for (int x = 0; x < boxWidth; x++)
        {
            if (y == 0 || y == boxHeight - 1)
                putchar('-');
            else if (x == 0 || x == boxWidth - 1)
                putchar('|');
            else
                putchar(' ');
        }
    }

    // ====== CONTEÚDO CENTRALIZADO ======
    screenSetColor(YELLOW, LIGHTCYAN);
    screenGotoxy(startX + 3, startY + 1);
    if (playerWonMatch)
        printf("RESULTADO: VITORIA");
    else
        printf("RESULTADO: DERROTA");

    screenSetColor(WHITE, LIGHTCYAN);
    screenGotoxy(startX + 3, startY + 3);
    printf("Jogador: %s", playerName ? playerName : "Player");

    screenGotoxy(startX + 3, startY + 4);
    printf("Score final: %d", score);

    // Vida num formato mais compacto
    screenGotoxy(startX + 3, startY + 6);
    printf("Vida: %d / %d (mantida / possivel)", hpTotalFinal, maxHpTotal);

    // Rounds + resultados
    screenGotoxy(startX + 3, startY + 8);
    printf("Rounds jogados: %d", roundsPlayed);

    screenSetColor(CYAN, LIGHTCYAN);
    screenGotoxy(startX + 3, startY + 9);
    printf("Resultado por round:");

    // cabem até 3 linhas de rounds dentro da caixa
    int maxLinesRounds = 3;
    int roundsToShow = roundsPlayed;
    if (roundsToShow > maxLinesRounds)
        roundsToShow = maxLinesRounds;

    for (int i = 0; i < roundsToShow; i++)
    {
        const char *texto = "Empate";
        if (roundResults[i] == 1)
            texto = "Vitoria do jogador";
        else if (roundResults[i] == 2)
            texto = "Vitoria do bot";

        screenGotoxy(startX + 5, startY + 10 + i);
        printf("Round %d: %s", i + 1, texto);
    }

    // instrução
    screenSetColor(LIGHTMAGENTA, LIGHTCYAN);
    screenGotoxy(startX + 13, startY + boxHeight - 1);
    printf("Pressione ESC para sair...");

    screenUpdate();

    int key = 0;
    while (key != 27)
        if (keyhit())
            key = readch();
}

