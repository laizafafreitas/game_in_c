# 🧠⚔️ Logic Fighters — Jogo em C (Cesar School)

Logic Fighters é um jogo de luta em ambiente de terminal desenvolvido inteiramente em **linguagem C**, unindo:

- Combate 2D no estilo arena  
- Quiz de **Lógica Proposicional**  
- ASCII art  
- Timer de frames  
- Som ambiente e efeitos sonoros  
- Modos **VS CPU** e **Multiplayer local**  
- Sistema de pontuação e tela final detalhada

O jogo foi criado como projeto final das disciplinas:

**Programação Imperativa e Funcional**  
**Lógica de Programação**  
**Cesar School — 2025.2**

Este projeto demonstra como conceitos fundamentais de C e lógica podem ser integrados em uma experiência lúdica, funcional e tecnicamente desafiadora.

---

## 📄 Documentação Oficial
Você pode acessar o documento completo (história, mecânica, arquitetura, sprites, sistemas internos, IA, HUD e muito mais) aqui:

📘 **Documentação do Projeto →** _(https://docs.google.com/document/d/19NdZ7VmI4WeWYiQTrc6tk05LmSTjHCnD3u0-ou4MG3o/edit?usp=sharing)_

---

# 🧩 1. Contexto e Proposta

O objetivo acadêmico era unir:

- **conceitos de lógica proposicional** (v, ^, ~, →, ↔),
- **modelagem de sistemas**,  
- **game loop**,  
- **modularização em C**,  
- **ponteiros, timers, entrada simultânea**,  
- **renderização ASCII**,  
- e **criatividade visual/sonora**.

Nasce, assim, **Logic Fighters**, uma arena futurista onde lutar exige raciocínio:  
**vencer a batalha depende tanto de seus golpes quanto do seu domínio lógico.**


---

# 🏛️ 3. Estrutura do Projeto
```
/game_in_c
├── assets/
│   └── sounds/              # Arquivos .wav/.mp3 usados para música e efeitos sonoros
├── build/
├── include/                 # Arquivos de cabeçalho (.h)
│   ├── lib/                 # Biblioteca CLI-Lib usada para render, timer e teclado
│   │
│   ├── screens/             # Headers das telas principais
│   │   ├── game.h           # Interface da tela de luta e controle de rounds
│   │   ├── menu.h           # Menu principal (VS CPU, VS Player, Tutorial)
│   │   ├── quiz.h           # Tela do quiz e lógica de respostas
│   │   └── tutorial.h       # Tela de tutorial do jogo
│   │
│   ├── config.h             # Constantes globais: tamanhos, HP, tempo do round etc.
│   ├── fighters.h           # Estrutura Fighter + funções de movimento/ataque
│   ├── render.h             # Funções de desenho, HUD, sprites, fundos
│   ├── sound.h              # Sistema de trilha sonora e efeitos sonoros
│   └── utils.h              # Funções auxiliares (ex: clamp)
│
├── src/                     # Implementações (.c)
│   ├── lib/                 # CLI-Lib
│   │
│   ├── screens/             # Telas do jogo
│   │   ├── game.c           # Game loop, lógica dos rounds, modos de jogo
│   │   ├── menu.c           # Menu e navegação principal
│   │   ├── quiz.c           # Sistema do quiz: perguntas, respostas, resultado
│   │   └── tutorial.c       # Tela de instruções para o jogador
│   │
│   ├── fighters.c           # Mecânica dos lutadores: ataque, movimento, update
│   ├── main.c               # Função principal, inicializações e início do menu
│   ├── render.c             # HUD, sprites ASCII, background e centralizações
│   ├── sound.c              # Sistema de som via chamadas ao sistema
│   └── utils.c              # Funções auxiliares
│
├── README.md                # Documentação do projeto
├── Makefile                 # Regras para compilar, limpar e executar o jogo
└── .gitignore               # Arquivos ignorados pelo Git

```

---

# 🔊 4. Sistema de Som

O jogo possui uma trilha completa:

- 🎵 **Música de batalha**  
- 🎵 **Música da tela de score**  
- 🔊 **Som de ataque**  
- 🔊 **Acerto de quiz**  
- 🔇 **Erro de quiz**  
- 🎶 **Vitória / Derrota**

As músicas são trocadas dinamicamente com:

```c
soundPlayFightMusic();
soundStopMusic();
soundPlayScoreMusic();
soundPlayWin();
soundPlayLose();
```
Efeitos sonoros são disparados por:
```c
soundPlayAttack();
soundPlayQuizCorrect();
soundPlayQuizWrong();
```
---
# ⚔️ 5. Mecânica do Jogo — Logic Fighters

## 🧍‍♂️ 5.1 Personagens

Existem sempre dois combatentes:

- **Player 1**  
- **CPU** ou **Player 2** (dependendo do modo escolhido)

Cada lutador possui um conjunto de atributos fundamentais:

| Atributo        | Descrição |
|-----------------|-----------|
| `x`             | Posição horizontal na arena |
| `hp`            | Pontos de vida (HP máximo definido em `MAX_HP`) |
| `facing`        | Direção para onde o personagem está virado — `1` (direita) ou `-1` (esquerda) |
| `attacking`     | Indica se o lutador está atacando |
| `attack_timer`  | Temporizador interno que controla duração e janela de dano do ataque |

---

## 🎮 5.2 Controles

### 🟦 Player 1
| Tecla | Ação                   |
|-------|-------------------------|
| **A** | mover para a esquerda  |
| **D** | mover para a direita   |
| **F** | atacar                 |
| **Q** | sair da luta           |

### 🟥 Player 2 (modo multiplayer)
| Tecla | Ação                   |
|-------|-------------------------|
| **J** | mover para a esquerda  |
| **K** | mover para a direita   |
| **P** | atacar                 |
| **Q** | sair da luta           |

## 🥊 5.3 Sistema de Ataques
Os ataques são calculados com base em regras simples, porém estratégicas:

- Cada golpe dura `ATTACK_DURATION` frames.
- **Só causa dano no primeiro frame** do ataque.
- O dano ocorre **apenas** se a distância horizontal entre os lutadores for menor ou igual ao valor constante: **ATTACK_RANGE**
- O ataque dispara som (`soundPlayAttack()`).

**💥 Dano aplicado**
- Dano padrão: `DAMAGE = 10`
- Dano buffado (após acertar o quiz): **15**

## ⏱️ 5.4 Sistema de Tempo

O jogo possui um timer de luta:

- `ROUND_TIME` segundos por round.
- O game loop decrementa o timer a cada `FPS` frames.
- Caso o tempo esgote antes de um KO, vence o lutador com mais HP.

## 🏆 5.5 Rounds e Vitória

O jogo segue o formato clássico **melhor de 3**:

- Quem conseguir **2 vitórias de round** vence.
- Entre rounds, lutadores têm HP restaurado e posições reiniciadas.
- Os resultados são armazenados em um array:  

---

# 📊 6. Sistema de Pontuação
A pontuação existe apenas no modo VS CPU, calculada da seguinte forma:

| Regra                        | Valor                  |
| ---------------------------- | ---------------------- |
| Score inicial                | **1000**               |
| Penalidade por HP perdido    | **-5 por ponto de HP** |
| Penalidade por segundo gasto | **-2 por segundo**     |
| Score mínimo                 | **0**                  |

A tela final exibe:
- Nome do jogador
- Score final
- Vida total mantida
- Tempo total gasto
- Resultado por round
- Música especial de vitória/derrota

---

# 🚀 7. Como Rodar

1. Compilar
```go
make
```
2. Executar
```go
make run
```
3. Limpar Arquivos
```go
make clean
```
---
# 📌 8. Trello do Projeto
🔗 https://trello.com/invite/b/69154931cc6d47d05b043b7d/ATTI071937ecc9ce459194b39c0acfb8038866A2A8C7/kanban-jogo-em-c

---
## 👥 9. Equipe
Projeto desenvolvido pelos estudantes da **Cesar School**:

- **Marcello Augusto - Líder** · [LinkedIn](https://www.linkedin.com/in/marcelloaugustosv/)
- **Luiza Vieira** · [LinkedIn](https://www.linkedin.com/in/vbluuiza)
- **Luan Guedes** · [Github](https://github.com/RyuuNoTenshi)
- **Lucca Spinelli** · [LinkedIn](https://www.linkedin.com/in/lucca-spinelli-a65672240/)
- **Eliziane Mota** · [LinkedIn](https://www.linkedin.com/in/eliziane-mota/)
- **Laíza Freitas** · [LinkedIn](https://www.linkedin.com/in/laizafreitas/)

**Professores:**  
- Diego de Freitas Bezerra  
- Guilherme Fernando
