#ifndef SPACEINVADERS_H
#define SPACEINVADERS_H

#include <allegro5/allegro.h>
#include <stdbool.h>

// Estruturas
typedef struct {
    float x, y;
    int vidas;
} Jogador;

typedef struct {
    float x, y;
    bool vivo;
} Inimigo;

typedef struct {
    float x, y;
    bool ativo;
} Tiro;

// Funções do jogo
bool inicializar_jogo(Jogador *jogador, Inimigo inimigos[], int num_inimigos);
void desenhar_jogo(Jogador jogador, Inimigo inimigos[], int num_inimigos, Tiro tiros[], int max_tiros);
bool carregar_imagens();
void mover_jogador(Jogador *jogador, int direcao);
void atirar(Jogador jogador, Tiro tiros[], int max_tiros);
void atualizar_inimigos(Inimigo inimigos[], int num_inimigos, float *velocidade);
void atualizar_tiros(Tiro tiros[], int max_tiros, float velocidade);
bool verificar_colisao(Tiro tiros[], Inimigo inimigos[], int num_inimigos, int max_tiros);
void liberar_imagens();
#endif
