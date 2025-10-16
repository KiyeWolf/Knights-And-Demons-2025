#include "TDAPartida.h"

/// BORRAR (SOLO ESTA PARA EVITAR EL ERROR AL COMPILAR)
int main()
{
    return 0;
}
///

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
    char tecla, temp;
    char* celda;
    int finSeleccion = 0;

    admin->cursor.posCursorX = 0;
    admin->cursor.posCursorY = 0;

    printf("\nUsa las teclas W/A/S/D para moverte y Enter para confirmar\n");
    printf("Si usas una Pika, confirma con P\n");

    while(!finSeleccion)
    {
        if (kbhit())
        {
            tecla = getch();

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
                    finSeleccion = 1;
                    admin->cursor.seUsoPika = 1;
                    admin->jugador.pikasRestantes--;
                    break;
            }

            system("cls"); // Limpiar consola antes de mostrar

            // Mostrar movimiento en el tablero y devolver el valor que habia originalmente
            celda = *(tablero + admin->cursor.posCursorY) + admin->cursor.posCursorX;
            temp = *celda;
            *celda = '@'; // "arroba" (@) = cursor

            mostrarTablero(tamTablero, tablero, &admin->cursor);

            *celda = temp;
        }

        sleep(50); // Pausa para no saturar
    }
}

int guardarPartida(Admin* manager) /// (Es la de Guille)
{
    char temporal[9];
    strcpy(temporal, manager->jugador.nombre);
    strcat(temporal, ".dat");
    FILE* arch = fopen(temporal, "wb");
    if(!arch)
    {
        return ARCHIVO_CORRUPTO;
    }
    fwrite(manager,sizeof(*manager),1, arch);
    puts("El guardado ha sido exitoso.");
    fclose(arch);
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
}

bool iniciarPartida(Admin* admin) // DEVUELVE SI GANÓ O NO (1 o 0)
{
    bool gano;
    int tiempoTranscurrido = 0;
    int tiempoInicial = tiempo(admin, T_INICIAL);

    size_t tamTablero = obtenerTamTablero(admin);
    char** tablero = NULL;

    tablero = crearTablero(tamTablero);
    inicializarTablero(tamTablero, tablero, &admin->cursor);

    // Mientras no se acabe el tiempo ni haya ganado, sigue la partida
    while( ( (tiempoInicial - tiempoTranscurrido) > 0 ) && !gano)
    {
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

void ciclarPartida(Admin* admin) // CICLA TODA LA PARTIDA (1 NIVEL A LA VEZ) HASTA QUE SALGA AL MENÚ
{
    int gano;
    int estaEnMenu = 0;

    while(!estaEnMenu)
    {
        gano = iniciarPartida(admin);
        if(gano)
        {
            admin->jugador.nivelActual++;
            guardarPartida(admin);
        } else
        {
            printf("\nLo siento, perdiste\n");
        }

        estaEnMenu = postNivel(admin, gano);
    }
}

int postNivel(Admin* admin, int resultado) // DEVUELVE SI SE VA O NO AL MENÚ (0 o 1)
{
    int respuesta;
    printf("\n¿Desea volver al menu?");
    if(resultado == 1) // Si el resultado es 1 es porque gano
    {
        printf("\n1. Continuar\n2. Volver al menu\n");
    } else
    {
        printf("\n1. Volver a intentar\n2. Volver al menu\n");
    }
    scanf("%d", &respuesta);

    admin->niveles[admin->jugador.nivelActual].estadoCompletado = resultado; // Actualizo el estado del nivel a ganó o no ganó (1 o 0)
    admin->jugador.tiempoDeJuego+=(admin->niveles[admin->jugador.nivelActual].tiempo); // Actualizo el tiempo total del jugador

    return (respuesta-1); // El -1 es para que devuelva 1 o 0 en vez de 1 o 2
}

void mostrarNivelPikasActual(int nivelActual, int pikasActuales) // MUESTRA PIKAS Y NIVEL ACTUALES + LIMPIA LA CONSOLA
{
    printf("\n<<Comienza una nueva batalla, preparate...>>\n");
    printf("\n°|Nivel actual: %d", nivelActual);
    printf("\n*|Pikas actuales: %d", pikasActuales);
    // El "°|" y "*|" son un agregado estetico

    system("cls"); // Limpio la consola
}
