#ifndef FIGHTERS_H
#define FIGHTERS_H

// Constantes de jogo relacionadas aos lutadores
#define MAX_HP          100
#define ATTACK_RANGE    4
#define DAMAGE          10
#define ATTACK_DURATION 10
#define MIN_DISTANCE    4
#define DEFENSE_MULTIPLIER 0.4

// Direção que o lutador está virado
typedef enum {
    FACING_LEFT  = -1,
    FACING_RIGHT =  1
} Facing;

// Estrutura básica de um lutador
typedef struct {
    int x;              // posição horizontal na arena
    int hp;             // pontos de vida
    Facing facing;      // direção
    int attacking;      // 1 se está atacando, 0 caso contrário
    int attack_timer;   // frames restantes do ataque atual
    int defending;
} Fighter;

// API pública
void initFighter(Fighter *f, int startX, Facing facing);
void startAttack(Fighter *f);
void updateAttack(Fighter *attacker, Fighter *defender, int damage);
void handlePlayerInput(int *running, Fighter *player, Fighter *cpu);
void updateCPU(Fighter *cpu, Fighter *player);

#endif