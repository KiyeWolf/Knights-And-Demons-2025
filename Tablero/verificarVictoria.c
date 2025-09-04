#define CORRECTO 1
#define INCORRECTO 0

int verificarVictoria(int orden, char **m){
    int i, j;

    for(i=0; i<orden; i++){
        for(j=1; j<orden; j++){
            if(*(*(m+i)+j)!=*(*(m+i)+j-1))
                return INCORRECTO;
        }
    }

    return CORRECTO;
}