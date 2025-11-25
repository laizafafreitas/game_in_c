#include "fighters.h"
#include "lib/keyboard.h" // keyhit, readch
#include <stdlib.h>       // rand, abs

void initFighter(Fighter *f, int startX, int facing) {
    f->x            = startX;
    f->hp           = MAX_HP;
    f->facing       = facing;
    f->attacking    = 0;
    f->attack_timer = 0;
}

void startAttack(Fighter *f) {
    if (!f->attacking) {
        f->attacking    = 1;
        f->attack_timer = ATTACK_DURATION;
    }
}

void updateAttack(Fighter *attacker, Fighter *defender) {
    if (!attacker->attacking) return;

    // Aplica dano apenas no primeiro frame do ataque
    if (attacker->attack_timer == ATTACK_DURATION) {
        if (abs(attacker->x - defender->x) <= ATTACK_RANGE) {
            defender->hp -= DAMAGE;
        }
    }

    attacker->attack_timer--;
    if (attacker->attack_timer <= 0) {
        attacker->attacking    = 0;
        attacker->attack_timer = 0;
    }
}

void handlePlayerInput(int *running, Fighter *player) {
    if (!keyhit()) return;

    int k = readch();

    if (k == 'q' || k == 'Q') {
        *running = 0;
        return;
    }

    if (k == 'a' || k == 'A') {
        player->x--;
        player->facing = -1;
    } else if (k == 'd' || k == 'D') {
        player->x++;
        player->facing = 1;
    } else if (k == 'j' || k == 'J') {
        startAttack(player);
    }
}

/*
 * IA do bot:
 * - Anda em direção ao player normalmente.
 * - Às vezes recua quando toma dano (chance aleatória).
 * - Quando está perto, ataca com mais frequência.
 */
void updateCPU(Fighter *cpu, Fighter *player) {
    // variáveis estáticas: lembram o valor entre chamadas
    static int last_hp       = -1;
    static int retreat_timer = 0;

    // inicializa last_hp na primeira vez
    if (last_hp == -1) {
        last_hp = cpu->hp;
    }

    // Detecta se tomou dano neste frame
    if (cpu->hp < last_hp) {
        // tomou dano → às vezes recua
        if (rand() % 2 == 0) {   
            retreat_timer = 10; 
        }
    }
    last_hp = cpu->hp;

    int dist = player->x - cpu->x;

    // 1) RECUAR SE PLAYER ESTÁ NA MESMA POSIÇÃO (ENCOSTOU)
    if (player->x == cpu->x) {
        if (rand() % 2 == 0) { 
            if (dist > 0) {
                cpu->x--;        // player à direita → bot vai pra esquerda
                cpu->facing = -1;
            } else {
                cpu->x++;        // player à esquerda → bot vai pra direita
                cpu->facing = 1;
            }
            return;
        }
    }

    // 2) RECUO DE DANO (RETREAT MODE)
    if (retreat_timer > 0) {
        if (dist > 0) {
            cpu->x--;
            cpu->facing = -1;
        } else {
            cpu->x++;
            cpu->facing = 1;
        }
        retreat_timer--;
        return;
    }

    // 3) AVANÇAR NORMALMENTE SE ESTIVER LONGE
    if (abs(dist) > MIN_DISTANCE) {
        if (dist > 0) {
            cpu->x++;
            cpu->facing = 1;
        } else {
            cpu->x--;
            cpu->facing = -1;
        }
    }
    else {
        // 4) PERTO O SUFICIENTE: ATACAR MAIS VEZES
        if (!cpu->attacking && rand() % 6 == 0) { 
            startAttack(cpu);
        }
    }
}

