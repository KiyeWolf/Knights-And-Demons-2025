#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void inicializarTablero(int orden, char **m){
    int i, j;
    srand(time(NULL));
    for(i=0; i<orden; i++){
        for(j=0; j<orden; j++){
            *(*(m+i)+j) = (rand()%2+1)==1 ? 'K' : 'D';
        }
    }
}