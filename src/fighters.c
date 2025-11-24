#include "fighters.h"
#include "lib/keyboard.h" // keyhit, readch

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

void updateCPU(Fighter *cpu, Fighter *player) {
    int dist = player->x - cpu->x; // pode ser negativo

    if (abs(dist) > MIN_DISTANCE) {
        // se está longe, aproxima
        if (dist > 0) {
            cpu->x++;
            cpu->facing = 1;
        } else {
            cpu->x--;
            cpu->facing = -1;
        }
    } else {
        // perto o suficiente: tenta atacar às vezes
        if (!cpu->attacking && rand() % 15 == 0) {
            startAttack(cpu);
        }
    }
}