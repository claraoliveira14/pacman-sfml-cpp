# pacman-sfml-cpp

# 🐊 Doces & Dentes (Pac-Man Clone)

Um jogo 2D desenvolvido em C++ utilizando a biblioteca **SFML**. Inspirado no clássico arcade *Pac-Man*, o jogador deve devorar um banquete completo (hambúrgueres, bolos, donuts e muito mais) por um labirinto, enquanto foge de 4 jacarés famintos!


## 🎮 Como Jogar

O objetivo é simples: coma todas as comidas do mapa para marcar pontos. Mas cuidado, se um jacaré te alcançar, é Game Over!

### Controles:
* **Setas Direcionais (⬅️ ⬆️ ⬇️ ➡️):** Movem o personagem.
* **Enter:** Reinicia a partida na tela de Game Over.
* **Esc:** Fecha o jogo.

## ✨ Funcionalidades (Features)

* **Sistema de Labirinto Contínuo:** Os clássicos túneis mágicos — saia por um lado da tela e apareça do outro.
* **Inteligência Artificial Mista:** 
  * 3 Jacarés possuem um algoritmo de busca aleatória inteligente (nunca ficam travados em paredes).
  * 1 Jacarés são "Perseguidores", que rastreiam a posição do jogador pelo mapa.
* **Detecção de Colisão:** Hitboxes ajustadas para interações justas e precisas entre o jogador e os inimigos.
* **Estados de Jogo:** Sistema de *Game Over* com reinício rápido sem precisar fechar a aplicação.
* **Interface:** Pontuação em tempo real na tela.

## 🛠️ Tecnologias Utilizadas

* **Linguagem:** C++
* **Biblioteca Gráfica:** [SFML](https://www.sfml-dev.org/) (Simple and Fast Multimedia Library)

## 🚀 Como compilar e rodar na sua máquina

### Pré-requisitos
Você precisa ter um compilador C++ (como o GCC/MinGW) e a biblioteca **SFML** instalada e configurada na sua máquina.

### Passos
1. Clone o repositório
2. Navegue até a pasta do projeto (cd nome-do-repositorio)
3. Compile o código (g++ main.cpp -o jogo -lsfml-graphics -lsfml-window -lsfml-system)
4. Execute o jogo (./jogo)

# Autor
Desenvolvido por Clara Rodrigues de Oliveira como um projeto prático de programação e desenvolvimento de jogos em C++.
