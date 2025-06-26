#ifndef SPACEINVADERS_H
#define SPACEINVADERS_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_font.h>
#include<allegro5/allegro_audio.h>
#include <stdbool.h>


typedef struct {
    float x, y;
    int vidas;
    int pontuacao;
} Jogador;

typedef struct {
    float x, y;
    bool vivo;
} Inimigo;

typedef struct {
    float x, y;
    bool ativo;
} Tiro;

extern int recorde;
extern ALLEGRO_SAMPLE *musica_fundo;
extern ALLEGRO_SAMPLE *som_tiro;
extern ALLEGRO_SAMPLE *som_explosao;
extern ALLEGRO_SAMPLE_INSTANCE *instancia_musica;

// Funções do jogo
bool inicializar_jogo(Jogador *jogador, Inimigo inimigos[], int num_inimigos);
bool carregar_imagens();
void mover_jogador(Jogador *jogador, int direcao);
void atirar(Jogador jogador, Tiro tiros[], int max_tiros);
void atualizar_inimigos(Inimigo inimigos[], int num_inimigos, float *velocidade);
void atualizar_tiros(Tiro tiros[], int max_tiros, float velocidade);
bool verificar_colisao(Tiro tiros[], Inimigo inimigos[], int num_inimigos, int max_tiros, Jogador *jogador);
void liberar_imagens();
void atualizar_recorde(Jogador *jogador);
void salvar_recorde();
void carregar_recorde();
void desenhar_jogo(Jogador jogador, Inimigo inimigos[], int num_inimigos, Tiro tiros[], int max_tiros, ALLEGRO_FONT* fonte);
bool carregar_sons();
void tocar_musica();
void parar_musica();
void liberar_sons();
#endif
