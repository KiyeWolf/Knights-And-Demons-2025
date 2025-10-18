#include "TDATablero.h"

/*Verifica si las coordenadas del cursor (es decir, la posicion en la que se encuentra
el jugador) son validas. Estas coordenadas seran validas siempre y cuando se encuentren
dentro de la matriz. Si al menos una de ellas es un numero negativo o supera el orden
de la matriz, deja de ser valida.*/
int verificarCoordenadasCursor(const int orden, tJugada *jug){
    return (jug->posCursorX >= 0 && jug->posCursorX < orden) && (jug->posCursorY >= 0 && jug->posCursorY < orden) ? 1 : 0;
}

/*Muestra el tablero. Si los indices coinciden con las coordenadas del cursor, mostraran
[x] en esa casilla. La matriz no se modifica; "debajo" de donde esta el cursor ([x])
seguira habiendo una K o una D.*/
void mostrarTablero(const int orden, char **mat, tJugada *jug){
    int i, j;

    for(i=0; i<orden; i++){
        for(j=0; j<orden; j++){
            if(i==jug->posCursorY&&j==jug->posCursorX)
                printf(ANSI_COLOR_CYAN "\t|" ANSI_COLOR_RESET "[x]" ANSI_COLOR_CYAN "|" ANSI_COLOR_RESET);
            else if(*(*(mat+i)+j)=='K')
                printf(ANSI_COLOR_CYAN "\t| " ANSI_COLOR_YELLOW "%c" ANSI_COLOR_CYAN " |" ANSI_COLOR_RESET, *(*(mat+i)+j));
            else
                printf(ANSI_COLOR_CYAN "\t| " ANSI_COLOR_RED "%c" ANSI_COLOR_CYAN " |" ANSI_COLOR_RESET, *(*(mat+i)+j));
        }
        printf("\n");
    }
}

/*Libera la memoria del tablero, ya sea porque hubo un error al generarlo o porque
no se utilizara mas.*/
void liberarMemoriaTablero(char **mat, const int filas){
    int i;

    for(i=0;i<filas;i++){
        free(*(mat+i));
    }
    free(mat);
}

/*Crea el tablero utilizando memoria dinamica. Si hubo problemas con el primer malloc
retorna NULL, lo mismo si falla al usarlo para cada fila (no sin antes liberar lo que
se haya reservado).*/
char **crearTablero(const int orden){
    int i;
    char **mat = (char **) malloc(orden*sizeof(char *));

    if(mat==NULL)
        return NULL;

    for(i=0;i<orden;i++){
        *(mat+i) = (char *) malloc(sizeof(char)*orden);

        if(*(mat+i)==NULL){
            liberarMemoriaTablero(mat, i);
            return NULL;
        }
    }

    return mat;
}

/*Inicializa el tablero en forma aleatoria, colocando en cada casilla una K o una D.
Ademas, se inicializan las coordenadas del cursor para que esten en "(0,0)" al
inicio de la partida.*/
void inicializarTablero(const int orden, char **mat, tJugada *jug){
    int i, j;
    srand(time(NULL));
    for(i=0; i<orden; i++){
        for(j=0; j<orden; j++){
            *(*(mat+i)+j) = (rand()%2+1)==1 ? 'K' : 'D';
        }
    }

    jug->posCursorX=0;
    jug->posCursorY=0;
}

/*Verifica si todas las casillas del tablero son iguales. Si no lo son retorna INCORRECTO (0),
si ganaron los caballeros retorna GANO_KNIGHTS (1), y si ganaron los demonios retorna GANO_DEMONS (2)*/
int verificarVictoria(const int orden, char **m){
    int i, j;

    for(i=0; i<orden; i++){
        for(j=1; j<orden; j++){
            if(*(*(m+i)+j)!=*(*(m+i)+j-1))
                return INCORRECTO;
        }
    }

    return **m == 'K' ? GANO_KNIGHTS : GANO_DEMONS;
}

/*Cambia una sola casilla. Si tenia una D pasa a tener una K y viceversa.*/
char intercambiarEstadoCasilla(char *casilla){
    return *casilla == 'D' ? 'K' : 'D';
}

/*Ejecuta una jugada, intercambiando una o mas casillas donde y cuando corresponda.
Independientemente de si se use o no la pika, la casilla en la que se encuentra el
cursor cambia. Con la pika solo cambia esa casilla, si no se usa tambien cambian
las adyacentes*/
int ejecutarJugada(const int orden, char **mat, tJugada *jug){
    int fil=jug->posCursorY, col=jug->posCursorX;

    *(*(mat+fil)+col) = intercambiarEstadoCasilla(&*(*(mat+fil)+col));

    if(!jug->seUsoPika){
        //estos vectores serviran para acceder a las casillas adyacentes
        int vFilas[ADY]={-1, 1, 0, 0};
        int vColumnas[ADY]={0, 0, -1, 1};

        for(int i=0; i<ADY; i++){
            int nueva_fil=fil+*(vFilas+i);
            int nueva_col=col+*(vColumnas+i);

            if((nueva_fil>=0&&nueva_fil<orden)&&(nueva_col>=0&&nueva_col<orden))
                *(*(mat+nueva_fil)+nueva_col) = intercambiarEstadoCasilla(&*(*(mat+nueva_fil)+nueva_col));
        }
    }

    return verificarVictoria(orden, mat);
}
