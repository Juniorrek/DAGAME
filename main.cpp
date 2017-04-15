#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "personagem.h"

#define LEVEL_WIDTH 1280
#define LEVEL_HEIGHT 960

//CRIAR .h QUANDO AUMENTAR OS DEFINES
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

//Inicializa o SDL e carrega a janela com sua superficie
bool inicializar();

//Libera e desliga o SDL
void encerrar();

//A janela que sera renderizada
SDL_Window* janela = NULL;

//A superficie da janela
SDL_Surface* superficieJanela = NULL;

//Renderizador da janela
SDL_Renderer* renderizador = NULL;

int main(int argc, char **args) {
	if(!inicializar()) {
		printf( "Falha ao inicializar!\n" );
	} else {
        //Chave do loop principal
        bool sair = false;
        //Event handler do SDL
        SDL_Event evento;
        //Inicia objeto personagem
        personagem p(renderizador);
        //Camera
        SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_Surface *loadedSurface = IMG_Load("bg.png");
        SDL_Texture *background = SDL_CreateTextureFromSurface(renderizador, loadedSurface);
        SDL_Rect renderQuad = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        //Loop principal
        while(!sair) {
            //Manipula eventos na fila
            while(SDL_PollEvent( &evento ) != 0) {
                //Botao X
                if(evento.type == SDL_QUIT) sair = true;
                //Manipula eventos do objeto personagem
                p.manipulaEvento(evento);
            }
            //Ação resultada da manipulação de eventos anterior
            p.acao();

            //Centra a camera no personagem
            camera.x = ( p.getPosX() + p.getLargura() / 2 ) - SCREEN_WIDTH / 2;
            camera.y = ( p.getPosY() + p.getAltura() / 2 ) - SCREEN_HEIGHT / 2;
            printf("CAMERA X = %d, CAMERA Y = %d, PER X = %d, PER Y = %d\n", camera.x, camera.y, p.getPosX(), p.getPosY());
            //Deixa a camera dentro do level
            if( camera.x < 0 ) camera.x = 0;
            if( camera.y < 0 ) camera.y = 0;
            if( camera.x > LEVEL_WIDTH - camera.w ) camera.x = LEVEL_WIDTH - camera.w;
            if( camera.y > LEVEL_HEIGHT - camera.h ) camera.y = LEVEL_HEIGHT - camera.h;

            //Limpa a tela
            SDL_SetRenderDrawColor(renderizador, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(renderizador);

            SDL_RenderCopy(renderizador, background, &camera, &renderQuad );

            //Renderiza a textura na tela
            p.renderizar(renderizador, camera.x, camera.y);
            //Atualiza a tela
            SDL_RenderPresent(renderizador);
            SDL_Delay(100);
        }
	}
	encerrar();

    return 1;
}

bool inicializar() {
    //Inicializa o SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL nao conseguiu inicializar! SDL Error: %s\n", SDL_GetError());
		return false;
	} else {
		//Cria a janela
		janela = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(janela == NULL) {
			printf("Janela nao conseguiu ser criada! SDL Error: %s\n", SDL_GetError());
			return false;
		} else {
		    //Create renderer for window
			renderizador = SDL_CreateRenderer(janela, -1, SDL_RENDERER_ACCELERATED);
			if(renderizador == NULL) {
				printf( "Falha ao criar o renderizador! SDL Error: %s\n", SDL_GetError() );
				return false;
			} else {
			    //Inicializa a cor do renderizador
				SDL_SetRenderDrawColor(renderizador, 0xFF, 0xFF, 0xFF, 0xFF);
                //Inicializa o SDL_image
                if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
                    printf("SDL_image nao conseguiu inicializar! SDL_image Error: %s\n", IMG_GetError());
                    return false;
                } else {
                    //Obtem a superficie da janela
                    superficieJanela = SDL_GetWindowSurface(janela);
                }
			}
		}
	}

	return true;
}

void encerrar() {
	//Destroi a janela
	SDL_DestroyRenderer(renderizador);
	SDL_DestroyWindow(janela);
	janela = NULL;
	renderizador = NULL;

	//Encerra o SDL
	IMG_Quit();
	SDL_Quit();
}
