#include <stdio.h>

void mostrarTablero(int orden, char **m){
    int i, j;

    for(i=0; i<orden; i++){
        for(j=0; j<orden; j++){
            printf("\t%c", *(*(m+i)+j));
        }
        printf("\n");
    }
}