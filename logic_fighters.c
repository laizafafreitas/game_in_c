#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define FPS 30
#define MAX_HP 100
#define ATTACK_RANGE 2
#define DAMAGE 10
#define MIN_DISTANCE 2
#define ATTACK_DURATION 6

typedef struct {
    int x;
    int hp;
    int facing;
    int attacking;
    int attack_timer;
} Fighter;

void clearGameArea() {
    for (int i = MINX+1; i < MAXX-1; i++) {
        for(int j = MINY+1; j < MAXY; j++){
            screenGotoxy(i, j);
            printf(" ");
        }
    }
}

int clamp(int v, int a, int b){
    return v<a ? a : (v > b ? b: v);
}

void drawHealthBar(int x, int y, int hp){
    int width = 20;
    if (hp < 0) hp = 0;
    int filled = hp * width / MAX_HP;

    screenGotoxy(x,y);
    printf("[");
    for (int i = 0; i < width; i++){
        printf(i < filled ? "█" : " ");
    }
    printf("] %3d", hp);
}

int main() 
{
    srand((unsigned)time(NULL));

    screenInit(1);
    keyboardInit();
    timerInit(1000 / FPS);

    Fighter player = { SCRSTARTX + 5, MAX_HP, 1, 0 };
    Fighter cpu    = { SCRENDX - 5,   MAX_HP, -1, 0 };

    int running = 1;
    while (running) 
    {
        // ---------- INPUT ----------
        if (keyhit()) 
        {
            int k = readch();
            if (k == 'q' || k == 'Q') running = 0;

            if (k == 'a' || k == 'A') {
                player.x--;
                player.facing = -1;
            }
            if (k == 'd' || k == 'D') {
                player.x++;
                player.facing = 1;
            }
            if (k == 'j' || k == 'J') {
                if(!player.attacking){
                    player.attacking = 1;
                    player.attack_timer = ATTACK_DURATION;
                }

            }
        }

        player.x = clamp(player.x, SCRSTARTX+1, SCRENDX-1);

        // ---------- UPDATE / TICK ----------
        if (timerTimeOver())
        {   

            // CPU simples: anda na direção do player
            int dist = player.x - cpu.x;        // pode ser negativo

            if (abs(dist) > MIN_DISTANCE) {
                // ainda está longe → pode se aproximar
                if (dist > 0) {
                    cpu.x++;
                    cpu.facing = 1;
                } else {
                    cpu.x--;
                    cpu.facing = -1;
                }
            } else {
               // CPU ataca às vezes
                if (!cpu.attacking && rand() % 15 == 0) {
                    cpu.attacking = 1;
                    cpu.attack_timer = ATTACK_DURATION;
                }
            }

            // PLAYER
            if (player.attacking) {
                // primeiro frame do ataque
                if (player.attack_timer == ATTACK_DURATION) {
                    if (abs(player.x - cpu.x) <= ATTACK_RANGE) {
                        cpu.hp -= DAMAGE;
                    }
                }

                player.attack_timer--;
                if (player.attack_timer <= 0) {
                    player.attacking    = 0;
                    player.attack_timer = 0;
                }
            }

            // CPU
            if (cpu.attacking) {
                // primeiro frame do ataque
                if (cpu.attack_timer == ATTACK_DURATION) {
                    if (abs(cpu.x - player.x) <= ATTACK_RANGE) {
                        player.hp -= DAMAGE;
                    }
                }

                cpu.attack_timer--;
                if (cpu.attack_timer <= 0) {
                    cpu.attacking    = 0;
                    cpu.attack_timer = 0;
                }
            }


            // fim de jogo?
            if (player.hp <= 0 || cpu.hp <= 0) {
                running = 0;
            }

            // ---------- DRAW ----------
            
            // barras de vida
            drawHealthBar(SCRSTARTX + 1, SCRSTARTY + 2, player.hp);
            drawHealthBar(SCRENDX - 24, SCRSTARTY + 2, cpu.hp);

            // desenha player
            screenGotoxy(player.x, SCRENDY-2);
            printf(player.attacking ? "O-<" : " O ");

            //desenha o piso

            screenGotoxy(SCRSTARTX-1, SCRENDY -1);
            for(int i = 0; i < MAXX-3; i++){
                printf("=");
            }

            // desenha cpu
            screenGotoxy(cpu.x, SCRENDY-2);
            printf(cpu.attacking ? ">-O" : " O ");

            screenGotoxy(SCRSTARTX + 1, SCRENDY);
            printf("[A/D] mover  [J] atacar  [Q] sair");

            screenUpdate();

        }
    }

    // tela final
    clearGameArea();
    screenGotoxy(SCRSTARTX + 5, SCRSTARTY + 5);
    if (player.hp > 0 && cpu.hp <= 0) {
        screenGotoxy(MAXX/2, MAXY/2);
        printf("VOCE VENCEU!");
    } else if (cpu.hp > 0 && player.hp <= 0) {
        screenGotoxy(MAXX/2, MAXY/2);
        printf("VOCE PERDEU!");
    } else {
        printf("EMPATE!");
    }
    screenUpdate();

    while (!keyhit()) { /* espera */ }
    (void)readch();

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}
