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
    SDL_Surface* surface = TTF_RenderText_Solid(font, MENSAJE_MENU_PRINCIPAL, 0, color);
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
void mostrarPantallaBienvenida(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color blanco = {255, 255, 255, 255};
    SDL_Surface* surf = TTF_RenderText_Solid(font, MENSAJE_MENU_PRINCIPAL, 0,blanco);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);

    SDL_FRect rect = { (WIDTH - surf->w)/2, (HEIGHT - surf->h)/2, surf->w, surf->h };
    SDL_DestroySurface(surf);

    bool esperando = true;
    SDL_Event e;

    while (esperando) {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                esperando = false;
            }
            if (e.type == SDL_EVENT_KEY_DOWN)
            {
                //presionando cualquier tecla se sale
                esperando = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, tex, NULL, &rect);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(tex);
}
void mostrarMenuPrincipal(SDL_Renderer* renderer, TTF_Font* font, char* opcion) {
    const char* opciones[CANT_OPCIONES_MENU_PRINCIPAL] = {
        "1. CARGAR PARTIDA",
        "2. NUEVO JUEGO",
        "3. CREDITOS",
        "4. TABLA DE PUNTAJES",
        "5. SALIR"
    };
    int numOpciones = CANT_OPCIONES_MENU_PRINCIPAL;

    bool enMenu = true;
    SDL_Event e;

    while (enMenu) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT)
            {
                 *(opcion) = '5';
                 enMenu=false;
            }
            if (e.type == SDL_EVENT_KEY_DOWN) {
                if (e.key.key >= '1' && e.key.key <= '5')
                {
                    *(opcion) = e.key.key;
                    enMenu = false;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Color blanco = {255, 255, 255, 255};
        int y = 200;
        for (int i = 0; i < numOpciones; i++) {
            SDL_Surface* surf = TTF_RenderText_Solid(font, opciones[i], 0, blanco);
            SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
            SDL_FRect rect = { (WIDTH - surf->w)/2, y, surf->w, surf->h };
            SDL_RenderTexture(renderer, tex, NULL, &rect);
            SDL_DestroySurface(surf);
            SDL_DestroyTexture(tex);
            y += 60;
        }

        SDL_RenderPresent(renderer);
    }

}
void mostrarPantallaNombre(SDL_Renderer* renderer, TTF_Font* font, char* nombrePlayer, SDL_Window* window) {
    bool escribiendo = true;
    SDL_Event e;
    char buffer[32] = "";
    SDL_Color blanco = {255,255,255,255};
    SDL_RaiseWindow(window);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_StartTextInput(window);

    //cuando hay que escribir se usa startTextInput
    SDL_StartTextInput(window);

    if (!renderer) { printf("[ERROR] renderer es NULL\n");}
    if (!font) { printf("[ERROR] font es NULL\n");  }


    while (escribiendo) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT)
            {

              escribiendo = false;
            }
            if (e.type == SDL_EVENT_TEXT_INPUT)
            {
                printf("[DEBUG] evento TEXT_INPUT: \"%s\"\n", e.text.text); fflush(stdout);
                /* evitar overflow: concatenar solo lo que cabe en buffer */
                size_t avail = sizeof(buffer) - 1 - strlen(buffer);
                if (avail > 0) {
                    strncat(buffer, e.text.text, avail);
                }
            }
            if (e.type == SDL_EVENT_KEY_DOWN) {
                if (e.key.key == SDLK_BACKSPACE && strlen(buffer) > 0)
                {
                    buffer[strlen(buffer)-1] = '\0';
                }
                if (e.key.key == SDLK_RETURN && strlen(buffer) == MIN_CANTIDAD_LETRAS_NOMBRE)
                {
                    escribiendo = false;
                }
            }
        }


        printf("[DEBUG] mostrarPantallaNombre: inicio. MIN_LETRAS=%d\n", MIN_CANTIDAD_LETRAS_NOMBRE);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        //texto de ingreso normalito:
        SDL_Surface* s1 = TTF_RenderText_Solid(font, "Ingresa tu nombre (3 Alfanumericos):", 0,blanco);

        if (!s1)
        {
            printf("[ERROR] TTF_RenderText_Solid fallo: %s\n", SDL_GetError());
            SDL_Delay(3000);
            return;
        }

        SDL_Texture* t1 = SDL_CreateTextureFromSurface(renderer, s1);

        if(!t1)
        {
            printf("[ERROR] SDL_CreateTextureFromSurface fallo: %s\n", SDL_GetError());
            SDL_Delay(3000);
            SDL_DestroySurface(s1);
            return;
        }
        SDL_FRect r1 = { (WIDTH-s1->w)/2, 200, s1->w, s1->h };
        SDL_RenderTexture(renderer, t1, NULL, &r1);
        SDL_DestroySurface(s1);
        SDL_DestroyTexture(t1);

        //solucion al error de que no printea si esta vacio el  buffer del teclado
        const char* textoMostrar = (strlen(buffer) > 0) ? buffer : "_";
        //de esta manera el texto a mostrar siempre es el buffer o un guion si no se ingreso nada

        int len2 = (int)strlen(buffer);
        SDL_Surface* s2 = TTF_RenderText_Solid(font, textoMostrar, len2,blanco);
        if(!s2)
        {
            //printf("[ERROR] No se pudo renderizar el texto del jugador: %s\n", SDL_GetError());
        }
        SDL_Texture* t2 = SDL_CreateTextureFromSurface(renderer, s2);
        if(!t2)
        {
            printf("[ERROR] SDL_CreateTextureFromSurface fallo: %s\n", SDL_GetError());
            SDL_Delay(3000);
            SDL_DestroySurface(s1);
            return;
        }
        SDL_FRect r2 = { (WIDTH-s2->w)/2, 300, s2->w, s2->h };
        SDL_RenderTexture(renderer, t2, NULL, &r2);
        SDL_DestroySurface(s2);
        SDL_DestroyTexture(t2);

        SDL_RenderPresent(renderer);
    }
    SDL_StopTextInput(window);
    strcpy(nombrePlayer, buffer);
}
void mostrarPantallaDificultad(SDL_Renderer* renderer,TTF_Font* font,int* dificultad)
{
    const char* opciones[3]={
        "1. FACIL",
        "2. MEDIO",
        "3. DIFICIL"
    };
    bool esperando = true;
    SDL_Event e;
    SDL_Color blanco = {255,255,255,255};
    int numOpciones = 3;

    while(esperando)
    {
        while(SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                //CORREGIR CERRAR EL PROGRAMA ESTA MAL
              exit(0);
            }
            if(e.type == SDL_EVENT_KEY_DOWN)
            {
                if(e.key.key >= '1' && e.key.key <= '3')
                {
                    esperando = false;
                    switch(e.key.key)
                    {
                        case '1':
                        *(dificultad) = 1;
                        break;
                        case '2':
                        *(dificultad) = 2;
                        break;
                        case '3':
                        *(dificultad) = 3;
                        break;
                        default:
                        break;
                    }
                }
            }


        }
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

            SDL_Surface* surf = TTF_RenderText_Solid(font,"Selecciona la dificultad:",0, blanco);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surf);
            SDL_FRect rect = { (WIDTH- surf->w)/2, 100, surf->w, surf->h };
            SDL_RenderTexture(renderer,texture,NULL,&rect);
            SDL_DestroySurface(surf);
            SDL_DestroyTexture(texture);
            int y = 200;
            for (int i = 0; i < numOpciones; i++) {
                SDL_Surface* surf = TTF_RenderText_Solid(font, opciones[i], 0, blanco);
                SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
                SDL_FRect rect = { (WIDTH - surf->w)/2, y, surf->w, surf->h };
                SDL_RenderTexture(renderer, tex, NULL, &rect);
                SDL_DestroySurface(surf);
                SDL_DestroyTexture(tex);
                y += 60;
            }
            SDL_RenderPresent(renderer);
    }
}
void mostrarPantallaHistoriaInicial(SDL_Renderer* renderer,TTF_Font* font, size_t* retorno)
{
    bool leyendo = true;
    SDL_Event e;
    SDL_Color blanco = {255,255,255,255};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    FILE* archivo = fopen(RUTA_HISTORIA_INICIAL, "r");
    system("cls");
    if (archivo == NULL) {
        *retorno = ARCHIVO_CORRUPTO;
        return;
    }
    else
    {
        char linea[TAM_LINEA_JUEGO]={"[Usa el enter para proseguir...]"};
        while(leyendo)
        {
            while(SDL_PollEvent(&e))
            {
                if (e.type == SDL_EVENT_QUIT)
                {
                    //CORREGIR CERRAR EL PROGRAMA ESTA MAL
                    exit(0);
                }
                if(e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_RETURN)
                {

                    if(fgets(linea, sizeof(linea), archivo)==NULL)
                    {
                        leyendo = false;

                    }
                    else
                    {
                        linea[strcspn(linea, "\r\n")] = '\0';
                    }
                }
            }
                SDL_SetRenderDrawColor(renderer,0,0,0,255);
                SDL_RenderClear(renderer);
            if(leyendo)
            {

                SDL_Surface* surf = TTF_RenderText_Solid(font, linea, 0, blanco);
                SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
                SDL_FRect rect = { 80,(HEIGHT - surf->h)/2, surf->w, surf->h };
                SDL_RenderTexture(renderer, tex, NULL, &rect);
                SDL_DestroySurface(surf);
                SDL_DestroyTexture(tex);
            }
            SDL_RenderPresent(renderer);
        }

        fclose(archivo);
        *(retorno) = TODO_OK;
    }
}
