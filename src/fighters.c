#include "fighters.h"
#include "lib/keyboard.h"
#include "lib/screen.h"
#include "utils.h"

#include <stdlib.h>
#include <math.h>

// -----------------------------------------------------------------------------
// Inicialização do lutador
// -----------------------------------------------------------------------------

void initFighter(Fighter *f, int startX, Facing facing)
{
    f->x            = startX;
    f->hp           = MAX_HP;
    f->facing       = facing;
    f->attacking    = 0;
    f->attack_timer = 0;
}

// -----------------------------------------------------------------------------
// Início do ataque
// -----------------------------------------------------------------------------

void startAttack(Fighter *f)
{
    if (!f->attacking)
    {
        f->attacking    = 1;
        f->attack_timer = ATTACK_DURATION;
    }
}

// -----------------------------------------------------------------------------
// Atualização do ataque (diminui timer + aplica dano no 1º frame)
// -----------------------------------------------------------------------------

void updateAttack(Fighter *attacker, Fighter *defender)
{
    if (!attacker->attacking) return;

    // Só causa dano no frame inicial do ataque
    if (attacker->attack_timer == ATTACK_DURATION)
    {
        if (abs(attacker->x - defender->x) <= ATTACK_RANGE)
        {
            defender->hp -= DAMAGE;
        }
    }

    attacker->attack_timer--;

    if (attacker->attack_timer <= 0)
    {
        attacker->attacking    = 0;
        attacker->attack_timer = 0;
    }
}

// -----------------------------------------------------------------------------
// Input do jogador
// -----------------------------------------------------------------------------

void handlePlayerInput(int *running, Fighter *player)
{
    if (!keyhit()) return;

    int k = readch();

    // sair da luta
    if (k == 'q' || k == 'Q')
    {
        *running = 0;
        return;
    }

    // mover
    if (k == 'a' || k == 'A')
    {
        player->x--;
        player->facing = FACING_LEFT;
    }
    else if (k == 'd' || k == 'D')
    {
        player->x++;
        player->facing = FACING_RIGHT;
    }
    // atacar
    else if (k == 'j' || k == 'J')
    {
        startAttack(player);
    }

    // limita posição dentro da arena
    player->x = clamp(player->x, SCRSTARTX + 1, SCRENDX - 1);
}

// -----------------------------------------------------------------------------
// IA DO BOT
// -----------------------------------------------------------------------------

/*
 * IA do bot:
 * - Anda em direção ao player normalmente.
 * - Às vezes recua quando toma dano (chance aleatória).
 * - Quando está perto, ataca com alguma chance.
 */
void updateCPU(Fighter *cpu, Fighter *player)
{
    // variáveis que persistem entre frames (somente para o BOT)
    static int last_hp       = -1;
    static int retreat_timer = 0;

    // inicializa na primeira chamada
    if (last_hp == -1)
        last_hp = cpu->hp;

    int dist = player->x - cpu->x;

    // direção que o bot está olhando
    cpu->facing = (dist > 0 ? FACING_RIGHT : FACING_LEFT);

    // --- DETECTA DANO ---
    if (cpu->hp < last_hp)
    {
        // 50% de chance de entrar em "modo recuo" ao tomar dano
        if (rand() % 2 == 0)
            retreat_timer = 10;
    }
    last_hp = cpu->hp;

    // --- 1) ENCOSTOU NO PLAYER ---
    if (player->x == cpu->x)
    {
        if (rand() % 2 == 0)
        {
            // afasta um passo na direção oposta
            cpu->x += (dist > 0 ? -1 : 1);
            cpu->facing = (dist > 0 ? FACING_LEFT : FACING_RIGHT);
            cpu->x = clamp(cpu->x, SCRSTARTX + 1, SCRENDX - 1);
            return;
        }
    }

    // --- 2) MODO RECUAR (APÓS TOMAR DANO) ---
    if (retreat_timer > 0)
    {
        // anda na direção contrária ao player
        cpu->x += (dist > 0 ? -1 : 1);
        retreat_timer--;

        cpu->x = clamp(cpu->x, SCRSTARTX + 1, SCRENDX - 1);
        return;
    }

    // --- 3) LONGE: APROXIMAR ---
    if (abs(dist) > MIN_DISTANCE)
    {
        cpu->x += (dist > 0 ? 1 : -1);
        cpu->x = clamp(cpu->x, SCRSTARTX + 1, SCRENDX - 1);
    }
    else
    {
        // --- 4) PERTO: ATACAR COM ALGUMA CHANCE ---
        if (!cpu->attacking && rand() % 6 == 0)
            startAttack(cpu);
    }
}
