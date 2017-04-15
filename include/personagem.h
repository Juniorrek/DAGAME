#ifndef PERSONAGEM_H
#define PERSONAGEM_H
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

class personagem {
    public:
        personagem(SDL_Renderer* renderizador);
        void renderizar(SDL_Renderer* renderizador, int camX, int camY);
        //Carrega a textura do personagem
        bool carregaMidia(SDL_Renderer* renderizador);
        void manipulaEvento( SDL_Event& e );
        void acao();

        int getPosX();
        int getPosY();
        int getLargura();
        int getAltura();

        virtual ~personagem();

    protected:

    private:
        int largura, altura;
        int posX, posY;
        SDL_Texture* textura;
        SDL_Rect sprites[4][8];
        //Mapa das teclas pressionadas
        bool teclas[SDL_NUM_SCANCODES];
        //Ultima tecla pressionada
        char ultima;
        int velocidade;
        //Indice dos sprites sheet
        int a, b;
        SDL_Rect colisor;
        bool debug;
        int velX, velY;
};

#endif // PERSONAGEM_H
