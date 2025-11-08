#include "Visual.h"


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
/*void mostrarPantallaSolicitudDeNombreParaCargarPartida(SDL_Renderer* renderer, TTF_Font* font)
{
    bool estaLeyendo = true;
    SDL_Event e;
    SDL_Color blanco = {255,255,255,255};
    while(estaLeyendo)
    {
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_EVENT_QUIT)
            {
                exit(3);
            }
            if(e.type == SDL_EVENT_KEY_DOWN)
            {
                estaLeyendo = false;
            }
        }
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        SDL_Surface* surf = TTF_RenderText_Solid(font,"[ Debes ingresar el nombre con el que se  guardo la partida]",0,blanco);
        SDL_Texture* text = SDL_CreateTextureFromSurface(renderer,surf);
        SDL_FRect rect = { 80,(HEIGHT - surf->h)/2, surf->w, surf->h };
        SDL_RenderTexture(renderer,text,NULL,&rect);
        SDL_DestroySurface(surf);
        SDL_DestroyTexture(text);


        SDL_RenderPresent(renderer);
    }

}*/
/*void mostrarMensajeDePartidaNoEncontrada(SDL_Renderer* renderer, TTF_Font* font)
{
    bool estaLeyendo = true;
    SDL_Color blanco = {255,255,255,255};
    SDL_Event e;

    while(estaLeyendo)
    {
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_EVENT_QUIT)
            {
                exit(3);
            }
            if(e.type == SDL_EVENT_KEY_DOWN)
            {
                estaLeyendo = false;
            }
        }

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        SDL_Surface* surf = TTF_RenderText_Solid(font,"Lo siento, no se encontró la partida Cargada", 0,blanco);
        SDL_Texture* text = SDL_CreateTextureFromSurface(renderer,surf);
        SDL_FRect rect = { 80,(HEIGHT - surf->h)/2, surf->w, surf->h };
        SDL_RenderTexture(renderer,text,NULL,&rect);
        SDL_DestroyTexture(text);
        SDL_DestroySurface(surf);


        SDL_RenderPresent(renderer);
    }
}*/

void mostrarMensajeEnVentanaYBorrarDespuesDeTecla(SDL_Renderer* renderer, TTF_Font* font, char* mensaje)
{
    bool estaLeyendo = true;
    SDL_Color blanco = {255,255,255,255};
    SDL_Event e;

    while(estaLeyendo)
    {
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_EVENT_QUIT)
            {
                exit(3);
            }
            if(e.type == SDL_EVENT_KEY_DOWN)
            {
                estaLeyendo = false;
            }
        }

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        SDL_Surface* surf = TTF_RenderText_Solid(font,mensaje, 0,blanco);
        SDL_Texture* text = SDL_CreateTextureFromSurface(renderer,surf);
        SDL_FRect rect = { 80,(HEIGHT - surf->h)/2, surf->w, surf->h };
        SDL_RenderTexture(renderer,text,NULL,&rect);
        SDL_DestroyTexture(text);
        SDL_DestroySurface(surf);


        SDL_RenderPresent(renderer);
    }
}
void mostrarCreditosEnPantalla(SDL_Renderer* renderer, TTF_Font* font)
{
    //FALTA HACER ESTOOOOO , no creo que sea dificil
}
void mostrarTablaDePuntajesDeArchivo(SDL_Renderer* renderer,TTF_Font* font)
{
   // printf("[DEBUG] Se llegó a abrir la funcióin");
    bool estaLeyendo = true;

    int cantLineas;
    char  vectorLineas[TAM_LINEA_SCORES+1][CANT_LINEAS_MAX];
    if(cargarTablaDePuntajes((char*)vectorLineas, &cantLineas)!=TODO_OK)
    {
        exit(4);
    }
    SDL_Color blanco = {255,255,255,255};
    SDL_Event e;
    printf("[DEBUG] Se llegó a cargar la tabla de puntajes");
    char* direccionDeLaLinea = (char*)vectorLineas;
    /*for(int i = 0 ; i < cantLineas ;i++)
    {
        printf("%s\n",direccionDeLaLinea);
        direccionDeLaLinea+=TAM_LINEA_SCORES+1;
    }*/
    while(estaLeyendo)
    {
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_EVENT_QUIT)
            {
                //CORREGIR ESETO, NO PUEDE SALIR ASI NO MAS DLE PROGRAMA
                exit(3);
            }
            if(e.type == SDL_EVENT_KEY_DOWN)
            {
                estaLeyendo = false;
            }
        }
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        int y = 20;
        direccionDeLaLinea = (char*)vectorLineas;
        for(int i = 0; i < cantLineas; i++)
        {
            SDL_Surface* surf = TTF_RenderText_Solid(font,direccionDeLaLinea,0,blanco);
            SDL_Texture* text = SDL_CreateTextureFromSurface(renderer,surf);
            SDL_FRect rect = { 20, y , surf->w, surf->h };
            SDL_RenderTexture(renderer,text,NULL,&rect);
            SDL_DestroySurface(surf);
            SDL_DestroyTexture(text);
            y+=40;
            direccionDeLaLinea+=TAM_LINEA_SCORES+1;
        }
        SDL_RenderPresent(renderer);
    }
}
