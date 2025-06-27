# Space Invaders em C com Allegro

## 📝 Descrição
Este é um jogo clássico Space Invaders desenvolvido em C usando a biblioteca Allegro 5. Controles simples, mecânicas divertidas e todos os elementos essenciais do jogo original!

## 🎮 Como Jogar
- **Setas direcionais**: Movimentam a nave
- **Barra de espaço**: Atira
- **ESC**: Sai do jogo

## ✨ Funcionalidades
- Sistema de pontuação e recordes
- Dificuldade progressiva (inimigos ficam mais rápidos)
- Efeitos sonoros para tiros e explosões
- Música de fundo épica
- Detecção de colisão precisa

## 🛠️ Tecnologias Utilizadas
- Linguagem C
- Biblioteca Allegro 5 (para gráficos, áudio e input)
- Sistema de arquivos para salvar recordes

## 📂 Estrutura de Arquivos
```
spaceinvaders/
├── sounds/          # Efeitos sonoros e música
├── images/         # Sprites do jogo
├── font/           # Fonte arcade
├── main.c          # Lógica principal
├── spaceinvaders.c # Implementação do jogo
└── spaceinvaders.h # Definições e headers
```

## 🔧 Compilação
```bash
gcc main.c spaceinvaders.c -o spaceinvaders \
    $(pkg-config --cflags --libs allegro-5 allegro_image-5 allegro_primitives-5 \
    allegro_font-5 allegro_ttf-5 allegro_audio-5 allegro_acodec-5) \
    -lm
```

## 🎵 Trilha Sonora
- Música tema: "Code Geass R2 - The Master"
- Efeitos sonoros originais
