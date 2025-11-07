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



    // 🖼️ Crear superficie y textura desde el texto
    SDL_Surface* surface = TTF_RenderText_Solid(font, MENSAJE, 0, color);
    if (!surface) {
        printf("Error al crear superficie de texto: %s\n", SDL_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 5;
    }

    //creo el boton
    SDL_FRect boton = {350,500,200,100};



    int textW = surface->w;
    int textH = surface->h;

    int indicadorDeCambioDeColor = 0;
    int indicadorDeMensajeDePantalla = 0;
    int indicadorDeBotonPresionado = 0;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);



    SDL_FRect dstRect = { (WIDTH - textW) / 2.0f, (HEIGHT - textH) / 2.0f, textW, textH };

    //este es el bucle de la ventana osea de la window
    bool running = true;
    SDL_Event e;
    while (running) {
        //pollEvent se encarga de revisar constantemente si el usuario hace algo
        //ese algo se guarda en SDLevent llamado e
        while (SDL_PollEvent(&e))
            {
                if (e.type == SDL_EVENT_QUIT)
                {
                    running = false;
                }
            //ESTA LINEA CONTROLA SI EL USUARIO presiono una tecla
            if (e.type == SDL_EVENT_KEY_DOWN)
            {
                if(e.key.key == SDLK_ESCAPE) //si la tecla fue escape es estooo
                {
                    running = false;
                }
                if(e.key.key == SDLK_RETURN)
                {
                    //si el usuario presiono enter estoy aca.
                    indicadorDeMensajeDePantalla = 1;
                    
                }
            }
            if(e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                //el usuario clickeo
                //pero clickeo en el boton?
                if(e.button.x >= boton.x && e.button.x <= boton.x + boton.w 
                && e.button.y >= boton.y && e.button.y <= boton.y +  boton.h)
                {
                    indicadorDeBotonPresionado = 1;
                    indicadorDeMensajeDePantalla = 1;
                    indicadorDeCambioDeColor = 1;
                }
            }
        }


        if(indicadorDeCambioDeColor==1)
        {
            SDL_SetRenderDrawColor(renderer,200,50,50,255);
            
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // fondo negro
        }
        SDL_RenderClear(renderer);
        
        //esto es solo para que el boton tenga color
        SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
        SDL_RenderFillRect(renderer, &boton);


        if(indicadorDeMensajeDePantalla==1)
        {
             SDL_Surface* surface2 = TTF_RenderText_Solid(font,MENSAJE2,0,color);

             SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer,surface2);

             SDL_FRect rect2 =  { (WIDTH - surface2->w) / 2.0f, (HEIGHT - surface2->h) / 2.0f, surface2->w, surface2->h };

             SDL_RenderTexture(renderer,texture2,NULL,&rect2);
        }
        else
        {
            SDL_RenderTexture(renderer, texture, NULL, &dstRect); // dibuja el texto
        }
        SDL_RenderPresent(renderer);
        //SDL_Delay(2000);
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
