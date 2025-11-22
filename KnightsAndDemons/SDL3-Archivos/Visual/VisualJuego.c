#include "VisualJuego.h"


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
            if(e.type==SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
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

void mostrarMenuPrincipal(SDL_Renderer* renderer, TTF_Font* font, char* opcion, tSonido* sonidoBotonMenu, tSonido* bgm)
{
    const char* opciones[CANT_OPCIONES_MENU_PRINCIPAL] = {
        "1. CARGAR PARTIDA",
        "2. NUEVO JUEGO",
        "3. CREDITOS",
        "4. TABLA DE PUNTAJES",
        "5. SALIR"
    };
    SDL_RenderClear(renderer);
    SDL_FRect botones[CANT_OPCIONES_MENU_PRINCIPAL];
    int numOpciones = CANT_OPCIONES_MENU_PRINCIPAL;
    bool enMenu = true;
    SDL_Event e;

    // CONSTANTES DE ESTILO
    const float ANCHO_BOTON = 400.0f;
    const float ALTO_BOTON = 50.0f;
    const float ESPACIO_ENTRE_BOTONES = 20.0f;

    while (enMenu) {
        
        // 1. OBTENER MOUSE PARA HOVER (Fuera del pool de eventos para que sea continuo)
        float mx, my;
        SDL_GetMouseState(&mx, &my);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                 *(opcion) = '5';
                 enMenu = false;
            }

            // CLICK
            if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                // Usamos las coordenadas del evento, que son precisas al momento del click
                float clickX = e.button.x;
                float clickY = e.button.y;

                for (int i = 0; i < numOpciones; i++) {
                    SDL_FRect r = botones[i];
                    if (clickX >= r.x && clickX <= r.x + r.w &&
                        clickY >= r.y && clickY <= r.y + r.h)
                    {
                        reproducirSFX(sonidoBotonMenu);
                        *opcion = '1' + i;
                        enMenu = false;
                    }
                }
            }

            // TECLADO
            if (e.type == SDL_EVENT_KEY_DOWN) {
                if (e.key.key >= '1' && e.key.key <= '5') {
                    reproducirSFX(sonidoBotonMenu); // Agregué sonido aquí también ;)
                    *(opcion) = e.key.key;
                    enMenu = false;
                }
            }
        }

        // --- RENDERIZADO ---
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fondo negro de pantalla
        SDL_RenderClear(renderer);

        // Calculamos la Y inicial para que el bloque de botones quede centrado verticalmente
        float totalAltoMenu = (numOpciones * ALTO_BOTON) + ((numOpciones - 1) * ESPACIO_ENTRE_BOTONES);
        float yActual = (HEIGHT - totalAltoMenu) / 2.0f;

        for (int i = 0; i < numOpciones; i++) {
            
            // 2. DEFINIR LA CAJA DEL BOTÓN
            SDL_FRect box = { 
                (WIDTH - ANCHO_BOTON) / 2.0f, // Centrado horizontal
                yActual, 
                ANCHO_BOTON, 
                ALTO_BOTON 
            };
            botones[i] = box; // Guardamos para la lógica de click

            // 3. DETECTAR HOVER (¿El mouse está sobre esta caja?)
            bool isHover = (mx >= box.x && mx <= box.x + box.w && 
                            my >= box.y && my <= box.y + box.h);

            // 4. ELEGIR COLORES SEGÚN ESTADO
            SDL_Color colorTexto;
            SDL_Color colorBorde;
            
            if (isHover) {
                // ESTILO HOVER: Borde Dorado, Relleno gris oscuro, Texto Dorado
                SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255); // Relleno gris
                SDL_RenderFillRect(renderer, &box);
                
                SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255); // Borde Dorado
                colorBorde = (SDL_Color){255, 215, 0, 255};
                colorTexto = (SDL_Color){255, 215, 0, 255};
            } else {
                // ESTILO NORMAL: Sin relleno (o negro), Borde Blanco, Texto Blanco
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Relleno negro
                SDL_RenderFillRect(renderer, &box);

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Borde Blanco
                colorBorde = (SDL_Color){255, 255, 255, 255};
                colorTexto = (SDL_Color){255, 255, 255, 255};
            }

            // Dibujar el borde (outline)
            SDL_RenderRect(renderer, &box); 

            // 5. DIBUJAR TEXTO (Centrado en la caja)
            SDL_Surface* surf = TTF_RenderText_Solid(font, opciones[i], 0, colorTexto);
            if(surf) {
                SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
                
                // Matemáticas para centrar texto dentro de la caja:
                // X = CajaX + (AnchoCaja - AnchoTexto) / 2
                SDL_FRect rectTexto = { 
                    box.x + (box.w - surf->w) / 2.0f, 
                    box.y + (box.h - surf->h) / 2.0f, 
                    (float)surf->w, 
                    (float)surf->h 
                };
                
                SDL_RenderTexture(renderer, tex, NULL, &rectTexto);
                SDL_DestroySurface(surf);
                SDL_DestroyTexture(tex);
            }

            // Avanzar Y para el siguiente botón
            yActual += ALTO_BOTON + ESPACIO_ENTRE_BOTONES;
        }

        if(terminoLaMusica()) {
            recargarLaBGM(bgm);
        }

        SDL_RenderPresent(renderer);
    }
}
void mostrarPantallaNombre(SDL_Renderer* renderer, TTF_Font* font, char* nombrePlayer, SDL_Window* window,tSonido* sonidoBotonMenu)
{
    bool escribiendo = true;
    SDL_Event e;
    char buffer[32] = "";
    SDL_Color blanco = {255,255,255,255};
    SDL_RaiseWindow(window);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_StartTextInput(window);
    float mx, my;
    SDL_FRect r3;

    //cuando hay que escribir se usa startTextInput
    SDL_StartTextInput(window);

    //if (!renderer) { printf("[ERROR] renderer es NULL\n");}
    //if (!font) { printf("[ERROR] font es NULL\n");  }


    while (escribiendo) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT)
            {

              escribiendo = false;
            }
            if (e.type == SDL_EVENT_TEXT_INPUT)
            {
               // printf("[DEBUG] evento TEXT_INPUT: \"%s\"\n", e.text.text); fflush(stdout);
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
            if(e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                if(e.button.x >= r3.x && e.button.x <= r3.x + r3.w &&
                         e.button.y >= r3.y && e.button.y <= r3.y + r3.h
                        && strlen(buffer)==MIN_CANTIDAD_LETRAS_NOMBRE)
                         {
                            reproducirSFX(sonidoBotonMenu);
                            escribiendo = false;
                         }
            }
        }


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





        //boton aceptar
        SDL_Surface* s3 = TTF_RenderText_Solid(font,"Aceptar",0,blanco);
        SDL_Texture* t3 = SDL_CreateTextureFromSurface(renderer,s3);
        if(!t3)
        {
            printf("[ERROR] SDL_CreateTextureFromSurface fallo: %s\n", SDL_GetError());
            SDL_Delay(3000);
            SDL_DestroySurface(s1);
        }
        SDL_FRect r4 = { (WIDTH - s3->w)/2, (HEIGHT - 150),s3->w,s3->h};



        //padding acolchado del boton justo aca
        float botonPaddingX  = 200;
        float botonPaddingY = 20;
        SDL_FRect botonAceptar = {r4.x - botonPaddingX, r4.y - botonPaddingY, r4.w + botonPaddingX*2, r4.h + botonPaddingY*2};

        // boton colores
        SDL_GetMouseState(&mx,&my);
        bool hover = (mx >= botonAceptar.x && mx <= botonAceptar.x + botonAceptar.w && my >= botonAceptar.y && my <= botonAceptar.y + botonAceptar.h) ? true : false;


        //Esta parte de aca pregunta si hay o no hover:
        SDL_Color fondoNormal = {40,40,40,255}; //opc 1
        SDL_Color fondoHover  = {70,70,70,255}; // opc 2
        SDL_Color borde       = {80,80,80,255};

        SDL_Color fondo = hover ? fondoHover : fondoNormal;

        //la clave esta en ahora fijar el draw color para el render, sacandolo de los colores que destine antes
        SDL_SetRenderDrawColor(renderer, fondo.r, fondo.g, fondo.b, fondo.a);
        //Fill rect, rellena el rectangulo con los colores seteados.
        SDL_RenderFillRect(renderer, &botonAceptar);


        //lo mismo para el borde:
        SDL_SetRenderDrawColor(renderer, borde.r, borde.g, borde.b, 255);
        //render REct solo renderiza el borde
        SDL_RenderRect(renderer,&botonAceptar);
        //Ahora si renderizo la textura del anterior (porque debe ir arriba del rectangulo.)
        SDL_RenderTexture(renderer,t3,NULL,&r4);

        //importante que el r3 usado para los clicks ahora debe ser el rectangulo, no el texto.
        r3 = botonAceptar;
        SDL_DestroySurface(s3);
        SDL_DestroyTexture(t3);
        //
        SDL_RenderPresent(renderer);
    }
    SDL_StopTextInput(window);
    strcpy(nombrePlayer, buffer);
}

void mostrarPantallaDificultad(SDL_Renderer* renderer, TTF_Font* font, int* dificultad, tSonido* sonidoBotonMenu)
{
    const char* opciones[3] = {
        "1. FACIL",
        "2. MEDIO",
        "3. DIFICIL"
    };

    // Configuración de estilo (Mismos valores que Menu Principal para coherencia)
    const float ANCHO_BOTON = 400.0f;
    const float ALTO_BOTON = 50.0f;
    const float ESPACIO_ENTRE_BOTONES = 20.0f;
    const float Y_INICIO_BOTONES = 250.0f; // Un poco más abajo para dejar lugar al título

    SDL_FRect botones[3];
    int numOpciones = 3;
    bool esperando = true;
    SDL_Event e;
    
    // Colores
    SDL_Color blanco = {255, 255, 255, 255};
    SDL_Color dorado = {255, 215, 0, 255};

    while(esperando)
    {
        // 1. Obtener estado del mouse para Hover
        float mx, my;
        SDL_GetMouseState(&mx, &my);

        while(SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                // Mantenemos tu lógica de salida abrupta por ahora
                exit(0);
            }

            // TECLADO
            if(e.type == SDL_EVENT_KEY_DOWN)
            {
                if(e.key.key >= '1' && e.key.key <= '3')
                {
                    reproducirSFX(sonidoBotonMenu); // ¡Feedback sonoro!
                    esperando = false;
                    switch(e.key.key)
                    {
                        case '1': *(dificultad) = 1; break;
                        case '2': *(dificultad) = 2; break;
                        case '3': *(dificultad) = 3; break;
                    }
                }
            }

            // MOUSE CLICK
            if(e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                float clickX = e.button.x;
                float clickY = e.button.y;

                for(int i = 0; i < numOpciones; i++)
                {
                    SDL_FRect r = botones[i];
                    // Chequeo de colisión con la CAJA, no con el texto
                    if(clickX >= r.x && clickX <= r.x + r.w &&
                       clickY >= r.y && clickY <= r.y + r.h)
                    {
                        reproducirSFX(sonidoBotonMenu);
                        *dificultad = i + 1;
                        esperando = false;
                    }
                }
            }
        }

        // --- RENDERIZADO ---
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // 1. DIBUJAR TÍTULO (Fijo arriba)
        SDL_Surface* surfTitulo = TTF_RenderText_Solid(font, "Selecciona la dificultad:", 0, blanco);
        if (surfTitulo) {
            SDL_Texture* texTitulo = SDL_CreateTextureFromSurface(renderer, surfTitulo);
            SDL_FRect rectTitulo = { (WIDTH - surfTitulo->w)/2, 100, surfTitulo->w, surfTitulo->h };
            SDL_RenderTexture(renderer, texTitulo, NULL, &rectTitulo);
            SDL_DestroySurface(surfTitulo);
            SDL_DestroyTexture(texTitulo);
        }

        // 2. DIBUJAR BOTONES
        float yActual = Y_INICIO_BOTONES;

        for (int i = 0; i < numOpciones; i++) {
            
            // Definir Caja
            SDL_FRect box = { 
                (WIDTH - ANCHO_BOTON) / 2.0f, 
                yActual, 
                ANCHO_BOTON, 
                ALTO_BOTON 
            };
            botones[i] = box; // Guardar para el click

            // Detectar Hover
            bool isHover = (mx >= box.x && mx <= box.x + box.w && 
                            my >= box.y && my <= box.y + box.h);

            SDL_Color colorTexto;

            if (isHover) {
                // Hover: Relleno gris, Borde dorado, Texto dorado
                SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
                SDL_RenderFillRect(renderer, &box);
                
                SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
                colorTexto = dorado;
            } else {
                // Normal: Relleno negro, Borde blanco, Texto blanco
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &box); // Limpia el fondo del botón
                
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                colorTexto = blanco;
            }

            // Dibujar borde
            SDL_RenderRect(renderer, &box);

            // Dibujar Texto Centrado en la Caja
            SDL_Surface* surf = TTF_RenderText_Solid(font, opciones[i], 0, colorTexto);
            if (surf) {
                SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
                SDL_FRect rectTexto = { 
                    box.x + (box.w - surf->w) / 2.0f, 
                    box.y + (box.h - surf->h) / 2.0f, 
                    (float)surf->w, 
                    (float)surf->h 
                };
                SDL_RenderTexture(renderer, tex, NULL, &rectTexto);
                SDL_DestroySurface(surf);
                SDL_DestroyTexture(tex);
            }

            yActual += ALTO_BOTON + ESPACIO_ENTRE_BOTONES;
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

    tSonido bgm;
    if(!crearBackgroundMusic(RUTA_BACKGROUND_MUSIC_HISTORIA_PRINCIPAL,&bgm))
    {
        printf("[DEBUG]: No se encontro la musica de la historia principal: %s", SDL_GetError());
    }
    reproducirBGM(&bgm);

    FILE* archivo = fopen(RUTA_HISTORIA_INICIAL, "r");
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
                if((e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_RETURN)
                 || (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN))
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
            if(terminoLaMusica())
            {
                recargarLaBGM(&bgm);
            }
            SDL_RenderPresent(renderer);
        }

        fclose(archivo);
        *(retorno) = TODO_OK;
    }
    detenerMusicaBGM();
   if(!liberarBGM(&bgm))
   {
    printf("[DEBUG]: no pude liberar bgm: %s", SDL_GetError());
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
            if(e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
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
void mostrarCreditosEnPantalla(SDL_Renderer* renderer, size_t* retorno)
{
    //aqui abro el archivo.
    FILE* archivo = fopen(RUTA_CREDITOS, "r");
    if (!archivo) {
        printf("[ERROR] No se pudo abrir el archivo de creditos: %s\n", RUTA_CREDITOS);
        return;
    }
    TTF_Font* font = TTF_OpenFont(RUTA_FONT_CREDITOS, TAMANO_FONT_CREDITOS);
    if (!font) {
        printf("[ERROR] No se pudo cargar la fuente de creditos: %s\n", SDL_GetError());
        fclose(archivo);
        return;
    }
    LineaCredito lineas[200]; // Asumimos un maximo de 200 lineas de creditos
    int totalLineas = 0;
    char bufferLectura[256];
    SDL_Color colorBlanco = {255, 255, 255, 255};


    //parte musica
    tSonido bgm;
    if(!crearBackgroundMusic(RUTA_BACKGROUND_MUSIC_CREDITOS,&bgm))
    {
        printf("[DEBUG]: No se encontro la musica de los creditos: %s", SDL_GetError());
    }
    else
    {
        reproducirBGM(&bgm);
        bajarElVolumenDeLaMusica(0.4f);
    }

    // Leemos el archivo y creamos las texturas
    while (fgets(bufferLectura, sizeof(bufferLectura), archivo))
    {
        bufferLectura[strcspn(bufferLectura, "\r\n")] = '\0';


        //chequear lineas corruptas (copiado y pegado de otro lado)
        if (strlen(bufferLectura) > 0) {
            SDL_Surface* surf = TTF_RenderText_Blended(font, bufferLectura, 0, colorBlanco);
            if (surf) {
                lineas[totalLineas].textura = SDL_CreateTextureFromSurface(renderer, surf);
                lineas[totalLineas].w = (float)surf->w;
                lineas[totalLineas].h = (float)surf->h;
                SDL_DestroySurface(surf);
            }
        } else {
            // Linea vacía (espaciador)
            lineas[totalLineas].textura = NULL;
            lineas[totalLineas].h = 20; // Altura del espacio vacío
        }
        totalLineas++;
    }
    fclose(archivo);


    float scrollY = (float)HEIGHT; // Comenzar desde abajo de la pantalla

    bool enCreditos = true;
    SDL_Event e;

    while (enCreditos)
    {
        // Procesar salida anticipada
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                exit(0);
            }
            if (e.type == SDL_EVENT_KEY_DOWN || e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                enCreditos = false; // Salir si tocan algo
            }
        }

        // Actualizar posición (Mover hacia arriba)
        //esta es la clave
        scrollY -= VELOCIDAD_SCROLL;

        // Limpiar pantalla
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Dibujar líneas
        float yActual = scrollY;
        float alturaTotalBloque = 0; // Para saber cuándo termina todo

        for (int i = 0; i < totalLineas; i++)
        {
            // Solo dibujar si está visible en pantalla (Optimization Culling)
            // Si yActual + altura > 0  Y  yActual < AltoPantalla
            if (yActual + lineas[i].h > 0 && yActual < HEIGHT)
            {
                if (lineas[i].textura) {
                    // Centrar horizontalmente
                    float x = (WIDTH - lineas[i].w) / 2;
                    SDL_FRect rect = { x, yActual, lineas[i].w, lineas[i].h };
                    SDL_RenderTexture(renderer, lineas[i].textura, NULL, &rect);
                }
            }

            // Avanzamos la Y para la siguiente línea
            float altoLinea = (lineas[i].textura) ? lineas[i].h : 20; // Altura texto o espacio
            yActual += altoLinea + ESPACIADO_ENTRE_LINEAS;

            // Calculamos dónde termina realmente el último texto
            if (i == totalLineas - 1) {
                alturaTotalBloque = yActual;
            }
        }

        // Chequear si la última línea ya salió por arriba de la pantalla
        // (alturaTotalBloque es la posición Y donde termina el último texto)
        if (alturaTotalBloque < 0) {
            enCreditos = false;
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // Aprox 60 FPS
    }
    //ahora para limpiar todo
    for (int i = 0; i < totalLineas; i++) {
        if (lineas[i].textura) {
            SDL_DestroyTexture(lineas[i].textura);
        }
    }
    *retorno = TODO_OK;
    restaurarVolumenMusica();
    detenerMusicaBGM();
    TTF_CloseFont(font);
    liberarBGM(&bgm);
    printf("[DEBUG] Creditos finalizados.\n");
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

    while(estaLeyendo)
    {
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_EVENT_QUIT)
            {
                //CORREGIR ESETO, NO PUEDE SALIR ASI NO MAS DLE PROGRAMA
                return;
            }
            if(e.type == SDL_EVENT_KEY_DOWN)
            {
                estaLeyendo = false;
            }
            if(e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
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
