#include "TDAJuego.h"
int iniciarJuegoNuevo(Admin* manager, int* d, char*  nomNuevo)
{

    inicializarPartidas(manager->niveles, d);
    inicializarJugador(&(manager->jugador), nomNuevo,d);
    /*EL GUARDADO DE PARTIDA DE MÁS ABAJO ESTA SIMULADO, SE DEBE BORRAR LO MAS PRONTO POSIBLE*/
    //guardarPartida(manager);
    return 0;
}
int jugar(Admin* manager)
{
    //TODO ESTO ESTÁ SIMULADO NOMÁS
    char** tablero = crearTablero(ORDEN_TEMPORAL);
    inicializarTablero(ORDEN_TEMPORAL,tablero);
    mostrarTablero(ORDEN_TEMPORAL, tablero);

    //parte de la partida en sí simulo con un get char
    fflush(stdin);
    getchar();
    liberarMemoriaTablero(tablero,ORDEN_TEMPORAL);
}
void inicializarPartidas(Partidas* partidas,const  int* dificultad)
{
    for(int i = 0 ; i < TAM_PARTIDAS ;i++ )
    {
        switch(*dificultad)
        {
            case FACIL:
            (partidas+i)->tiempo = TIEMPO_INICIALES_FACIL - (i*PENALIZACION_DE_TIEMPO_POR_NIVEL);
            (partidas+i)->dificultadDePartida = *(dificultad);
            (partidas+i)->estadoCompletado=false;

            break;
            case MEDIO:
            (partidas+i)->tiempo = TIEMPO_INICIALES_MEDIO - (i*PENALIZACION_DE_TIEMPO_POR_NIVEL);
            (partidas+i)->dificultadDePartida = *(dificultad);
            (partidas+i)->estadoCompletado=false;
            break;
            case DIFICIL:
            (partidas+i)->tiempo = TIEMPO_INICIALES_DIFICIL - (i*PENALIZACION_DE_TIEMPO_POR_NIVEL);
           (partidas+i)->dificultadDePartida = *(dificultad);
            (partidas+i)->estadoCompletado=false;
            break;
        }

    }
}
void inicializarJugador(Player* jugador, char* nombre, const  int* dificultad)
{
    strcpy(jugador->nombre,nombre);
    jugador->dificultadSeleccionada = *(dificultad);
    jugador->TotalestadoUno = 0;
    jugador->TotalestadoDos = 0;
    jugador->tiempoDeJuego = 0;
    jugador->nivelActual = 1;
    switch (*dificultad)
    {
    case FACIL:
        jugador->pikasRestantes = PIKAS_INICIALES_FACIL;
        break;
    case MEDIO:
        jugador->pikasRestantes = PIKAS_INICIALES_MEDIO;
        break;
    case DIFICIL:
        jugador->pikasRestantes = PIKAS_INICIALES_DIFICIL;
        break;
    }
}

void mostrarManager(Admin* manager)
{
    mostrarNiveles(manager->niveles);
    mostrarJugador(&(manager->jugador));
}
void mostrarNiveles(Partidas* niveles)
{
    for(int i = 0 ; i < TAM_PARTIDAS ; i++)
    {
        printf("%d\n", (*(niveles+i)).tiempo);
        printf("%d\n", (niveles+i)->dificultadDePartida);
        printf("%d\n", (*(niveles+i)).estadoCompletado);
    }
}
void mostrarJugador(Player* jugador)
{
    printf("%s\n", jugador->nombre);
    printf("%d\n", jugador->dificultadSeleccionada);
    printf("%d\n", jugador->TotalestadoUno);
    printf("%d\n", jugador->TotalestadoDos);
    printf("%f\n", jugador->tiempoDeJuego);
    printf("%d\n", jugador->nivelActual);
    printf("%d\n", jugador->pikasRestantes);

}
int guardarPartida(Admin* manager)
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

int cargarPartida(Admin* manager, char* nomGuardado)
{
    char temporal[9];
    strcpy(temporal, nomGuardado);
    strcat(temporal, ".dat");
    puts(temporal);
    FILE* arch = fopen(temporal, "rb");
    if(!arch)
    {
        return ARCHIVO_CORRUPTO;
    }
    fread(manager, sizeof(*manager), 1, arch);
    puts("Se ha cargado exitosamente!");
    fclose(arch);
    return TODO_OK;
}
