#include "spaceinvaders.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdbool.h>
#include <stdio.h>
#define ALTURA_TELA 600
#define LARGURA_TELA 800
#define NUM_INIMIGOS 20
#define MAX_TIROS 10

int main() {
    // Inicializa Allegro e seus addons
    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return -1;
    }
    if (!al_install_keyboard()) {
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
        return -1;
    }
    if (!al_init_primitives_addon()) {
        fprintf(stderr, "Falha ao inicializar primitives addon.\n");
        return -1;
    }
    if (!al_init_image_addon()) {
        fprintf(stderr, "Falha ao inicializar image addon.\n");
        return -1;
    }

    // Configura display
    ALLEGRO_DISPLAY *display = al_create_display(800, 600);
    if (!display) {
        fprintf(stderr, "Falha ao criar display.\n");
        return -1;
    }

    // Fila de eventos
    ALLEGRO_EVENT_QUEUE *fila_eventos = al_create_event_queue();
    al_register_event_source(fila_eventos, al_get_display_event_source(display));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());

    // Carrega imagens
    if (!carregar_imagens()) {
        al_destroy_display(display);
        return -1;
    }

    // Variáveis do jogo
    Jogador jogador;
    Inimigo inimigos[NUM_INIMIGOS];
    Tiro tiros[MAX_TIROS] = {0};
    float velocidade_inimigos = 1.0f;
    bool sair = false;
    bool tecla[2] = {false, false}; // [0] = esquerda, [1] = direita

    inicializar_jogo(&jogador, inimigos, NUM_INIMIGOS);

    // Loop principal
    while (!sair) {
        ALLEGRO_EVENT evento;
        while (al_get_next_event(fila_eventos, &evento)) {
            switch (evento.type) {
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    sair = true;
                    break;
                case ALLEGRO_EVENT_KEY_DOWN:
                    if (evento.keyboard.keycode == ALLEGRO_KEY_LEFT) tecla[0] = true;
                    if (evento.keyboard.keycode == ALLEGRO_KEY_RIGHT) tecla[1] = true;
                    if (evento.keyboard.keycode == ALLEGRO_KEY_SPACE) atirar(jogador, tiros, MAX_TIROS);
                    if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) sair = true;
                    break;
                case ALLEGRO_EVENT_KEY_UP:
                    if (evento.keyboard.keycode == ALLEGRO_KEY_LEFT) tecla[0] = false;
                    if (evento.keyboard.keycode == ALLEGRO_KEY_RIGHT) tecla[1] = false;
                    break;
            }
        }

        // Movimento do jogador
        if (tecla[0]) mover_jogador(&jogador, -1);
        if (tecla[1]) mover_jogador(&jogador, 1);

        // Atualiza lógica do jogo
        atualizar_inimigos(inimigos, NUM_INIMIGOS, &velocidade_inimigos);
        atualizar_tiros(tiros, MAX_TIROS, 5.0f);
        verificar_colisao(tiros, inimigos, NUM_INIMIGOS, MAX_TIROS);

        // Renderização
        desenhar_jogo(jogador, inimigos, NUM_INIMIGOS, tiros, MAX_TIROS);

        // Game Over se inimigos chegarem perto
        for (int i = 0; i < NUM_INIMIGOS; i++) {
            if (inimigos[i].vivo && inimigos[i].y > ALTURA_TELA - 60) {
                printf("GAME OVER! Inimigos invadiram!\n");
                sair = true;
                break;
            }
        }

        al_rest(0.016); // ~60 FPS
    }

    // Libera recursos
    liberar_imagens();
    al_destroy_event_queue(fila_eventos);
    al_destroy_display(display);

    return 0;
}
