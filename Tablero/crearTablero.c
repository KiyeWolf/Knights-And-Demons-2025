void liberarMemoriaTablero(char **mat, const int filas){
    int i;

    for(i=0;i<filas;i++){
        free(*(mat+i));
    }
    free(mat);
}

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