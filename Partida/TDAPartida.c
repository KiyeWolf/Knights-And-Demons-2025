#include "TDAPartida.h"

int tiempo(int var)
{
    // W.I.P
}
int pedirJugada()
{
    // W.I.P
}
int ejecutarJugada()
{
    // W.I.P
}
int guardarPartida()
{
    // W.I.P
}

// PENDING DE LA FUNCIÓN -> PONER PARAMETROS EN LAS FUNCIONES QUE RESTAN Y HACER FUNCIÓN tiempo(), pedirJugada() y ejecutarJugada()
bool iniciarPartida(Admin* admin) // DEVUELVE SI GANÓ O NO (1 o 0)
{
    bool gano = 0;
    int actual = 0; // CALCULAR TIEMPO ACTUAL
    int inicialNivel;

    size_t tamTablero = 3; // VER DÓNDE OBTENER TAM TABLERO
    char** tablero = NULL;

    // ESTABLEZCO EL TIEMPO INICIAL SEGUN DIFICULTAD
    if(admin->jugador.dificultadSeleccionada == FACIL)
    {
        inicialNivel = TIEMPO_INICIALES_FACIL;
    } else if(admin->jugador.dificultadSeleccionada == MEDIO)
    {
        inicialNivel = TIEMPO_INICIALES_MEDIO;
    } else if(admin->jugador.dificultadSeleccionada == DIFICIL)
    {
        inicialNivel = TIEMPO_INICIALES_DIFICIL;
    }

    tablero = crearTablero(tamTablero);
    inicializarTablero(tamTablero, tablero);

    // Mientras no se acabe el tiempo ni haya ganado, sigue la partida
    while( ( (tiempo(inicialNivel) - tiempo(actual)) > 0 ) && !gano) // **PENDING TODO LO RELACIONADO AL TIEMPO!**
    {
        mostrarTablero(tamTablero, tablero);

        // MOSTRAR TIEMPO RESTANTE -> tiempo(inicialNivel) - tiempo(actual)

        pedirJugada(); // VER FUNC Y PARAMS

        if(admin->jugador.pikasRestantes > 0) // Si tiene pikas entonces se ejecuta la jugada
        {
            gano = ejecutarJugada(); // VER FUNC Y PARAMS
        }

        tiempo(actual); // RE CALCULAR
        // tiempoTotal -= tiempoActual; // RE CALCULAR
    }

    liberarMemoriaTablero(tablero, tamTablero);

    return gano; // VER BIEN TEMA RETURN
}

// PENDING -> HACER LA FUNCIÓN guardarPartida()
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
            guardarPartida(admin); // HACER guardarPartida()
        } else
        {
            printf("\nLo siento\n");
        }

        estaEnMenu = postNivel(admin, gano);
    }
}

int postNivel(Admin* admin, int resultado) // DEVUELVE SI SE VA O NO AL MENÚ (0 o 1)
{
    int respuesta;
    printf("\n¿Desea volver al menu?");
    if(resultado == 1) // Si el resultado es 1 es porque ganó
    {
        printf("\n1. Continuar\n2. Volver al menu\n");
    } else
    {
        printf("\n1. Volver a intentar\n2. Volver al menu\n");
    }
    scanf("%d", &respuesta);

    admin->niveles[admin->jugador.nivelActual].estadoCompletado = resultado; // Actualizo el estado del nivel a ganó o no ganó (1 o 0)
    admin->niveles[admin->jugador.nivelActual].tiempo+=(admin->jugador.tiempoDeJuego); // Actualizo el tiempo del nivel

    return (respuesta-1); // El -1 es para que devuelva 1 o 0 en vez de 1 o 2
}

void mostrarNivelPikasActual(int nivelActual, int pikasActuales) // MUESTRA PIKAS Y NIVEL ACTUALES
{
    printf("\n<<Comienza una nueva batalla, preparate...>>\n");
    printf("\n°|Nivel actual: %d", nivelActual);
    printf("\n*|Pikas actuales: %d", pikasActuales);
    // El "°|" y "*|" son un agregado estético

    system("cls");
}
