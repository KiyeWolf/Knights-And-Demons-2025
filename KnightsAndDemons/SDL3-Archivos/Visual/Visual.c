#include "Visual.h"

int funcionDePrueba()
{

    //INICIO SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("Error al iniciar SDL: %s\n", SDL_GetError());
        return 1;
    }
    //inicio la ventana
    SDL_Window* window = SDL_CreateWindow("Knights & Demons", WIDTH, HIGHT, SDL_WINDOW_FULLSCREEN);
    if (!window) {
        printf("Error al crear ventana: %s\n", SDL_GetError());
        SDL_Quit();
        return 2;
    }

    //En este punto abr� las fuentes
    if(TTF_Init() != 1)
    {
        printf("Error");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 3;
    }



    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if(!renderer)
    {
        printf("Error");
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 3;
    }


    TTF_Font* font = TTF_OpenFont(RUTA_ARCHIVO_FUENTES_DE_TEXTO, 26);
    if (!font) {
        printf("Error al abrir fuente: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 4;
    }


    //ahora agrego el color del proximo texto
    SDL_Color color = {255, 255, 255, 255};
    const char* mensaje = "Bienvenido a Knights && Demons 2025";


    // 🖼️ Crear superficie y textura desde el texto
    SDL_Surface* surface = TTF_RenderText_Solid(font, mensaje, 0, color);
    if (!surface) {
        printf("Error al crear superficie de texto: %s\n", SDL_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 5;
    }
    

    // 🔢 Obtener tamaño del texto
    int textW = surface->w;
    int textH = surface->h;


    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FRect dstRect = { (WIDTH - textW) / 2.0f, (HEIGHT - textH) / 2.0f, textW, textH };

    // 🔁 Bucle principal
    bool running = true;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // fondo negro
        SDL_RenderClear(renderer);

        SDL_RenderTexture(renderer, texture, NULL, &dstRect); // dibuja el texto
        SDL_RenderPresent(renderer);
    }

    //siempre se cirra todo
    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
