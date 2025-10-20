#include "TDAPartida.h"

/// FUNCIONES DE AGUS QUE IMPLEMENTÉ YO
/*

bool ejecutarJugada(Admin* admin, char** tablero, size_t tamTablero) // RETORNA 1 SI GANO EN LA MISMA JUGADA, SINO 0
{
    int bandoGanador;
    char* celdaAfectada = *(tablero + admin->cursor.fila) + admin->cursor.columna;

    invertirCasilla(celdaAfectada);

    if(admin->cursor.usoPika == 0) // Si NO uso Pika, se invierte en cruz
    {
        if(admin->cursor.fila > 0)
           invertirCasilla(*(tablero + (admin->cursor.fila-1)) + admin->cursor.columna); // La de arriba

        if(admin->cursor.fila < tamTablero-1)
            invertirCasilla(*(tablero + (admin->cursor.fila+1)) + admin->cursor.columna); // La de abajo

        if(admin->cursor.columna > 0)
            invertirCasilla(*(tablero + admin->cursor.fila) + (admin->cursor.columna-1)); // La de la izquierda

        if(admin->cursor.columna < tamTablero-1)
            invertirCasilla(*(tablero + admin->cursor.fila) + (admin->cursor.columna+1)); // La de la derecha
    }

    bandoGanador = obtenerBandoGanador(tablero, tamTablero);

    if(bandoGanador != 0) // Puede ser 0, 1 o 2
    {
        if(bandoGanador == 1) // Si ademas ganaron los caballeros se premia al jugador con Pikas
            admin->jugador.pikasRestantes+=PREMIO_PIKAS;

        return 1;
    }

    return 0;
}

void invertirCasilla(char* casilla) // RECIBE CONTENIDO DE UNA CASILLA Y LO INVIERTE IN SITU (AL OTRO BANDO)
{
    // Suponiendo que caballeros = 'K' (por "Knights") y demonios = 'D' (por "Demons")
    if(*casilla == 'K')
        *casilla = 'D';
    else
        *casilla = 'K';
}

*/
///

/// NO ES NECESARIA CON LAS DE AGUS
/*
int obtenerBandoGanador(char** tablero, size_t tamTablero) // RETORNA 1 SI GANARON LOS BUENOS, 2 SI LOS MALOS Y 0 SI NINGUNO
{
    int cantCaballeros = 0, cantDemonios = 0;

    // Recorre el tablero para contar las piezas de cada bando
    for(int i=0; i<tamTablero; i++)
    {
        for(int j=0; j<tamTablero; j++)
        {
            if(*(*(tablero + i) + j) == 'K')
                cantCaballeros++;
            else if(*(*(tablero + i) + j) == 'D')
                cantDemonios++;
        }
    }

    if(cantCaballeros == tamTablero*tamTablero)
        return 1;
    else if(cantDemonios == tamTablero*tamTablero)
        return 2;

    return 0;
}

*/
///

void pedirJugada(Admin* admin, char** tablero, size_t tamTablero) // CARGA LA POSICION SELECCIONADA EN CURSOR
{
    char tecla;
    // char temp;
    //char* celda;
    int finSeleccion = 0;

    admin->cursor.seUsoPika = 0;
    //admin->cursor.posCursorX = 0;
    //admin->cursor.posCursorY = 0;

    printf("\nUsa las teclas W/A/S/D para moverte y Enter para confirmar\n");
    printf("Si usas una Pika, confirma con P\n");
    mostrarNivelPikasActual(admin->jugador.nivelActual, admin->jugador.pikasRestantes);

    while(!finSeleccion)
    {
        if (kbhit())
        {
            tecla = getch();
            tecla = tolower(tecla);
            switch (tecla)
            {
                case 'w': // Arriba
                    if(admin->cursor.posCursorY > 0)
                       admin->cursor.posCursorY--;

                    break;

                case 'a': // izquierda
                    if(admin->cursor.posCursorX > 0)
                        admin->cursor.posCursorX--;

                    break;

                case 's': // abajo
                    if(admin->cursor.posCursorY < tamTablero-1)
                        admin->cursor.posCursorY++;

                    break;

                case 'd': // Derecha
                    if(admin->cursor.posCursorX < tamTablero-1)
                        admin->cursor.posCursorX++;

                    break;

                case '\r': // Enter
                    finSeleccion = 1;
                    break;
                case 'p': // "Enter con Pika"
                    if(admin->jugador.pikasRestantes > 0)
                    {
                        admin->cursor.seUsoPika = 1;
                        admin->jugador.pikasRestantes--;
                        finSeleccion = 1;
                    }else
                    {
                        printf("No tienes mas pikas");
                        Sleep(3000);
                    }

                    break;
            }

            system("cls"); // Limpiar consola antes de mostrar

            // Mostrar movimiento en el tablero y devolver el valor que habia originalmente
            //celda = *(tablero + admin->cursor.posCursorY) + admin->cursor.posCursorX;
            //temp = *celda;
            //*celda = 'x'; // "equis" (x) = cursor

            mostrarTablero(tamTablero, tablero, &admin->cursor);

            //*celda = temp;
        }

        Sleep(50); // Pausa para no saturar
    }
    system("cls"); // Limpiar consola despues de mostrar (evita duplicados)
}

int guardarPartida(Admin* manager) // GUARDA EL ESTADO ACTUAL DEL JUEGO (HASTA ULTIMO NIVEL JUGADO)
{
    char guardado[256];
    snprintf(guardado, sizeof(guardado), "%s%s.dat", RUTA_PARTIDAS_GUARDADAS, manager->jugador.nombre);
    FILE* arch = fopen(guardado, "wb");
    if(!arch)
    {
        return ARCHIVO_CORRUPTO; //No se pudo encontrar el archivo, por tanto se retorna codigo de error
    }
    fwrite(manager,sizeof(*manager),1, arch);
    system("cls");
    puts("El guardado ha sido exitoso.");
    fclose(arch);
    Sleep(2000);
    system("cls");
    return TODO_OK;
}

size_t obtenerTamTablero(const Admin* admin) // DEVUELVE TAM TABLERO (ORDEN)
{
    switch(admin->jugador.dificultadSeleccionada)
    {
        case (1): return TAM_TABLERO_1;
        case (2): return TAM_TABLERO_2;
        case (3): return TAM_TABLERO_3;
    }
    //Para sacar el warning
    return 1;
}

int tiempo(const Admin* admin, int operacion) // DEVUELVE TIEMPO (EN SEGUNDOS) O LO REINICIA SEGUN LA OPERACION
{
    static time_t inicioNivel = 1; // Es static, se inicializa una vez, no va a ocurrir que cada vez que se llame la var sea 1

    switch(operacion)
    {
        case T_INICIAL:
            switch(admin->jugador.dificultadSeleccionada)
            {
                case (1): return TIEMPO_INICIALES_FACIL;
                case (2): return TIEMPO_INICIALES_MEDIO;
                case (3): return TIEMPO_INICIALES_DIFICIL;
            }

        case T_TRANSCURRIDO:
            if(inicioNivel == 1)
                inicioNivel = time(NULL);

            // Tiempo transcurrido de juego = diferencia entre tiempo actual y tiempo en el que arranco (en segundos)
            return (int)difftime(time(NULL), inicioNivel);

        case T_REINICIO:
            inicioNivel = 1; // Para que la logica funcione al cambiar de nivel
            return 0;
    }
    //para sacar el warning nomás la linea de abajo.
    return 0;
}

int iniciarPartida(Admin* admin) // DEVUELVE SI GANÓ O NO (1,2 o 0 si perdio el útlimo)
{
    int gano = 0;
    int tiempoTranscurrido = 0;
    int tiempoInicial = tiempo(admin, T_INICIAL);

    size_t tamTablero = obtenerTamTablero(admin);
    char** tablero = NULL;

    tablero = crearTablero(tamTablero);
    inicializarTablero(tamTablero, tablero, &admin->cursor);

    // Mientras no se acabe el tiempo ni haya ganado, sigue la partida
    while( ( (tiempoInicial - tiempoTranscurrido) > 0 ) && gano == 0)
    {
        mostrarBarraEstado(admin, gano);

        mostrarTablero(tamTablero, tablero, &admin->cursor);

        printf("\nTiempo restante: %d\n", (tiempoInicial - tiempoTranscurrido));

        pedirJugada(admin, tablero, tamTablero);

        gano = ejecutarJugada(tamTablero, tablero, &admin->cursor);

        tiempoTranscurrido = tiempo(admin, T_TRANSCURRIDO); // Re-calculo el tiempo transcurrido
    }

    liberarMemoriaTablero(tablero, tamTablero);

    admin->niveles[admin->jugador.nivelActual].tiempo = tiempoTranscurrido; // Establezco el tiempo del jugador en el nivel

    tiempo(admin, T_REINICIO); // Reinicio el tiempo transcurrido

    return gano;
}

int ciclarPartida(Admin* admin) // CICLA TODA LA PARTIDA (1 NIVEL A LA VEZ) HASTA QUE SALGA AL MENÚ
{
    int finalJuego = 0;
    int gano;
    int estaEnMenu = 0;
    int respuesta;

    while(!estaEnMenu && finalJuego == 0)
    {
        textoDeInicioDeNivel();
        gano = iniciarPartida(admin);
        if(gano != 0) //Por lo menos no perdió, entra dentro de este if
        {
            if(gano == GANO_KNIGHTS){
                admin->jugador.TotalestadoUno++;
                admin->jugador.pikasRestantes+=PREMIO_PIKAS;
                puts("¡Los caballeros están agradecidos!");
                printf("Has obtenido %d pikas extras", PREMIO_PIKAS);
            }

            // gano == GANO_DEMONS
            else{
                admin->jugador.TotalestadoDos++;
                puts("¡Los demonios están avanzando!");
                puts("");
            }
            admin->jugador.nivelesCompletados+=1;
            admin->niveles[admin->jugador.nivelActual].estadoCompletado = 1; // Actualizo el estado del nivel a ganó o no ganó (1 o 0)
            admin->jugador.nivelActual++;

            printf("\n¿Desea guardar la partida?: ");
            printf("\n1. Sí\n2. No\n");
            scanf("%d", &respuesta);

            if(respuesta == 1)
            {
                if(guardarPartida(admin)!=0)
                {
                    puts("\nNo se ha podido guardar la partida correctamente.\n");
                }
            }
            puts(" ");

        } else
        {
            //Significa que perdió, por eso se muestra este mensaje de derrota
            printf("%s", MENSAJE_DERROTA);
        }

        if(admin->jugador.nivelActual < TAM_PARTIDAS) //Se evalúa si quedan niveles por jugar o no.
        {
            //estaEnMenu = postNivel(admin, gano);
            if(postNivel(admin, gano) == 1)
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

int postNivel(Admin* admin, int resultado) // DEVUELVE SI SE VA O NO AL MENÚ (0 o 1)
{
    int respuesta;

    if(resultado != 0) // Si el resultado es 1 ó 2 es porque gano
    {
        printf("\n¿Desea volver al menu?");
        printf("\n1. Continuar\n2. Volver al menu\n");
    } else
    {
        printf("\n¿Desea volver al menu?");
        printf("\n1. Volver a intentar\n2. Volver al menu\n");
    }
    scanf("%d", &respuesta);

    system("cls");


   // admin->jugador.tiempoDeJuego+=(admin->niveles[admin->jugador.nivelActual].tiempo); // Actualizo el tiempo total del jugador

    return (respuesta-1); // El -1 es para que devuelva 1 o 0 en vez de 1 o 2
}

void mostrarNivelPikasActual(int nivelActual, int pikasActuales) // MUESTRA PIKAS Y NIVEL ACTUALES + LIMPIA LA CONSOLA
{
    printf("\n°|Nivel actual: \t%d", (nivelActual + 1)); // Este nivelActual + 1, es porque el primer nivel en el codigo es el nivel 0, entonces acá se imprime como nivel 1
    printf("\n*|Pikas actuales: \t%d", pikasActuales);
    // El "°|" y "*|" son un agregado estetico
}

void mostrarBarraEstado(Admin* admin, int resultado)
{
    const int largoBarra = 50;
    int mitad = largoBarra / 2;
    int baseCaballeros = mitad;

    int diferencia = admin->jugador.TotalestadoUno - admin->jugador.TotalestadoDos;

    // Cada victoria representa este cambio en cantidad de caracteres
    float cambioPorVictoria = mitad / TAM_PARTIDAS;

    // Ajustamos la longitud de cada lado según la diferencia
    int parteCaballeros = (int)(baseCaballeros + diferencia * cambioPorVictoria);
    int parteDemonios = largoBarra - parteCaballeros;

    // Limitar por si acaso (que no se pase de los extremos)
    if (parteCaballeros < 0) parteCaballeros = 0;
    if (parteCaballeros > largoBarra) parteCaballeros = largoBarra;
    if (parteDemonios < 0) parteDemonios = 0;
    if (parteDemonios > largoBarra) parteDemonios = largoBarra;

    printf("\nCaballeros [");

    // Caballeros: más '#' si van ganando
    for (int i = 0; i < parteCaballeros; i++)
        putchar('#');

    putchar('|');

    // Demonios: más '-' si van ganando
    for (int i = parteCaballeros; i < largoBarra; i++)
        putchar('-');

    printf("] Demonios\n");

    printf("%d frentes                                                       %d frentes\n\n",
           admin->jugador.TotalestadoUno,
           admin->jugador.TotalestadoDos);
}

int jugar(Admin* manager) /// AGREGA GUILLE
{
    int finalJuego;

    barraDeCarga();
    finalJuego = ciclarPartida(manager);
    if(finalJuego == 2) //Este if pregunta si el jugador gano
    {
        system("cls");
        printf("Felicidades %s, has completado el juego...\n", manager->jugador.nombre);
        printf("FINAL COMPLETADO:\n"); /// COMPLETAR CON MSJ PERSONALIZADO
        Sleep(300);
        if(manager->jugador.TotalestadoUno==TAM_PARTIDAS)
        {
            //Entonces completo el final de Knigths
            if(manager->jugador.dificultadSeleccionada == DIFICIL)
            {
                //En este caso completo un final de todos caballeros, en dificil
                printf("%s",FINAL_BUENO_MAXIMA_DIFICULTAD);
            }
            else
            {
                //En este caso completo un final de todos caballeros, en facil o medio
                printf("%s", FINAL_BUENO);
            }
        } else if(manager->jugador.TotalestadoDos==TAM_PARTIDAS)
        {
            //En este caso completo un final de todos demonios, en cualquier dificultad
            printf("%s", FINAL_MALO);
        } else
        {
            //Completo otro final comun, es decir el tablero no es solo de Knights -> (neutral)
            printf("%s", FINAL_NEUTRAL);
        }
        Sleep(8000);
        return 0; //el jugador completo el juego
    }
    barraDeCarga();

    return 1; //El jugador no completo el juego
}

void barraDeCarga()
{
    system("cls");
    printf("Cargando");
    Sleep(400);
    printf(".");
    Sleep(400);
    printf(" .");
    Sleep(400);
    printf(" .");
    Sleep(400);
    printf(" .");
    system("cls");
}

void textoDeInicioDeNivel()
{
    system("cls");
    printf("<< Una nueva Batalla Comienza... >>");
    Sleep(500);
    system("cls");
}
