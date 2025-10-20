#include "TDAJuego.h"
//Esta función se encarga de Iniciar las estructuras que componen al administrador, exceptuando el cursor.
int iniciarJuegoNuevo(Admin* manager, int* d, char*  nomNuevo)
{
    inicializarPartidas(manager->niveles, d);
    inicializarJugador(&(manager->jugador), nomNuevo,d);
    return 0;
}
//Inicia cada nivel acorde a la cantidad de niveles deseados en el header, además por cada nivel se coloca que no se completó y la dificultad del mismo.
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
//Esta función inicia la estructura del jugador, colocando el nombre, la dificultad seleccionada, los niveles completados, la cantidad de tableros a favor de los caballeros y los demonios en 0
void inicializarJugador(Player* jugador, char* nombre, const  int* dificultad)
{
    strcpy(jugador->nombre,nombre);
    jugador->dificultadSeleccionada = *(dificultad);
    jugador->TotalestadoUno = 0;
    jugador->TotalestadoDos = 0;
    jugador->nivelesCompletados = 0;
    jugador->nivelActual = NIVEL_INICIAL;
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


/*Esta función busca un archivo en la carpeta Juego/Saved con el nombre indicado y concatenandolo a .dat
para luego cargar de ese archivo la estructura administrador, de esta manera recupera una estructura guardada anteriormente.*/
int cargarPartida(Admin* manager, char* nomGuardado)
{
    char guardado[TAM_LINEA_JUEGO];
    snprintf(guardado, sizeof(guardado), "%s%s.dat", RUTA_PARTIDAS_GUARDADAS_PARA_CARGAR, nomGuardado);
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

/*Se encarga de buscar en Juego/Saved un archivo txt y mostrar el texto contenido en el mismo.
La ruta del archivo está definido en el encabezado*/
int mostrarHistoriaInicial()
{

    FILE* archivo = fopen(RUTA_HISTORIA_INICIAL, "r");
    system("cls");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo de historia inicial.\n");
        return ARCHIVO_CORRUPTO;
    }

    char linea[TAM_LINEA_JUEGO];
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        printf("%s", linea);
    }
    puts("\n");
    system("pause");// Pausa para que el jugador pueda leer la historia
    fclose(archivo);
    system("cls");
    return TODO_OK;
}