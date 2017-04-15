#include "personagem.h"

#define LEVEL_WIDTH 1280
#define LEVEL_HEIGHT 960

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

personagem::personagem(SDL_Renderer* renderizador) {
    //ctor
    largura = 64;
    altura = 96;
    posX = LEVEL_WIDTH / 2;
    posY = LEVEL_HEIGHT / 2;
    velocidade = 20;
    a = b = 0;
    //Seta todas as teclas como falsas
    for(int i = 0; i < SDL_NUM_SCANCODES; i++) {
        teclas[i] = false;
    }
    carregaMidia(renderizador);
    //Define o retangulo de cosilao
    colisor.x = posX + 16;
    colisor.y = posY + 2;
    colisor.h = 90;
    colisor.w = 32;
    debug = false;
    velX = velY = 0;
}

personagem::~personagem() {
    //dtor
    //Libera as texturas
	SDL_DestroyTexture(textura);
	textura = NULL;
}

void personagem::renderizar(SDL_Renderer* renderizador, int camX, int camY) {
    //Define o retangulo da renderização
	SDL_Rect rect = { posX - camX, posY - camY, largura, altura };

	SDL_RenderCopy(renderizador, textura, &sprites[a][b], &rect );

	if(debug) {
        SDL_SetRenderDrawColor(renderizador, 0xFF, 0x00, 0x00, 0xFF);
        SDL_RenderDrawRect(renderizador, &colisor);
	}
}

bool personagem::carregaMidia(SDL_Renderer* renderizador) {
	//Carrega a superficie PNG
	SDL_Surface* loadedSurface = IMG_Load("character.png");
	textura = SDL_CreateTextureFromSurface(renderizador, loadedSurface);
	if(textura == NULL) {
		printf("Falha ao carregar a textura do personagem!\n");
		return false;
	} else {
	    int x, y;
	    x = y = 0;
	    //Define as posiçoes de cada sprite de movimentacao
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 8; j++) {
                sprites[i][j].x = x;
                sprites[i][j].y = y;
                sprites[i][j].w = 32;
                sprites[i][j].h = 48;
                x += 32;
            }
            x = 0;
            y += 48;
        }
	}

	return true;
}

void personagem::manipulaEvento(SDL_Event& evento) {
    //Tecla pressionada pelo jogador
    //Pega o scancode da key pq a key é universal e o scancode local
    if(evento.type == SDL_KEYDOWN && evento.key.repeat == 0) {
        teclas[SDL_GetScancodeFromKey(evento.key.keysym.sym)] = true;
        if (SDL_GetScancodeFromKey(evento.key.keysym.sym) == SDL_SCANCODE_D) ultima = 'R';
        if (SDL_GetScancodeFromKey(evento.key.keysym.sym) == SDL_SCANCODE_A) ultima = 'L';
        if (SDL_GetScancodeFromKey(evento.key.keysym.sym) == SDL_SCANCODE_S) ultima = 'D';
        if (SDL_GetScancodeFromKey(evento.key.keysym.sym) == SDL_SCANCODE_W) ultima = 'U';

        if (SDL_GetScancodeFromKey(evento.key.keysym.sym) == SDL_SCANCODE_F12) {
            if(!debug){
                debug = true;
                printf("=== debug mode ON ===\n");
            } else {
                debug = false;
                printf("=== debug mode OFF ===\n");
            }
        }
    //Tecla largada pelo jogador
    } else if(evento.type == SDL_KEYUP && evento.key.repeat == 0) teclas[SDL_GetScancodeFromKey(evento.key.keysym.sym)] = false;
}

void personagem::acao() {
    if(teclas[SDL_SCANCODE_D] && teclas[SDL_SCANCODE_S]) {
        if(ultima == 'R') a = 0;
        else a = 2;
        posX += velocidade;
        posY += velocidade;
    } else if(teclas[SDL_SCANCODE_D] && teclas[SDL_SCANCODE_W]) {
        if(ultima == 'R') a = 0;
        else a = 3;
        posX += velocidade;
        posY -= velocidade;
    } else if(teclas[SDL_SCANCODE_A] && teclas[SDL_SCANCODE_S]) {
        if(ultima == 'L') a = 1;
        else a = 2;
        posX -= velocidade;
        posY += velocidade;
    } else if(teclas[SDL_SCANCODE_A] && teclas[SDL_SCANCODE_W]) {
        if(ultima == 'L') a = 1;
        else a = 3;
        posX -= velocidade;
        posY -= velocidade;
    } else if (teclas[SDL_SCANCODE_D]) {
        a = 0;
        posX += velocidade;
    } else if (teclas[SDL_SCANCODE_A]) {
        a =  1;
        posX -= velocidade;
    } else if (teclas[SDL_SCANCODE_S]) {
        a = 2;
        posY += velocidade;
    } else if (teclas[SDL_SCANCODE_W]) {
        a = 3;
        posY -= velocidade;
    }
    if ((!teclas[SDL_SCANCODE_D] && !teclas[SDL_SCANCODE_A] && !teclas[SDL_SCANCODE_S] && !teclas[SDL_SCANCODE_W]) || b == 7)
        b = 0;
    else b++;

    //Detecta colisao com as extremidades da tela
    if(colisor.x < 0) posX += velocidade;
    if(colisor.y < 0) posY += velocidade;
    if(colisor.x + colisor.w > LEVEL_WIDTH) posX -= velocidade;
    if(colisor.y + colisor.h > LEVEL_HEIGHT) posY -= velocidade;

    //Atualiza o colisor
    colisor.x = posX + 16;
    colisor.y = posY + 2;
}

int personagem::getPosX() {
    return posX;
}

int personagem::getPosY() {
    return posY;
}

int personagem::getLargura() {
    return largura;
}

personagem::getAltura() {
    return altura;
}
