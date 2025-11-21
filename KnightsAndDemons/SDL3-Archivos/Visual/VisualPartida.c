#include "VisualPartida.h"

int jugar(Admin* manager,  SDL_Renderer* renderer, TTF_Font* font,tSonido* sonidoBotonCasilla)
{
    int finalJuego;
    //barraDeCarga();
    finalJuego = ciclarPartida(manager, renderer,sonidoBotonCasilla);
    if(finalJuego == 2) //Este if pregunta si el jugador gano todos los niveles
    {
        char mensajeFinal[100];
        sprintf(mensajeFinal, "Felicidades %s, has completado el juego...", manager->jugador.nombre);
        mostrarMensajeEnVentanaYBorrarDespuesDeTecla(renderer, font, mensajeFinal);
        mostrarMensajeEnVentanaYBorrarDespuesDeTecla(renderer, font, "FINAL COMPLETADO:");
        if(manager->jugador.TotalestadoUno==TAM_PARTIDAS)
        {
            //Entonces completo el final de Knigths
            if(manager->jugador.dificultadSeleccionada == DIFICIL)
            {
                //En este caso completo un final de todos caballeros, en dificil
                mostrarMensajeEnVentanaYBorrarDespuesDeTecla(renderer, font, FINAL_BUENO_MAXIMA_DIFICULTAD);
            }
            else
            {
                //En este caso completo un final de todos caballeros, en facil o medio
               mostrarMensajeEnVentanaYBorrarDespuesDeTecla(renderer, font, FINAL_BUENO);
            }
        } else if(manager->jugador.TotalestadoDos==TAM_PARTIDAS)
        {
            //En este caso completo un final de todos demonios, en cualquier dificultad
            mostrarMensajeEnVentanaYBorrarDespuesDeTecla(renderer, font, FINAL_MALO);
        } else
        {
            //Completo otro final comun, es decir el tablero no es solo de Knights -> (neutral)
            mostrarMensajeEnVentanaYBorrarDespuesDeTecla(renderer, font, FINAL_NEUTRAL);
        }
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
        if(gano != 0) //Por lo menos no perdió, entra dentro de este if
        {
            if(gano == GANO_KNIGHTS){
                admin->jugador.TotalestadoUno++;
                admin->jugador.pikasRestantes+=PREMIO_PIKAS;
                mostrarMensajeEnVentanaYBorrarDespuesDeTecla(renderer, fontMensajeInicioNivel, "¡Los caballeros están agradecidos!");
                char mensajePikas[50];
                sprintf(mensajePikas, "Has obtenido %d pikas extras", PREMIO_PIKAS);
                mostrarMensajeEnVentanaYBorrarDespuesDeTecla(renderer, fontMensajeInicioNivel, mensajePikas);
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

            /*printf("\n¿Desea guardar la partida?: ");
            printf("\n1. Sí\n2. No\n");
            scanf("%d", &respuesta);
            */
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

        } else
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

    //parteSonido



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

                    //reproduce sonido
                    reproducirSFX(sonidoBotonCasilla);


                    if(e.button.button == SDL_BUTTON_LEFT)
                    {
                        actualizarCursor(&(admin->cursor), posX, posY, false);
                        gano = ejecutarJugadaSDL(tamTablero, tablero, TAMANIO_CELDA, &(admin->cursor));
                    }
                    if(e.button.button == SDL_BUTTON_RIGHT)
                    {

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
            //renderizarBarraDeTiempoSDL()

            renderizarContadoresSDL(renderer, tiempoRestante, font);

        SDL_RenderPresent(renderer);

        //limitar fps, copiado y pegado de stackoverflow
        SDL_Delay(16);
    }

    liberarMemoriaTablero(tablero, tamTablero);

   if(gano)
   {
        actualizarPikas(&pikasDeInicio, &(admin->jugador.pikasRestantes));
        admin->niveles[admin->jugador.nivelActual].tiempo = tiempoRestante; // Establezco el tiempo del jugador en el nivel
   }


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
    printf("[DEBUG]: tiempo restante: %d \n", tiempo);
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


    //ARREGLAR FUNCION



    /*SDL_Color blanco = {255,255,255,255};

        printf("\n¿Desea volver al menu?");
        printf("\n1. Continuar\n2. Volver al menu\n");
        */
       mostrarMensajeEnVentanaYBorrarDespuesDeTecla(renderer, font, "¿Desea volver al menu? 1. Continuar  2. Volver al menu");
    respuesta = 1; //por defecto sigue jugando

    return (respuesta-1); // El -1 es para que devuelva 1 o 0 en vez de 1 o 2
}
