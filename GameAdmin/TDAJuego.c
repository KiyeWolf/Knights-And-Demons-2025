#include <stdbool.h>
#include <string.h>
#define FACIL 1
#define MEDIO 2
#define DIFICIL 3
#define PIKAS_INICIALES_FACIL 30
#define PIKAS_INICIALES_MEDIO 15
#define PIKAS_INICIALES_DIFICIL 7
#define TIEMPO_INICIALES_FACIL 300
#define TIEMPO_INICIALES_MEDIO 200
#define TIEMPO_INICIALES_DIFICIL 150
#define PENALIZACION_DE_TIEMPO_POR_NIVEL 2
#define TAM_NOMBRE 4
#define TAM_PARTIDAS 20

#define ARCHIVO_CORRUPTO 1
#define TODO_OK 0

typedef struct Partidas
{
    int tiempo;
    int dificultadDePartida;
    bool estadoCompletado;
}Partidas;
typedef struct Usuario
{
    char nombre[TAM_NOMBRE];
    int dificultadSeleccionada;
    int TotalestadoUno;
    int TotalestadoDos;
    float tiempoDeJuego;
    int nivelActual;
    int pikasRestantes;
}Player;
typedef struct
{
    Partidas niveles[TAM_PARTIDAS];
    Player jugador;
    /* data */
}Admin;


void inicializarJugador(Player* jugador, char* nombre, const  int* dificultad);
int iniciarJuegoNuevo(Admin* manager, int* d, char*  nomNuevo);
int jugar(Admin* manager);
void inicializarPartidas(Partidas* partidas,const  int* dificultad);
void mostrarManager(Admin* manager);
void mostrarNiveles(Partidas* niveles);
void mostrarJugador(Player* jugador);
int guardarPartida(Admin* manager);
int cargarPartida(Admin* manager,  char* nomGuardado);

int iniciarJuegoNuevo(Admin* manager, int* d, char*  nomNuevo)
{

    inicializarPartidas(manager->niveles, d);
    inicializarJugador(&(manager->jugador), nomNuevo,d);
    guardarPartida(manager);
    return 0;
}
int jugar(Admin* manager)
{

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
