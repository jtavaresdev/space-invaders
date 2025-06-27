#include "spaceinvaders.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <math.h>
#include <stdio.h>
#define LARGURA_TELA 800
#define ALTURA_TELA 600

ALLEGRO_BITMAP *jogador_img = NULL;
ALLEGRO_BITMAP *inimigo_img = NULL;
ALLEGRO_FONT *fonte = NULL;
ALLEGRO_SAMPLE *musica_fundo = NULL;
ALLEGRO_SAMPLE *som_tiro = NULL;
ALLEGRO_SAMPLE *som_explosao = NULL;
ALLEGRO_SAMPLE_INSTANCE *instancia_musica = NULL;

int recorde = 0;

bool inicializar_jogo(Jogador *jogador, Inimigo inimigos[], int num_inimigos){
	
	jogador->x = LARGURA_TELA / 2;
	jogador->y = ALTURA_TELA - 50;
	jogador->vidas = 3;
	jogador->pontuacao = 0;
	
	for (int i = 0; i < num_inimigos; i ++){
		// inimigos na mesma 'linha'
		inimigos[i].x = 50 + (i % 5) * 60;
		// inimigos na 'coluna'
		inimigos[i].y = 50 + (i / 5) * 40;
		inimigos[i].vivo = true;
	}

	return true;
}

bool carregar_sons(){
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(3); 

    musica_fundo = al_load_sample("./sounds/Code Geass R2 - The Master.ogg");
    som_tiro = al_load_sample("./sounds/laser5.wav");
    som_explosao = al_load_sample("./sounds/explosion.wav");

    if (!musica_fundo || !som_tiro || !som_explosao) {
        printf("Erro ao carregar sons!\n");
        return false;
    }

    instancia_musica = al_create_sample_instance(musica_fundo);
    al_set_sample_instance_playmode(instancia_musica, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(instancia_musica, al_get_default_mixer());
    return true;
}

void tocar_musica() {
  if (musica_fundo) {
        al_play_sample(musica_fundo, 0.3, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
    }
}

void parar_musica() {
    al_stop_sample_instance(instancia_musica);
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

void desenhar_jogo(Jogador jogador, Inimigo inimigos[], int num_inimigos, Tiro tiros[], int max_tiros, ALLEGRO_FONT *fonte){
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
	char texto_pontuacao[50];
    sprintf(texto_pontuacao, "Pontos: %d", jogador.pontuacao);
    al_draw_text(fonte, al_map_rgb(255, 255, 255), 10, 10, 0, texto_pontuacao);

    char texto_recorde[50];
    sprintf(texto_recorde, "Recorde: %d", recorde);
    al_draw_text(fonte, al_map_rgb(255, 255, 255), 10, 30, 0, texto_recorde);

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
	    al_play_sample(som_tiro, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            break;
        }
    }
}

void atualizar_inimigos(Inimigo inimigos[], int num_inimigos, float *velocidade){
    static bool move_direita = true;
    static bool deve_descer = false;  
    float nova_velocidade = *velocidade;

    for (int i = 0; i < num_inimigos; i++) {
        if (inimigos[i].vivo) {
            if (move_direita) {
                inimigos[i].x += nova_velocidade;
                if (inimigos[i].x > LARGURA_TELA - 30) {
                    deve_descer = true;
                }
            } else {
                inimigos[i].x -= nova_velocidade;
                if (inimigos[i].x < 30) {
                    deve_descer = true;
                }
            }
        }
    }

    if (deve_descer) {
        for (int i = 0; i < num_inimigos; i++) {
            if (inimigos[i].vivo) {
                inimigos[i].y += 20;  
            }
        }
        move_direita = !move_direita;  
        deve_descer = false;           
        *velocidade += 0.1f;         
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
bool verificar_colisao(Tiro tiros[], Inimigo inimigos[], int num_inimigos, int max_tiros, Jogador *jogador) {
    for (int i = 0; i < num_inimigos; i++) {
        if (inimigos[i].vivo) {
            for (int j = 0; j < max_tiros; j++) {
                if (tiros[j].ativo) {
                    float dx = tiros[j].x - inimigos[i].x;
                    float dy = tiros[j].y - inimigos[i].y;
                    float distancia = sqrt(dx * dx + dy * dy);

                    if (distancia < 20) { 
                        inimigos[i].vivo = false;
                        tiros[j].ativo = false;
			jogador->pontuacao += 10;
			al_play_sample(som_explosao, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
void salvar_recorde() {
    FILE *arquivo = fopen("recorde.dat", "wb");
    if (arquivo) {
        fwrite(&recorde, sizeof(int), 1, arquivo);
        fclose(arquivo);
    }
}

void carregar_recorde() {
    FILE *arquivo = fopen("recorde.dat", "rb");
    if (arquivo) {
        fread(&recorde, sizeof(int), 1, arquivo);
        fclose(arquivo);
    }
}
void atualizar_recorde(Jogador *jogador) {  
    if (jogador->pontuacao > recorde) {
        recorde = jogador->pontuacao;
        salvar_recorde();  
        printf("Novo recorde: %d!\n", recorde);
    }
}

void liberar_imagens() {
    al_destroy_bitmap(jogador_img);
    al_destroy_bitmap(inimigo_img);
    al_destroy_font(fonte);
}

void liberar_sons() {
    al_stop_sample_instance(instancia_musica);
    al_destroy_sample_instance(instancia_musica);
    al_destroy_sample(musica_fundo);
    al_destroy_sample(som_tiro);
    al_destroy_sample(som_explosao);
    al_uninstall_audio();
}
