#include "TDAJuego.h"
#include "../Partida/TDAPartida.h"
int iniciarJuegoNuevo(Admin* manager, int* d, char*  nomNuevo)
{
    inicializarPartidas(manager->niveles, d);
    inicializarJugador(&(manager->jugador), nomNuevo,d);
    return 0;
}
int jugar(Admin* manager)
{
    ciclarPartida(manager);
    //ARREGLA ESTA FUNCIÓN
    return 0;
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

int cargarPartida(Admin* manager, char* nomGuardado)
{
    char guardado[200];
    guardado[0]='\0';
    char temporal[9];
    strcpy(temporal, nomGuardado);
    strcat(temporal, ".dat");
    strcat(guardado,RUTA_PARTIDAS_GUARDADAS_PARA_CARGAR);
    strcat(guardado,temporal);
    FILE* arch = fopen(guardado, "rb");
    if(!arch)
    {
        return ARCHIVO_CORRUPTO;
    }
    fread(manager, sizeof(*manager), 1, arch);
    puts("Se ha cargado exitosamente!");
    fclose(arch);
    return TODO_OK;
}
