#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

char mapa[15][31] = {
    "111111111111112311111111111111",
    "145678234567824578234567823451",
    "161111711111110011111115111161",
    "171111811111110011111118111121",
    "123456782345678234567823456781",
    "131111411511111111116117111181",
    "145678211345678234567118234561",
    "821111311436785624387118111123",
    "123456711813400002315116782341",
    "131111411511111111116117111181",
    "182345678234567823456782345671",
    "121111311111115611111118111121",
    "131111411111117811111113111141",
    "156782345678234567823456782341",
    "111111111111116711111111111111"
};

const float SIZE = 50;  //tamanho da largura dos blocos para calculos

int placar = 0;         //variavel para contar pontos

float posx = 720, posy = 115;   // posicao do personagem (começa no meio do mapa)


struct Inimigo {
    float x, y;
    int direcao; // 0 = cima, 1 = baixo, 2 = esquerda, 3 = direita
    int tipo;    // 0 = aleatorio, 1 = perseguidor
};

//função para verificar se o personagem pode mover
bool PodeMover (float novox, float novoy) {

    int esquerda = (novox + 23.f)  / SIZE;  // calculo para achar coordenada na matriz e barrar o personagem de passar pelas paredes
    int direita = (novox + 45.f)  / SIZE; 
    int topo = (novoy + 15.f) / SIZE;
    int base = (novoy + 45.f) / SIZE;


    if (mapa[topo][esquerda] == '1' ||         // para não atravessar as paredes
        mapa[topo][direita] == '1' ||
        mapa[base][esquerda] == '1' ||
        mapa[base][direita] == '1')
        return false;
    
    return true;
}

//função para verificar se o local onde o personagem está tem pilula(comida), e após passar por elas, transformar em espaço vazio
bool Pilulas (float novox, float novoy) {
    int x = (novox + 25.f) / SIZE;  //verficar as coordenadas na matriz
    int y = (novoy + 25.f) / SIZE;

    // Verifica se está dentro dos limites da matriz
    if (x >= 0 && x < 30 && y >= 0 && y < 15) {
        char bloco = mapa[y][x];
        // Se o bloco for qualquer coisa entre '2' e '8', é comida e é transformada em '0'.
        if (bloco >= '2' && bloco <= '8') {
            mapa[y][x] = '0';
            return true;
        }
    }
    return false;
}

//verfica se o mapa está sem pilulas, se estiver termina o jogo
bool AchouPilula (float novox, float novoy) {   
    for(int i=0;i<15;i++)
            for(int j=0;j<31;j++) {
                if (mapa[i][j] >= '2' && mapa[i][j] <= '8')
                    return true;
            }
    return false;
}

bool cima = false;  // direcao de movimento do personagem
bool baixo = false;
bool esq = false;
bool dir = false;
bool Movendo = false;   // verifica se o personagem está atualmente se movendo

int main() {
    // cria a janela
    
    sf::RenderWindow window(sf::VideoMode({1500, 750}), "Minha janela");

    //********** TEXTOS ************//
    sf::Font font;  //escolhendo fonte
    if (!font.openFromFile("fontejogo.ttf")) {
        std::cout << "Erro lendo fonte fontejogo.ttf\n";
        return 0;
    }
    sf::Text texto(font);
    texto.setFont(font);
    texto.setCharacterSize(40); //tamanho
    texto.setFillColor(sf::Color::White);   //cor
    texto.setOutlineColor(sf::Color(80, 50, 30));   //cor do delineamento
    texto.setOutlineThickness(4.f); //grossura do delineamento
    texto.setStyle(sf::Text::Bold); //estilo
    texto.setString("SCORE:   0");   //começa em '0'
    //*******************************//

   // Array com 4 jacarés: 3 aleatórios (tipo 0) e 1 perseguidor (tipo 1)
    Inimigo jacares[4] = {
    {740, 400, 3, 0}, // Jacaré 1: Aleatório
    {790, 400, 2, 0}, // Jacaré 2: Aleatório
    {840, 400, 0, 0}, // Jacaré 3: Aleátorio
    {890, 400, 0, 1}  // Jacaré 4: Perseguidor
    };
    //********** SPRITES ************//
    
    //PERSONAGEM
    // personagem vista de frente
    sf::Texture texparada;
    if (!texparada.loadFromFile("frente.png")) {
        std::cout << "Erro lendo imagem frente.png\n";
        return 0;
    }
    // personagem vista da direita
    sf::Texture texDireita;
    if (!texDireita.loadFromFile("direita.png")) {
        std::cout << "Erro lendo direita.png\n";
        return 0;
}   
    // personagem vista da esquerda
    sf::Texture texEsquerda;
    if (!texEsquerda.loadFromFile("esquerda.png")) {
        std::cout << "Erro lendo esquerda.png\n";
        return 0;
}
    // personagem vista de trás
    sf::Texture texTras;
    if (!texTras.loadFromFile("tras.png")) {
        std::cout << "Erro lendo tras60.png\n";
        return 0;
}
    //qual 'roupa' o personagem começa utilizando
    sf::Sprite personagem{texparada};

    //INIMIGOS
    sf::Texture jacparado;
    if (!jacparado.loadFromFile("jacarefrente.png")) {
        std::cout << "Erro lendo imagem jacarefrente.png\n";
        return 0;
    }
    //direita
    sf::Texture jacDireita;
    if (!jacDireita.loadFromFile("direita.png")) {
        std::cout << "Erro lendo direita.png\n";
        return 0;
}   
    //esquerda
    sf::Texture jacEsquerda;
    if (!jacEsquerda.loadFromFile("jacareesquerda.png")) {
        std::cout << "Erro lendo esquerda.png\n";
        return 0;
}
    //trás
    sf::Texture jacTras;
    if (!jacTras.loadFromFile("jacaretras.png")) {
        std::cout << "Erro lendo jacaretras.png\n";
        return 0;
}
    //qual 'roupa' o jacaré começa utilizando
    sf::Sprite jacare{jacparado};
    
    //PAREDES
    sf::Texture paredes;
    if(!paredes.loadFromFile("agoravai.png")) {
        std::cout << "Erro lendo paredes.png\n";
        return 0;
    }
    sf::Sprite parede{paredes};

    //COMIDAS
    // 2 = hamburguer, 3 = bolo de morango, 4 = macarrão, 5 = bolo de chocolate, 6 = donut, 7 = hotdog, 8 = ovo frito.
    sf::Texture comidas2;
    if(!comidas2.loadFromFile("burger.png")){
        std::cout << "Erro lendo burguer.png\n";
        return 0;
    }
    sf::Sprite comida2{comidas2};

    sf::Texture comidas3;
    if(!comidas3.loadFromFile("strawberrycake.png")){
        std::cout << "Erro lendo strawberrycake.png\n";
        return 0;
    }
    sf::Sprite comida3{comidas3};

    sf::Texture comidas4;
    if(!comidas4.loadFromFile("spaghetti.png")){
        std::cout << "Erro lendo spaghetti.png\n";
        return 0;
    }
    sf::Sprite comida4{comidas4};

    sf::Texture comidas5;
    if(!comidas5.loadFromFile("chocolatecake.png")){
        std::cout << "Erro lendo chocolatecake.png\n";
        return 0;
    }
    sf::Sprite comida5{comidas5};

    sf::Texture comidas6;
    if(!comidas6.loadFromFile("donut.png")){
        std::cout << "Erro lendo donut.png\n";
        return 0;
    }
    sf::Sprite comida6{comidas6};

    sf::Texture comidas7;
    if(!comidas7.loadFromFile("hotdog.png")){
        std::cout << "Erro lendo hotdog.png\n";
        return 0;
    }
    sf::Sprite comida7{comidas7};

    sf::Texture comidas8;
    if(!comidas8.loadFromFile("friedegg.png")){
        std::cout << "Erro lendo friedegg.png\n";
        return 0;
    }
    sf::Sprite comida8{comidas8};

    //***********************************//

    //********** RELOGIO ************//
    sf::Clock clock;


    //********* MOVIMENTOS ************//
    // executa o programa enquanto a janela está aberta
    while (window.isOpen()) {

        // verifica todos os eventos que foram acionados na janela desde a última iteração do loop
        while (const std::optional event = window.pollEvent()) {
            // evento "fechar" acionado: fecha a janela
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                  if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                      window.close();
                  else if (keyPressed->scancode == sf::Keyboard::Scancode::Left) {
                      if (PodeMover(posx-5.f, posy))
                        esq = true;   // left key: personagem tem intenção de se mover para esquerda
                        dir = cima = baixo = false;   // left key: move o personagem para esquerda
                  }
                  else if (keyPressed->scancode == sf::Keyboard::Scancode::Right) {
                      if (PodeMover(posx+5.f, posy))
                        dir = true;   // right key: personagem tem intenção de se mover para direita
                        esq = cima = baixo = false;   // right key: move o personagem para direita
                  }
                  else if (keyPressed->scancode == sf::Keyboard::Scancode::Up) {
                      if (PodeMover(posx, posy-5.f))
                        cima = true;   // up key: personagem tem intenção de se mover para cima
                        esq = dir = baixo = false;   // up key: move o personagem para cima
                  }
                  else if (keyPressed->scancode == sf::Keyboard::Scancode::Down) {
                      if (PodeMover(posx, posy+5.f))
                        baixo = true;   // down key: personagem tem intenção de se mover para baixo
                        esq = dir = cima = false; // down key: move o personagem para baixo
                  }
                  
            }
        }

        // Muda a posição do PacMan a cada 0.2 segundos
        if (clock.getElapsedTime() > sf::seconds(0.02)) { // tempo desde último restart
            clock.restart();      // recomeça contagem do tempo
            if (Pilulas(posx,posy)) {
                 placar+=10;    //cada pilula é 10 pontos
                 texto.setString("Score: " + std::to_string(placar));   //atualização do placar
            }
            //se o personagem estiver movendo para cima, e poder continuar movendo, ele continua de 5 em 5 pixels
            if (cima && PodeMover(posx, posy - 5.f)) {
                posy = posy - 5.f;     
                Movendo = true;
                if (posy < -50) //túnel para passar pro outro lado da tela
                    posy = 750;
            }
            //se o personagem estiver movendo para baixo, e poder continuar movendo, ele continua de 5 em 5 pixels
            if (baixo && PodeMover(posx, posy + 5.f)) {
                posy = posy + 5.f;
                Movendo = true;
                if (posy > 750) //túnel para passar pro outro lado da tela
                    posy = -50;
            }
            //se o personagem estiver movendo para esquerda, e poder continuar movendo, ele continua de 5 em 5 pixels
            if (esq && PodeMover(posx - 5.f, posy)) {
                posx = posx - 5.f;
                if (posx <= 0)   //túnel para passar pro outro lado da tela
                    posx = 1500;
                Movendo = true;
            }
            //se o personagem estiver movendo para direita, e poder continuar movendo, ele continua de 5 em 5 pixels
            if (dir && PodeMover(posx + 5.f, posy)) {
                posx = posx + 5.f;
                if (posx >= 1500)     //túnel para passar pro outro lado da tela
                    posx = 0;
                Movendo = true;
            }
        //MOVIMENTO DOS INIMIGOS
        for (int i = 0; i < 4; i++) {
        // === TIPO 0: MOVIMENTO ALEATÓRIO ===
            if (jacares[i].tipo == 0) {
                bool moveu = false;
            
            if (jacares[i].direcao == 0 && PodeMover(jacares[i].x, jacares[i].y - 5.f)) { jacares[i].y -= 5.f; moveu = true; }
            else if (jacares[i].direcao == 1 && PodeMover(jacares[i].x, jacares[i].y + 5.f)) { jacares[i].y += 5.f; moveu = true; }
            else if (jacares[i].direcao == 2 && PodeMover(jacares[i].x - 5.f, jacares[i].y)) { jacares[i].x -= 5.f; moveu = true; }
            else if (jacares[i].direcao == 3 && PodeMover(jacares[i].x + 5.f, jacares[i].y)) { jacares[i].x += 5.f; moveu = true; }

            // Se bateu na parede, escolhe uma NOVA direção inteligente
            if (!moveu) {
                int caminhos_livres[4]; // Guardar as direções permitidas
                int total_livres = 0;   // Contador de quantos caminhos estão abertos

                // Verifica cada direção e salva no array se não for parede
                if (PodeMover(jacares[i].x, jacares[i].y - 5.f)) {
                    caminhos_livres[total_livres] = 0; // Cima
                    total_livres++;
        }
                if (PodeMover(jacares[i].x, jacares[i].y + 5.f)) {
                    caminhos_livres[total_livres] = 1; // Baixo
                    total_livres++;
        }
                if (PodeMover(jacares[i].x - 5.f, jacares[i].y)) {
                    caminhos_livres[total_livres] = 2; // Esquerda
                    total_livres++;
            }
                if (PodeMover(jacares[i].x + 5.f, jacares[i].y)) {
                    caminhos_livres[total_livres] = 3; // Direita
                    total_livres++;
        }

        // Se encontrou pelo menos um caminho livre, sorteia entre eles
                if (total_livres > 0) {
                    int sorteio = rand() % total_livres;
                    jacares[i].direcao = caminhos_livres[sorteio];
        }
    }
        }
        
        // === TIPO 1: MOVIMENTO PERSEGUIDOR ===
        else if (jacares[i].tipo == 1) {
            if (jacares[i].x < posx && PodeMover(jacares[i].x + 5.f, jacares[i].y)) 
                jacares[i].x += 5.f; 
            else if (jacares[i].x > posx && PodeMover(jacares[i].x - 5.f, jacares[i].y)) 
                jacares[i].x -= 5.f; 
            else if (jacares[i].y < posy && PodeMover(jacares[i].x, jacares[i].y + 5.f)) 
                jacares[i].y += 5.f;  
            else if (jacares[i].y > posy && PodeMover(jacares[i].x, jacares[i].y - 5.f))
                jacares[i].y -= 5.f; 
        }
    }
        }

        // ==========================================
        // VERIFICAÇÃO DE COLISÃO (GAME OVER)
        // ==========================================
        for (int i = 0; i < 4; i++) {
            // Calcula a distância entre o centro do personagem e o centro do jacaré
            float distanciaX = std::abs(posx - jacares[i].x);
            float distanciaY = std::abs(posy - jacares[i].y);

            // Se a distância nos dois eixos for menor que 35 pixels, eles se bateram!
            // Nota: Deixamos 35 no lugar de 50 (SIZE) para dar uma "folga" e o jogo não ser injusto
            if (distanciaX < 20.f && distanciaY < 20.f) {
            std::cout << "\n===============================\n";
            std::cout << " GAME OVER! \n";
            std::cout << " SCORE FINAL: " << placar << "\n";
            std::cout << "===============================\n";
        
            window.close(); // Fecha a janela e encerra o jogo
    }
} 
        // limpa a janela com a cor rosa claro
        window.clear(sf::Color(255, 209, 220));

        //********** DESENHOS **********//
        
        for(int i=0;i<15;i++)
            for(int j=0;j<31;j++) {
                if (mapa[i][j]=='1') {
                    parede.setPosition({j*SIZE, i*SIZE});
                    window.draw(parede);
                }
                if (mapa[i][j] == '2') {
                    comida2.setPosition({j*SIZE + 9.f, i*SIZE+ 9.f});	//desenha a comida no meio do passeio livre
                    window.draw(comida2);
                }
                if (mapa[i][j] == '3') {
                    comida3.setPosition({j*SIZE + 9.f, i*SIZE+ 9.f});
                    window.draw(comida3);
                }
                if (mapa[i][j] == '4') {
                    comida4.setPosition({j*SIZE + 9.f, i*SIZE+ 9.f});
                    window.draw(comida4);
                }
                if (mapa[i][j] == '5') {
                    comida5.setPosition({j*SIZE + 9.f, i*SIZE+ 9.f});
                    window.draw(comida5);
                }
                if (mapa[i][j] == '6') {
                    comida6.setPosition({j*SIZE + 9.f, i*SIZE+ 9.f});
                    window.draw(comida6);
                }
                if (mapa[i][j] == '7') {
                    comida7.setPosition({j*SIZE + 9.f, i*SIZE+ 9.f});
                    window.draw(comida7);
                }
                if (mapa[i][j] == '8') {
                    comida8.setPosition({j*SIZE + 9.f, i*SIZE+ 9.f});
                    window.draw(comida8);
                }
            }

        window.draw(texto);
        // reposiciona o personagem
        personagem.setPosition({posx, posy});
        // posição dos inimigos
        for (int i = 0; i < 4; i++) {
        jacare.setPosition({jacares[i].x, jacares[i].y});
        window.draw(jacare);
}
        // desenha o personagem na janela de acordo com seu movimento
        if (Movendo) {
            if (dir)
                personagem.setTexture(texDireita);
            if (esq)
                personagem.setTexture(texEsquerda);
            if (cima)
                personagem.setTexture(texTras);
            if (baixo)
                personagem.setTexture(texparada);
        } 
        else {
            personagem.setTexture(texparada);
        
}
        window.draw(personagem);
        
        //se não achar mais pilulas, o jogador ganhou e o programa fecha
        for(int i=0;i<15;i++)
            for(int j=0;j<31;j++) {
                if (AchouPilula(i,j) == false){
                    std::cout << "===============================\n";
                    std::cout << "            YOU WIN!           \n";
                    std::cout << "===============================\n";
                    window.close(); // Fecha a janela e encerra o jogo
            }
        }
        // termina e desenha o frame corrente
        window.display();
    }

    return 0;
}
