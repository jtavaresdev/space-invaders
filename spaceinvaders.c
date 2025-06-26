#include "spaceinvaders.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <math.h>
#include <stdio.h>
#define LARGURA_TELA 800
#define ALTURA_TELA 600

ALLEGRO_BITMAP *jogador_img = NULL;
ALLEGRO_BITMAP *inimigo_img = NULL;


bool inicializar_jogo(Jogador *jogador, Inimigo inimigos[], int num_inimigos){
	
	jogador->x = LARGURA_TELA / 2;
	jogador->y = ALTURA_TELA - 50;
	jogador->vidas = 3;
	
	for (int i = 0; i < num_inimigos; i ++){
		// inimigos na mesma 'linha'
		inimigos[i].x = 50 + (i % 10) * 60;
		// inimigos na 'coluna'
		inimigos[i].y = 50 + (i / 10) * 40;
		inimigos[i].vivo = true;
	}

	return true;
}

bool carregar_imagens(){
	jogador_img = al_load_bitmap("./images/Ship_1.png");
	inimigo_img = al_load_bitmap("./images/Ship_2.png");
	
	if (!jogador_img || !inimigo_img){
		printf("Erro  ao carregar imagens\n");
		return false;
	}
	return true;
}

void desenhar_jogo(Jogador jogador, Inimigo inimigos[], int num_inimigos, Tiro tiros[], int max_tiros){

	al_clear_to_color(al_map_rgb(0, 0, 0));
	
	al_draw_bitmap(jogador_img,
		jogador.x - 16,
		jogador.y - 16,
		0);
	
	for (int i = 0; i < num_inimigos; i++) {
   	 if (inimigos[i].vivo) {
        	al_draw_bitmap(
          	  inimigo_img,
          	  inimigos[i].x - 16,
         	   inimigos[i].y - 16,
        	    0);
  		  }
	}
	for (int i = 0; i < max_tiros; i++) {
        if (tiros[i].ativo) {
            al_draw_filled_rectangle(
                tiros[i].x - 2, tiros[i].y - 8,
                tiros[i].x + 2, tiros[i].y + 8,
                al_map_rgb(255, 255, 255)
            );
        }
    }
	al_flip_display();
}

void mover_jogador(Jogador *jogador, int direcao) {
	jogador->x += direcao * 5;

	if (jogador->x < 20) jogador->x = 20;
	if (jogador->x > LARGURA_TELA - 20) jogador->x = LARGURA_TELA - 20;

}

void atirar(Jogador jogador, Tiro tiros[], int max_tiros) {
    for (int i = 0; i < max_tiros; i++) {

	if (tiros[i].ativo) return;
        if (!tiros[i].ativo) {
            tiros[i].x = jogador.x;
            tiros[i].y = jogador.y - 20;
            tiros[i].ativo = true;
            break;
        }
    }
}

void atualizar_inimigos(Inimigo inimigos[], int num_inimigos, float *velocidade){
    static bool move_direita = true;
    static bool deve_descer = false;  // Controla quando os inimigos devem descer
    float nova_velocidade = *velocidade;

    for (int i = 0; i < num_inimigos; i++) {
        if (inimigos[i].vivo) {
            if (move_direita) {
                inimigos[i].x += nova_velocidade;
                // Verifica se algum inimigo atingiu a borda direita
                if (inimigos[i].x > LARGURA_TELA - 30) {
                    deve_descer = true;
                }
            } else {
                inimigos[i].x -= nova_velocidade;
                // Verifica se algum inimigo atingiu a borda esquerda
                if (inimigos[i].x < 30) {
                    deve_descer = true;
                }
            }
        }
    }

    // Se os inimigos devem descer
    if (deve_descer) {
        for (int i = 0; i < num_inimigos; i++) {
            if (inimigos[i].vivo) {
                inimigos[i].y += 20;  // Desce 20 pixels
            }
        }
        move_direita = !move_direita;  // Inverte a direção
        deve_descer = false;           // Reseta o flag
        *velocidade += 0.2f;           // Aumenta a velocidade (dificuldade progressiva)
    }
}

void atualizar_tiros(Tiro tiros[], int max_tiros, float velocidade) {
    for (int i = 0; i < max_tiros; i++) {
        if (tiros[i].ativo) {
            tiros[i].y -= velocidade;
            if (tiros[i].y < 0) tiros[i].ativo = false;
        }
    }
}

bool verificar_colisao(Tiro tiros[], Inimigo inimigos[], int num_inimigos, int max_tiros) {
    for (int i = 0; i < num_inimigos; i++) {
        if (inimigos[i].vivo) {
            for (int j = 0; j < max_tiros; j++) {
                if (tiros[j].ativo) {
                    float dx = tiros[j].x - inimigos[i].x;
                    float dy = tiros[j].y - inimigos[i].y;
                    float distancia = sqrt(dx * dx + dy * dy);

                    if (distancia < 20) { // Raio de colisão
                        inimigos[i].vivo = false;
                        tiros[j].ativo = false;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void liberar_imagens() {
    al_destroy_bitmap(jogador_img);
    al_destroy_bitmap(inimigo_img);
}

