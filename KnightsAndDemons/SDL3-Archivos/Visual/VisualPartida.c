#include "VisualPartida.h"

int jugar(Admin* manager,  SDL_Renderer* renderer, TTF_Font* font,tSonido* sonidoBotonCasilla)
{
    int finalJuego;
    //barraDeCarga();
    finalJuego = ciclarPartida(manager, renderer,sonidoBotonCasilla);
    if(seCompletoElJuego(&finalJuego)) //Este if pregunta si el jugador gano todos los niveles
    {
         chequearYMostrarFinalDelJuego(manager, renderer);
        mostrarCreditosEnPantalla(renderer, font, NULL);
        return 0; //el jugador completo el juego
    }
    //barraDeCarga();

    return 1; //El jugador no completo el juego
}
int ciclarPartida(Admin* admin, SDL_Renderer* renderer,tSonido* sonidoBotonCasilla)// CICLA TODA LA PARTIDA (1 NIVEL A LA VEZ) HASTA QUE SALGA AL MENÚ
{
    int finalJuego = 0;
    int gano;
    int estaEnMenu = 0;
    int respuesta;
    int cantPikas;
    //parte sdl
    TTF_Font* fontMensajeInicioNivel = TTF_OpenFont(RUTA_FUENTE_MENSAJE_INICIO_NIVEL, TAMANIO_TEXTO_INICIO_NIVEL);
    if(!fontMensajeInicioNivel)
    {
        printf("[DEBUG] Error al cargar la fuente de mensaje de inicio de nivel: %s", SDL_GetError());
    }
    SDL_Surface* sKnight = SDL_LoadBMP(RUTA_IMAGEN_CABALLERO);
    SDL_Surface* sDemon = SDL_LoadBMP(RUTA_IMAGEN_DEMONIO);
    if(!sKnight || !sDemon)
    {
        printf("[DEBUG] Error al cargar las imagenes: %s", SDL_GetError());
    }
    SDL_Texture* textKnight = SDL_CreateTextureFromSurface(renderer,sKnight);
    SDL_DestroySurface(sKnight);
    SDL_Texture* textDemon = SDL_CreateTextureFromSurface(renderer, sDemon);
    SDL_DestroySurface(sDemon);

    while(!estaEnMenu && finalJuego == 0)
    {
        mostrarMensajeDeInicioDeNivel(renderer, fontMensajeInicioNivel);


        gano = iniciarPartidaConSDL(admin, textKnight,textDemon,renderer,sonidoBotonCasilla);




        if(noPerdio(&gano)) //Por lo menos no perdió, entra dentro de este if
        {
            if(gano == GANO_KNIGHTS)
            {

                admin->jugador.TotalestadoUno++;
                admin->jugador.pikasRestantes+=PREMIO_PIKAS;
                //mostrarMensajeEnVentanaYBorrarDespuesDeTecla(renderer, fontMensajeInicioNivel, "¡Los caballeros están agradecidos!");
                char mensajePikas[50];
                sprintf(mensajePikas, "Has obtenido %d pikas extras", PREMIO_PIKAS);
                mostrarAgradecimientoCaballeros(renderer, fontMensajeInicioNivel, mensajePikas);
                //mostrarMensajeEnVentanaYBorrarDespuesDeTecla(renderer, fontMensajeInicioNivel, mensajePikas);
                cantPikas = admin->jugador.pikasRestantes; // Se recalcula su valor
            }

            // gano == GANO_DEMONS
            else{
                admin->jugador.TotalestadoDos++;
                mostrarMensajeEnVentanaYBorrarDespuesDeTecla(renderer, fontMensajeInicioNivel, "¡Los demonios están avanzando!");
                cantPikas = admin->jugador.pikasRestantes; // Se recalcula su valor
            }
            admin->jugador.nivelesCompletados+=1;
            admin->niveles[admin->jugador.nivelActual].estadoCompletado = 1; // Actualizo el estado del nivel a ganó o no ganó (1 o 0)
            admin->jugador.nivelActual++;


           preguntarGuardarPartida(&respuesta, renderer, fontMensajeInicioNivel);
            if(respuesta == 1)
            {
                if(guardarPartida(admin)!=0)
                {
                    mostrarMensajeEnVentanaYBorrarDespuesDeTecla(renderer, fontMensajeInicioNivel, "Partida No se pudo Guardar Correctamente.");
                    //corregir tipo retorno aca
                }
                else
                {
                    mostrarMensajeEnVentanaYBorrarDespuesDeTecla(renderer, fontMensajeInicioNivel, "Partida Guardada Correctamente.");
                }
            }

        }
        else
        {
            //Significa que perdió, por eso se muestra este mensaje de derrota
            mostrarMensajeEnVentanaYBorrarDespuesDeTecla(renderer, fontMensajeInicioNivel, MENSAJE_DERROTA);

            // Se reinician las pikas a las que tenía al iniciar el nivel
            if(admin->jugador.nivelActual > 1)
                admin->jugador.pikasRestantes = cantPikas;
            else
            {
                switch(admin->jugador.dificultadSeleccionada)
                {
                    case FACIL:
                        admin->jugador.pikasRestantes = PIKAS_INICIALES_FACIL;
                        break;

                    case MEDIO:
                        admin->jugador.pikasRestantes = PIKAS_INICIALES_MEDIO;
                        break;

                    case DIFICIL:
                        admin->jugador.pikasRestantes = PIKAS_INICIALES_MEDIO;
                        break;
                }
            }

        }

        if(admin->jugador.nivelActual < TAM_PARTIDAS) //Se evalúa si quedan niveles por jugar o no.
        {
            //estaEnMenu = postNivel(admin, gano);
            if(postNivelUsandoSDL(admin, gano, renderer, fontMensajeInicioNivel) == 1)
                finalJuego = 1; // SE VA A MENU
        }
        else
        {
            //Significa que se completo el juego, con todos sus niveles al menos
            finalJuego = 2; // GANO
        }
    }

    return finalJuego;
}
int iniciarPartidaConSDL(Admin* admin, SDL_Texture* textKnight, SDL_Texture* textDemon, SDL_Renderer* renderer,tSonido* sonidoBotonCasilla) // DEVUELVE SI GANÓ O NO (1,2 o 0 si perdio el útlimo)
{


    TTF_Font* font = TTF_OpenFont(RUTA_FUENTE_TIEMPO_RESTANTE, TAMANIO_TEXTO_TIEMPO_RESTANTE);
    int gano = 0;
    int pikasDeInicio = admin->jugador.pikasRestantes;

    Uint64 tiempoInicio = SDL_GetTicks();
    Uint64 tiempoDelNivel = tiempo(admin,T_INICIAL);
    Uint64 tiempoRestante = tiempoDelNivel;

    size_t tamTablero = obtenerTamTablero(admin);
    char** tablero = NULL;
    printf("[DEBUG]: ME LLEGUE A METER EN INICIAR PARTIDA");

    int mouseX;
    int mouseY;

    SDL_Event e;
    bool estaJugando = true;

    int hoverEnFila = -1; //es decir no hay
    int hoverEnColumna = -1;

    tablero = crearTablero(tamTablero);
    inicializarTablero(tamTablero, tablero, &admin->cursor);
    //parte barra de estado:
    tRecursosHUD barraDeEstado;
    inicializarHUD(renderer,&barraDeEstado);
    //parte VFX: fuego
    tVFX VFXfuego;
    inicializarVFX(renderer,&VFXfuego,1,RUTA_VFX_FUEGO,RUTA_FUENTE_PIKAS_RESTANTES,TAMANIO_FUENTE_PIKAS_RESTANTES);
    tSonido sonidoBotonPika;
    if(!cargarUnSonidoNuevo(RUTA_SONIDO_BOTON_PIKA,&sonidoBotonPika))
    {
        SDL_Log("No se pudo cargar el sonido de pikas al presionar casilla: %s", SDL_GetError());
    }

    //parteSonido
    tSonido bgm;
    if(!crearBackgroundMusic(RUTA_BACKGROUND_MUSIC_NIVEL,&bgm))
    {
        printf("[DEBUG]: Error creando bgm en nivel: %s\n", SDL_GetError());
    }


    reproducirBGM(&bgm);
    bajarElVolumenDeLaMusica(0.2);
    while(estaJugando)
    {
        //printf("[DEBUG]: ME LLEGUE A METER EN EL WHILE DE INICIAR PARTIDA");
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_EVENT_QUIT)
            {
                estaJugando = false;
            }
            if(e.type == SDL_EVENT_MOUSE_MOTION)
            {
                //printf("[DEBUG]: ME LLEGUE A METER EN EL EVENTO MOUSE MOTION");
                mouseX =  e.motion.x;
                mouseY  = e.motion.y;
                actualizarHoverCelda(&hoverEnFila,&hoverEnColumna,mouseX,mouseY,tamTablero,TAMANIO_CELDA);
            }
            if(e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                mouseX =  e.motion.x;
                mouseY  = e.motion.y;
                //primero chequear que celda fue.
                //printf("[DEBUG]: mousex: %d , mouseY: %d \n", mouseX, mouseY);
                if(hizoClickEnTablero(mouseX,mouseY,tablero,tamTablero, TAMANIO_CELDA))
                {
                    //printf("[DEBUG]: TABLERO");
                    //deben ir al reves por eso los invierto
                    int posX = (mouseX - (WIDTH - tamTablero*TAMANIO_CELDA)/2)/TAMANIO_CELDA;
                    int posY = (mouseY - (HEIGHT - tamTablero*TAMANIO_CELDA)/2)/TAMANIO_CELDA;
                    //printf("[DEBUG]: posX: %d , posY: %d \n", posX, posY);




                    if(e.button.button == SDL_BUTTON_LEFT)
                    {
                        //reproduce sonido
                        reproducirSFX(sonidoBotonCasilla);

                        actualizarCursor(&(admin->cursor), posX, posY, false);
                        gano = ejecutarJugadaSDL(tamTablero, tablero, TAMANIO_CELDA, &(admin->cursor));
                    }
                    if(e.button.button == SDL_BUTTON_RIGHT)
                    {
                        // ESTA PARTE ES PARA EL EFECTO VISUAL JUICE del fuego:
                        activarEfectoFuego(&VFXfuego);

                        //fin efecto
                        //reproducir sonido pero distinto
                        reproducirSFX(&sonidoBotonPika);

                        //printf("[DEBUG]: ME LLEGUE A METER EN EL EVENTO MOUSE BUTTON DOWN y entre al if del tablero");
                        if(tienePikas(&pikasDeInicio))
                        {
                            restarUnaPika(&pikasDeInicio);
                            actualizarCursor(&(admin->cursor), posX, posY, true);
                            gano = ejecutarJugadaSDL(tamTablero, tablero, TAMANIO_CELDA, &(admin->cursor));
                        }
                        else
                        {
                            //no tiene pikas habría que avisarle.
                            renderizarMensajeDeQueNoHayPikas(renderer,font);
                        }
                        admin->cursor.seUsoPika = false;
                    }
                    if(gano)
                    {
                        estaJugando = false;
                    }
                }
            }

        }
        Uint64 ahora = SDL_GetTicks();
        Uint64 transcurrido = ahora - tiempoInicio;
        tiempoRestante = tiempoDelNivel - (transcurrido/1000);
        //tengo que dividirlo en 1000 para cquitar los milisegundos

        if(tiempoRestante <= 0)
        {
            //es decir se quedo sin tiempo
            gano = 0;
            estaJugando = false;
        }
        //limpio todo por las dudas que no quede nada en el render
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);

        renderizarTableroConSDL(renderer,
                          tablero,
                          tamTablero,
                          textKnight,
                          textDemon,
                          TAMANIO_CELDA,
                          true,
                          hoverEnFila,
                          hoverEnColumna);
            //CORREGIR BARRA DE TIEMOPOOOO
        renderizarBarraEstadoSDL(renderer,&barraDeEstado,admin->jugador.TotalestadoUno,admin->jugador.TotalestadoDos);

        renderizarContadoresSDL(renderer, tiempoRestante, font);

        renderizarPikasRestantes(renderer,&VFXfuego,pikasDeInicio,HEIGHT);

        SDL_RenderPresent(renderer);
        //justo acá preguntar si termino la musica
        if(terminoLaMusica())
        {
            recargarLaBGM(&bgm);
        }
        //limitar fps, copiado y pegado de stackoverflow
        SDL_Delay(16);
    }

    liberarMemoriaTablero(tablero, tamTablero);

   if(gano)
   {
        actualizarPikas(&pikasDeInicio, &(admin->jugador.pikasRestantes));
        admin->niveles[admin->jugador.nivelActual].tiempo = tiempoRestante; // Establezco el tiempo del jugador en el nivel
   }
   //justo aca corto la musica:
   restaurarVolumenMusica();
   detenerMusicaBGM();
   liberarBGM(&bgm);
   //justo aca elimino la barra de estado:
   liberarHUD(&barraDeEstado);
   //justo aca elimino el sound effect de pika
   liberarSonido(&sonidoBotonPika);

    return gano;
}
void actualizarHoverCelda(int* hoverEnFila,int* hoverEnColumna,int mouseX,int mouseY,const size_t tamTablero,const int tamCelda)
{
    float offsetEnX = (WIDTH - (tamTablero*tamCelda))/2;//240;
    float offsetEnY = (HEIGHT - (tamTablero*tamCelda))/2;//140;
    //printf("El tam x: %.2f el tam y: %.2f\n", offsetEnX,offsetEnY);
    //este if basicamente pregunta si estoy dentro de la grilla, por muy raro que parezca.
    //el offset es donde empieza la grilla
    //si le sumo tamTablero*tamCelda, sumo toda la linea o toda la columna, miden lo mismo
     if (mouseX < offsetEnX || mouseX > offsetEnX + (tamTablero*tamCelda) ||
        mouseY < offsetEnY || mouseY > offsetEnY + (tamTablero*tamCelda))
    {
        //printf("[DEBUG]: mouseX: %d, mouseY: %d\n", mouseX, mouseY);
        *hoverEnFila = -1;
        *hoverEnColumna  = -1;
    }
    else
    {

        //la nueva posicion de la matriz la pueda sacar por una resta
        // entre las coordenas cartesianas
        //luego lo divido por el tamaño de cada celda
        //todo aritmetica


        // EL HOVER TIENE QUE IR AL REVES
        *hoverEnFila = mouseY - offsetEnY;
        *hoverEnFila = (*hoverEnFila)/tamCelda;
        *hoverEnColumna = mouseX - offsetEnX;
        *hoverEnColumna = (*hoverEnColumna)/tamCelda;
       // printf("[DEBUG]: hoverEnFila: %d , hoverEnColumna: %d \n", *hoverEnFila, *hoverEnColumna);
    }
}

void renderizarContadoresSDL(SDL_Renderer* renderer, const int tiempo, TTF_Font* font)
{
    //printf("[DEBUG]: tiempo restante: %d \n", tiempo);
    char texto[70]="";
    SDL_Color blanco = {255,255,255,255};
    sprintf(texto,"Tiempo Restante: %d", tiempo);
    SDL_Surface* surf = TTF_RenderText_Solid(font,texto,0,blanco);
    SDL_Texture* text = SDL_CreateTextureFromSurface(renderer,surf);
    int y = 100;
    SDL_FRect rect = {(WIDTH - surf->w)/2, (HEIGHT - y), surf->w, surf->h};
    SDL_RenderTexture(renderer,text,NULL,&rect);
    SDL_DestroySurface(surf);
    SDL_DestroyTexture(text);
}
void renderizarMensajeDeQueNoHayPikas(SDL_Renderer* renderer, TTF_Font* font)
{
    char texto[70]="¡No tienes más Pikas para usar!";
    SDL_Color blanco = {255,255,255,255};
    SDL_Surface* surf = TTF_RenderText_Solid(font,texto,0,blanco);
    SDL_Texture* text = SDL_CreateTextureFromSurface(renderer,surf);
    int y = 300;
    SDL_FRect rect = {(WIDTH - surf->w)/2, (HEIGHT - y), surf->w, surf->h};
    SDL_RenderTexture(renderer,text,NULL,&rect);
    SDL_DestroySurface(surf);
    SDL_DestroyTexture(text);
    SDL_Delay(60);
}
void  mostrarMensajeDeInicioDeNivel(SDL_Renderer* renderer, TTF_Font*   fontMensajeInicioNivel)
{
    char texto[70]="<< Una nueva Batalla Comienza... >>";
    SDL_RenderClear(renderer);
    SDL_Color blanco = {255,255,255,255};
    SDL_Surface* surf = TTF_RenderText_Solid(fontMensajeInicioNivel,texto,0,blanco);
    SDL_Texture* text = SDL_CreateTextureFromSurface(renderer,surf);
    SDL_FRect rect = {(WIDTH - surf->w)/2, (HEIGHT - surf->h)/2, surf->w, surf->h};
    SDL_RenderTexture(renderer,text,NULL,&rect);
    SDL_DestroySurface(surf);
    SDL_DestroyTexture(text);
    SDL_RenderPresent(renderer);
    SDL_Delay(1500);
}
void preguntarGuardarPartida(int* respuesta, SDL_Renderer* renderer, TTF_Font* font)
{
    bool estaLeyendo = true;
    SDL_Color blanco = {255,255,255,255};
    SDL_Event e;
    SDL_FRect aceptarRect;
    SDL_FRect cancelarRect;
    while(estaLeyendo)
    {
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_EVENT_QUIT)
            {
                estaLeyendo = false;
                *respuesta = 2; //por las dudas que quiera salir, no guardar
            }
            if(e.type == SDL_EVENT_KEY_DOWN)
            {
                if(e.key.key == '1')
                {
                    *respuesta = 1;
                    estaLeyendo = false;
                }
                else if(e.key.key == '2')
                {
                    *respuesta = 2;
                    estaLeyendo = false;
                }
            }
            if(e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                if(e.button.button == SDL_BUTTON_LEFT)
                {
                    int mouseX =  e.motion.x;
                    int mouseY  = e.motion.y;
                    //ahora cual boton
                    if(mouseX >= aceptarRect.x && mouseX <= (aceptarRect.x + aceptarRect.w) &&
                       mouseY >= aceptarRect.y && mouseY <= (aceptarRect.y + aceptarRect.h))
                    {
                        //aceptar
                        *respuesta = 1;
                        estaLeyendo = false;
                    }
                    else if(mouseX >= cancelarRect.x && mouseX <= (cancelarRect.x + cancelarRect.w) &&
                            mouseY >= cancelarRect.y && mouseY <= (cancelarRect.y + cancelarRect.h))
                    {
                        //cancelar
                        *respuesta = 2;
                        estaLeyendo = false;
                    }
                }
            }
        }
            SDL_SetRenderDrawColor(renderer,0,0,0,255);
            SDL_RenderClear(renderer);
            SDL_Surface* s2 = TTF_RenderText_Solid(font,"Aceptar", 0,blanco);
            SDL_Surface* s3 = TTF_RenderText_Solid(font,"Cancelar", 0,blanco);
            SDL_Texture* t2 = SDL_CreateTextureFromSurface(renderer,s2);
            SDL_Texture* t3 = SDL_CreateTextureFromSurface(renderer,s3);
            int y = 100;
            SDL_FRect r2 = { WIDTH/2 - 100 , (HEIGHT - y), s2->w, s2->h };
            SDL_FRect r3 = { WIDTH/2 + 50 , (HEIGHT - y), s3->w, s3->h };
            SDL_RenderTexture(renderer,t2,NULL,&r2);
            SDL_RenderTexture(renderer,t3,NULL,&r3);
            aceptarRect = r2;
            cancelarRect = r3;

            SDL_Surface* surf = TTF_RenderText_Solid(font,"¿Desea guardar la partida?",0, blanco);
            SDL_Texture* text = SDL_CreateTextureFromSurface(renderer,surf);
            SDL_FRect rect = { 80,(HEIGHT - surf->h)/2, surf->w, surf->h };
            SDL_RenderTexture(renderer,text,NULL,&rect);
            SDL_DestroyTexture(text);
            SDL_DestroySurface(surf);
            SDL_RenderPresent(renderer);

    }
}
int postNivelUsandoSDL(Admin* admin, int resultado, SDL_Renderer* renderer, TTF_Font* font) // DEVUELVE SI SE VA O NO AL MENÚ (0 o 1)
{
    int respuesta;
    bool esperandoRespuesta;
    SDL_Event e;
    SDL_Color blanco = {255,255,255,255};
    SDL_FRect aceptarRect;
    SDL_FRect cancelarRect;

    //parte sonido
    tSonido sonidoBoton;
    if(!cargarUnSonidoNuevo(RUTA_SONIDO_BOTON_MENU,&sonidoBoton))
    {
        SDL_Log("No se pudo cargar el sonido de boton en postNivelSDL: %s", SDL_GetError());
    }


    while(esperandoRespuesta)
    {
        if(SDL_PollEvent(&e))
        {
            if(e.type==SDL_EVENT_QUIT)
            {
                esperandoRespuesta = false;
            }
            if(e.type == SDL_EVENT_KEY_DOWN)
            {
                int tecla = e.key.key;
                if(tecla == '1')
                {
                   respuesta = 1;
                   esperandoRespuesta = false;
                }
                if(tecla == '2')
                {
                    respuesta = 2;
                    esperandoRespuesta = false;
                }
            }
            if(e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                int mouseX =  e.motion.x;
                int mouseY  = e.motion.y;
                //ahora cual boton
                if(mouseX >= aceptarRect.x && mouseX <= (aceptarRect.x + aceptarRect.w) &&
                   mouseY >= aceptarRect.y && mouseY <= (aceptarRect.y + aceptarRect.h))
                {
                        //aceptar
                        reproducirSFX(&sonidoBoton);
                        respuesta = 1;
                    esperandoRespuesta = false;
                }
                else if(mouseX >= cancelarRect.x && mouseX <= (cancelarRect.x + cancelarRect.w) &&
                        mouseY >= cancelarRect.y && mouseY <= (cancelarRect.y + cancelarRect.h))
                {
                    //cancelar
                    reproducirSFX(&sonidoBoton);
                    respuesta = 2;
                    esperandoRespuesta = false;
                }
            }
        }
        SDL_RenderClear(renderer);
        SDL_Surface* surf = TTF_RenderText_Solid(font,"¿Desea volver al menu?",0, blanco);
        SDL_Surface* s2 = TTF_RenderText_Solid(font,"Continuar", 0,blanco);
        SDL_Surface* s3 = TTF_RenderText_Solid(font,"Volver al menú", 0,blanco);
        if(!surf || !s2 || !s3)
        {
            printf("[DEBUG]: Error creando superficies en postNivelSDL: %s\n", SDL_GetError());
        }
        SDL_Texture* text = SDL_CreateTextureFromSurface(renderer,surf);
        SDL_Texture* t2 = SDL_CreateTextureFromSurface(renderer,s2);
        SDL_Texture* t3 = SDL_CreateTextureFromSurface(renderer,s3);
        if(!text || !t2 || !t3)
        {
            printf("[DEBUG]: Error creando texturas en postNivelSDL: %s\n", SDL_GetError());
        }
        int y = 100;
        SDL_FRect rect = { (WIDTH - surf->w)/2, (HEIGHT - surf->h)/2, surf->w, surf->h };
        SDL_FRect r2 = { WIDTH/2 - 100 , (HEIGHT - y), s2->w, s2->h };
        SDL_FRect r3 = { WIDTH/2 + 50 , (HEIGHT - y), s3->w, s3->h };
        aceptarRect = r2;
        cancelarRect = r3;
        SDL_RenderTexture(renderer,text,NULL,&rect);
        SDL_RenderTexture(renderer,t2,NULL,&r2);
        SDL_RenderTexture(renderer,t3,NULL,&r3);

        SDL_DestroySurface(surf);
        SDL_DestroySurface(s2);
        SDL_DestroySurface(s3);
        SDL_DestroyTexture(text);
        SDL_DestroyTexture(t2);
        SDL_DestroyTexture(t3);
        SDL_RenderPresent(renderer);
    }
    liberarSonido(&sonidoBoton);
    return (respuesta-1); // El -1 es para que devuelva 1 o 0 en vez de 1 o 2
}
bool inicializarHUD(SDL_Renderer* renderer, tRecursosHUD* hud)
{
    // Cargar texturas desde una surface
    SDL_Surface* surf2 = SDL_LoadBMP(RUTA_IMAGEN_BARRA_DE_ESTADO_DEMONS);
    SDL_Surface* surf1 = SDL_LoadBMP(RUTA_IMAGEN_BARRA_DE_ESTADO_KNIGHTS);
    if(!surf2 || !surf1)
    {
        printf("[DEBUG]: No se encontraron las imagenes de la barra de estado: %s\n", SDL_GetError());
        return false;
    }
    hud->texK = SDL_CreateTextureFromSurface(renderer,surf1);
    hud->texD = SDL_CreateTextureFromSurface(renderer,surf2);

    if(!(hud->texK) || !(hud->texD))
    {
        printf("[DEBUG]: No se encontraron las texturas de la barra de estado: %s\n", SDL_GetError());
        return false;
    }

    // Cargar fuente (Ajusta la ruta y tamaño)
    hud->fuente = TTF_OpenFont(RUTA_FUENTE_BARRA_DE_ESTADO, TAMANIO_FUENTE_BARRA_DE_ESTADO);

    if (!hud->fuente) {
        printf("Error cargando HUD: %s", SDL_GetError());
        return false;
    }
    return true;
}
void renderizarBarraEstadoSDL(SDL_Renderer* renderer, tRecursosHUD* hud, int scoreCaballeros, int scoreDemonios)
{
    // --- 1. CÁLCULO MATEMÁTICO (Tu lógica original) ---

    // El punto medio en PIXELES
    float mitadAncho = ANCHO_BARRA / 2.0f;

    int diferencia = scoreCaballeros - scoreDemonios;

    // Cuántos píxeles ganan por cada victoria
    float pixelesPorVictoria = mitadAncho / TAM_PARTIDAS;

    // Calculamos el ancho de la barra de caballeros
    float anchoCaballeros = mitadAncho + (diferencia * pixelesPorVictoria);

    // Clamping (Límites para que no se salga de dibujo)
    if (anchoCaballeros < 0) anchoCaballeros = 0;
    if (anchoCaballeros > ANCHO_BARRA) anchoCaballeros = ANCHO_BARRA;

    // El resto es para los demonios
    float anchoDemonios = ANCHO_BARRA - anchoCaballeros;


    // --- 2. RENDERIZADO DE BARRAS ---

    // Dibujar parte Caballeros (Izquierda) -> Espada
    if (anchoCaballeros > 0)
    {
        //una forma rectangular obvio
        SDL_FRect rectEspada =
        {
            POS_X_BARRA,
            POS_Y_BARRA,
            anchoCaballeros,
            ALTO_BARRA
        };
        SDL_RenderTexture(renderer, hud->texK, NULL, &rectEspada);
    }

    // parte Demonios (Derecha)
    if (anchoDemonios > 0) {
        SDL_FRect rectGarra = {
            // Empieza donde termina la parte de los caballeros
            POS_X_BARRA + anchoCaballeros,
            POS_Y_BARRA,
            anchoDemonios,
            ALTO_BARRA
        };
        SDL_RenderTexture(renderer, hud->texD, NULL, &rectGarra);
    }

    //una linea para separar los dos
    /*SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderLine(renderer, POS_X_BARRA + (ANCHO_BARRA/2), POS_Y_BARRA - 5, POS_X_BARRA + (ANCHO_BARRA/2), POS_Y_BARRA + ALTO_BARRA + 5);
    */

    // --- 3. RENDERIZADO DE TEXTO (Frentes) ---

    SDL_Color colorTexto = {255, 255, 255, 255};
    char bufferTexto[64];

    // Texto Caballeros (Izquierda abajo)
    sprintf(bufferTexto, "%d Frentes", scoreCaballeros);
    SDL_Surface* surfK = TTF_RenderText_Solid(hud->fuente, bufferTexto, 0, colorTexto);
    if(!surfK)
    {
        SDL_Log("Error creating surface for Knights: %s", SDL_GetError());
    }
        SDL_Texture* texK = SDL_CreateTextureFromSurface(renderer, surfK);
        SDL_FRect rectK = { POS_X_BARRA, POS_Y_BARRA + ALTO_BARRA + 5, (float)surfK->w, (float)surfK->h };
        SDL_RenderTexture(renderer, texK, NULL, &rectK);
        SDL_DestroySurface(surfK);
        SDL_DestroyTexture(texK);

    // Texto Demonios (Derecha abajo)
    sprintf(bufferTexto, "%d Frentes", scoreDemonios);
    SDL_Surface* surfD = TTF_RenderText_Solid(hud->fuente, bufferTexto, 0, colorTexto);
    if (surfD) {
        SDL_Texture* texD = SDL_CreateTextureFromSurface(renderer, surfD);
        // Alineado a la derecha: X = (FinBarra) - AnchoTexto
        SDL_FRect rectD = { (POS_X_BARRA + ANCHO_BARRA) - surfD->w, POS_Y_BARRA + ALTO_BARRA + 5, (float)surfD->w, (float)surfD->h };
        SDL_RenderTexture(renderer, texD, NULL, &rectD);
        SDL_DestroySurface(surfD);
        SDL_DestroyTexture(texD);
    }
}
void liberarHUD(tRecursosHUD* hud)
{
    if(hud->texK)
        SDL_DestroyTexture(hud->texK);
    if(hud->texD)
        SDL_DestroyTexture(hud->texD);
    if(hud->fuente)
        TTF_CloseFont(hud->fuente);
}
bool inicializarVFX(SDL_Renderer* renderer, tVFX* vfx, const int id, const char* rutaIMG, const char* rutaFuente, const int tamFuente)
{
    vfx->id = id;
    vfx->alpha = 0;

    vfx->font = TTF_OpenFont(rutaFuente,tamFuente);
    if(!vfx->font)
    {
        vfx->id = 0;
        printf("[DEBUG]: No se encontro la ruta del font %d code: %s\n", id, SDL_GetError());
        return false;
    }
    SDL_Surface* surf = SDL_LoadBMP(rutaIMG);
    if(!surf)
    {
        vfx->id = 0;
        printf("[DEBUG]: No se encontro la ruta del vfx %d code: %s\n", id, SDL_GetError());
        return false;
    }
    vfx->text = SDL_CreateTextureFromSurface(renderer,surf);
    if(!vfx->text)
    {
        SDL_DestroySurface(surf);
        vfx->id = 0;
        printf("[DEBUG]: No se pudo crear la text del vfx %d code: %s\n", id, SDL_GetError());
        return false;
    }
    return true;
}
void activarEfecto(tVFX* vfx, const int efecto)
{
    switch(efecto)
    {
        case 1:
        vfx->alpha = 140;
        break;
        case 2:
        vfx->alpha = 255;
        break;
        default:
        vfx->alpha = 0;
        break;
    }
}
void activarEfectoFuego(tVFX* fuego)
{
    //Debo encender el fuego:
    activarEfecto(fuego,1);
}

void renderizarPikasRestantes(SDL_Renderer* renderer,tVFX* vfx, const int cantPikas,const int altoPantalla)
{
    SDL_Color colorTexto = {255, 255, 255, 255};
    char buffer[32]="";
    sprintf(buffer, "Pikas: %d", cantPikas);

    SDL_Surface* surf = TTF_RenderText_Solid(vfx->font, buffer, 0, colorTexto);

    if(!surf)
    {
        SDL_Log("Error creando surf para pikas restantes: %s", SDL_GetError());
    }
    SDL_Texture* textTexto = SDL_CreateTextureFromSurface(renderer, surf);
    if(!textTexto)
    {

        SDL_Log("Error creando text para pikas restantes: %s", SDL_GetError());
    }

    int margenIzquierdo = 20;
    int posY = (altoPantalla / 2) - (surf->h / 2); // Centro vertical exacto

    SDL_FRect rectTexto = { (float)margenIzquierdo, (float)posY, (float)surf->w, (float)surf->h };

    SDL_RenderTexture(renderer, textTexto, NULL, &rectTexto);

    //ahora la parte del fuego:
    if(seActivoEfectoAlpha(vfx))
    {
        //een este momento actualizo la textura con el valor del alpha
        SDL_SetTextureAlphaMod(vfx->text,(Uint8)vfx->alpha);
        //ahora coloco la textura sobre el numero
        SDL_FRect rectFuego = {
            rectTexto.x + 80,
            rectTexto.y - 10,
            rectTexto.w - 70,
            rectTexto.h + 10
        };
        SDL_RenderTexture(renderer,vfx->text, NULL, &rectFuego);
        //ahora lo más importante el decremento para que se vaya desvaneciendo:
        vfx->alpha -= 5;
        //pequeña comprobación de que nunca sobrepase 0
        if(vfx->alpha < 0 ) vfx->alpha = 0;
    }
    SDL_DestroySurface(surf);
    SDL_DestroyTexture(textTexto);
}
bool seActivoEfectoAlpha(tVFX* vfx)
{
    return (vfx->alpha > 0);
}

bool mostrarAgradecimientoCaballeros(SDL_Renderer* renderer, TTF_Font* font, const char* mensaje)
{
    mostrarMensajeEnVentanaYBorrarDespuesDeTecla(renderer, font, MENSAJE_AGRADECIMIENTO_CABALLEROS);
    mostrarMensajeEnVentanaYBorrarDespuesDeTecla(renderer, font, (char*)mensaje);
    return true;
}
void chequearYMostrarFinalDelJuego(Admin* manager, SDL_Renderer* renderer)
{
    char lineaFinal[200]="";

    TTF_Font* font = TTF_OpenFont(RUTA_FUENTE_FINAL_JUEGO, TAMANIO_FUENTE_FINAL_JUEGO);
    if(!font)
    {
        printf("[DEBUG]: Error cargando fuente para final del juego: %s\n", SDL_GetError());
    }
    tSonido bgmFinal;
    if(!crearBackgroundMusic(RUTA_BACKGROUND_MUSIC_FINAL,&bgmFinal))
    {
        printf("[DEBUG]: Error creando bgm en final del juego: %s\n", SDL_GetError());
    }
    reproducirBGM(&bgmFinal);

    sprintf(lineaFinal,"Felicidades %s, has completado el juego...", manager->jugador.nombre);
        //printf("FINAL COMPLETADO:\n"); /// COMPLETAR CON MSJ PERSONALIZADO
    mostrarMensajeEnVentanaYBorrarDespuesDeTecla(renderer, font, lineaFinal);
    mostrarMensajeEnVentanaYBorrarDespuesDeTecla(renderer, font, "FINAL COMPLETADO:");
        if(manager->jugador.TotalestadoUno==TAM_PARTIDAS)
        {
            if(manager->jugador.dificultadSeleccionada == DIFICIL)
            {
                detenerMusicaBGM();
                    liberarBGM(&bgmFinal);
                if(!crearBackgroundMusic(RUTA_BACKGROUND_MUSIC_FINAL_VERDADERO,&bgmFinal))
                {
                    printf("[DEBUG]: Error creando bgm en final de caballeros: %s\n", SDL_GetError());
                }
                reproducirBGM(&bgmFinal);
                //En este caso completo un final de todos caballeros, en dificil
                //printf("%s",FINAL_BUENO_MAXIMA_DIFICULTAD);
                char finalKnightsDificil[13][300]=
                {
                                    "El reino estalló en vítores.",
                                    "Los Demonios, cuya influencia había sido totalmente borrada de la faz del mundo conocido,",
                                      "se retiraron a las profundidades del Averno",
                                      "Bakelor, orgulloso y alegre te dice:",
                                      "¡Valiente becario! ",
                                      "No solo lograste la victoria donde yo temía la derrota,",
                                      "sino que lo hiciste en las condiciones más duras que jamás se han impuesto a un comandante.",
                                      "El Rey Bakelor tomó su espada ceremonial y, colocándola sobre tu hombro, continuó:",
                                      "A partir de este día, ya no eres un becario. Eres mi mano derecha,",
                                      "el Comandante Dorado de la Luz, el arquitecto de la paz en Garnick.",
                                      "Tu nombre LAUTY BARRETO será recordado no solo por la victoria,",
                                      "sino por la perfección con la que la lograste.",
                                      "Que el sol brille eternamente sobre nuestro reino gracias a tu heroísmo."
                };
                for(int j=0; j<13; j++)
                {
                    mostrarMensajeEnVentanaYBorrarDespuesDeTecla(renderer, font, finalKnightsDificil[j]);
                    if(terminoLaMusica())
                    {
                        recargarLaBGM(&bgmFinal);
                    }
                }
            }
            else
            {
                detenerMusicaBGM();
                //En este caso completo un final de todos caballeros, en facil o medio
                 liberarBGM(&bgmFinal);
                if(!crearBackgroundMusic(RUTA_BACKGROUND_MUSIC_FINAL_KNIGHTS,&bgmFinal))
                {
                    printf("[DEBUG]: Error creando bgm en final de caballeros: %s\n", SDL_GetError());
                }
                bajarElVolumenDeLaMusica(0.7);
                reproducirBGM(&bgmFinal);

                //Entonces completo el final de Knigths
                char finalKnights[4][300]=
                {
                    "El reino estalló en vítores. ",
                    "Los Demonios, cuya influencia había sido totalmente borrada de la faz del mundo conocido,",
                    " se retiraron a las profundidades del Averno",
                    "Bakelor está orgulloso"
                };
                for(int i=0; i<4; i++)
                {
                    mostrarMensajeEnVentanaYBorrarDespuesDeTecla(renderer, font, finalKnights[i]);
                    if(terminoLaMusica())
                    {
                        recargarLaBGM(&bgmFinal);
                    }
                }
                //printf("%s", FINAL_BUENO);
                restaurarVolumenMusica();
            }
        } else if(manager->jugador.TotalestadoDos==TAM_PARTIDAS)
        {
            detenerMusicaBGM();
            liberarBGM(&bgmFinal);
            //En este caso completo un final de todos demonios, en cualquier dificultad
           // printf("%s", FINAL_MALO);
            if(!crearBackgroundMusic(RUTA_BACKGROUND_MUSIC_FINAL_DEMONS,&bgmFinal))
            {
                printf("[DEBUG]: Error creando bgm en final de demonios: %s\n", SDL_GetError());
            }
            reproducirBGM(&bgmFinal);
            bajarElVolumenDeLaMusica(0.7);
            char finalDemonios[7][200]=
            {
                "Desde su cama de convalecencia, el Rey Bakelor observó por la ventana",
                "cómo el cielo de Garnick, una vez azul, se teñía de un perpetuo tono carmesí.",
                "Las hordas demoníacas, al percibir la victoria táctica, ",
                "irrumpieron en el castillo con una confianza renovada y brutal.",
                "Has logrado completar el desafío; pero con un costo impensable.",
                "Bakelor está horrorizado... Garnick está condenado",
                "¡Qué has hecho becario!"
            };
            for(int k=0; k<7; k++)
            {
                mostrarMensajeEnVentanaYBorrarDespuesDeTecla(renderer, font, finalDemonios[k]);
                if(terminoLaMusica())
                {
                    recargarLaBGM(&bgmFinal);
                }
            }
            restaurarVolumenMusica();
        } else
        {
            char finalNeutral[4][200]=
            {
                "Becario, no lo has hecho mal, pero... ",
                "¡DEBEMOS GANAR LA GUERRA, NO ES SUFICIENTE CON TENER ALGUNOS FRENTES!",
                "Apresurate y no me hagas enojar.",
                "-Bakelor, el señor de la guerra (en una situación doméstica algo delicada)"
            };
            for(int m=0; m<4; m++)
            {
                mostrarMensajeEnVentanaYBorrarDespuesDeTecla(renderer, font, finalNeutral[m]);
                if(terminoLaMusica())
                {
                    recargarLaBGM(&bgmFinal);
                }
            }
            //Completo otro final comun, es decir el tablero no es solo de Knights -> (neutral)
            //printf("%s", FINAL_NEUTRAL);
        }
        detenerMusicaBGM();
        liberarBGM(&bgmFinal);
}
