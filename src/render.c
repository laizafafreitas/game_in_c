#include "screen.h"
#include "fighters.h"
#include "keyboard.h"

static const char *PLAYER_IDLE_LEFT[3] = {
    " O  ",
    "/|>",
    "/ \\"
};

// Player parado (virado pra esquerda)
static const char *PLAYER_IDLE_RIGHT[3] = {
    " O  ",
    "<|\\",
    "/ \\"
};

// Player atacando para a direita
static const char *PLAYER_ATTACK_RIGHT[3] = {
    " O ",
    "<|‾‾",
    "/ \\"
};

// Player atacando para a esquerda
static const char *PLAYER_ATTACK_LEFT[3] = {
    "  O ",
    "‾‾|>",
    " / \\"
};

// Bot parado (virado pra direita)
static const char *BOT_IDLE_RIGHT[3] = {
    " @  ",
    "<|\\",
    "/ \\"
};

// Bot parado (virado pra esquerda)
static const char *BOT_IDLE_LEFT[3] = {
    " @  ",
    "/|>",
    "/ \\"
};

// Bot atacando para a direita
static const char *BOT_ATTACK_LEFT[3] = {
    "  @ ",
    "‾‾|>",
    " / \\"
};

// Bot atacando para a esquerda
static const char *BOT_ATTACK_RIGHT[3] = {
    " @  ",
    "<|‾‾",
    "/ \\ "
};

static void drawSprite(int x, int topY, const char *sprite[3]) {
    for (int i = 0; i < 3; i++) {
        screenGotoxy(x, topY + i);
        printf("%s", sprite[i]);
    }
}

void drawFloor(void) {
    screenGotoxy(SCRSTARTX - 1, SCRENDY - 1);
    for (int i = 0; i < MAXX - 3; i++) {
        printf("=");
    }
}

void drawHealthBar(int x, int y, int hp) {
    int width = 20;
    if (hp < 0) hp = 0;
    if (hp > MAX_HP) hp = MAX_HP;

    int filled = (hp * width) / MAX_HP;

    screenGotoxy(x, y);
    printf("[");
    for (int i = 0; i < width; i++) {
        printf(i < filled ? "█" : " ");
    }
    printf("] %3d", hp);
}

void drawTimer(int x, int y, int timeLeft) {
    screenGotoxy(x, y);
    printf("TIME: %02d", timeLeft);  // imprime como 70, 08, 07, etc
}

void drawFighter(const Fighter *f, int topY, int isPlayer) {
    const char **sprite = NULL;
    int facingRight = (f->facing == 1);

    if (f->attacking) {
        if (isPlayer) {
            sprite = facingRight ? PLAYER_ATTACK_RIGHT : PLAYER_ATTACK_LEFT;
        } else {
            sprite = facingRight ? BOT_ATTACK_RIGHT : BOT_ATTACK_LEFT;
        }
    } else {
        if (isPlayer) {
            sprite = facingRight ? PLAYER_IDLE_RIGHT : PLAYER_IDLE_LEFT;
        } else {
            sprite = facingRight ? BOT_IDLE_RIGHT : BOT_IDLE_LEFT;
        }
    }

    drawSprite(f->x, topY, sprite);
}

void drawHUD(const Fighter *player, const Fighter *cpu, int timeLeft) {
    drawHealthBar(SCRSTARTX + 1, SCRSTARTY + 2, player->hp);
    drawHealthBar(SCRENDX - 24, SCRSTARTY + 2, cpu->hp);

    // Timer no centro da parte de cima
    int centerX = (SCRSTARTX + SCRENDX) / 2 - 4;
    drawTimer(centerX, SCRSTARTY + 1, timeLeft);

    drawFloor();

    screenGotoxy(SCRSTARTX + 1, SCRENDY);
    printf("[A/D] mover  [J] atacar  [Q] sair");
}

void drawGame(const Fighter *player, const Fighter *cpu, int timeLeft) {
    clearGameArea();
    drawHUD(player, cpu, timeLeft);

    // y dos lutadores (linha "do chão")
    int floorY = SCRENDY - 1;
    
    // topo do sprite (5 linhas de altura → sobe 4)
    int fighterTopY = floorY - 3;

    drawFighter(player, fighterTopY, 1);
    drawFighter(cpu,    fighterTopY, 0);

    screenUpdate();
}

void drawEndScreen(const Fighter *player, const Fighter *cpu) {
    clearGameArea();

    screenGotoxy(MAXX / 2 - 6, MAXY / 2);
    if (player->hp > 0 && cpu->hp <= 0) {
        printf("VOCE VENCEU!");
    } else if (cpu->hp > 0 && player->hp <= 0) {
        printf("VOCE PERDEU!");
    } else {
        printf("EMPATE!");
    }

    screenGotoxy(MAXX / 2 - 12, MAXY / 2 + 2);
    printf("Pressione qualquer tecla...");

    screenUpdate();

    while (!keyhit()) { /* espera */ }
    (void)readch();
}
