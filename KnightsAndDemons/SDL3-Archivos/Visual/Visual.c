#include "Visual.h"

int funcionDePrueba()
{

    //INICIO SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("Error al iniciar SDL: %s\n", SDL_GetError());
        return 1;
    }
    //inicio la ventana
    SDL_Window* window = SDL_CreateWindow("Knights & Demons", 800, 600, 0);
    if (!window) {
        printf("Error al crear ventana: %s\n", SDL_GetError());
        SDL_Quit();
        return 2;
    }

    //muestra  esta ventana solo 4 segundos,
    SDL_Delay(4000);


    //siempre se cirra despues la ventana, y despues SDL
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}