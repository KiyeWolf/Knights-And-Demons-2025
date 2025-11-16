#include "VisualTablero.h"
void renderizarTableroConSDL(SDL_Renderer *renderer,
                          char **tablero,
                          size_t tam,
                          SDL_Texture *texKnight,
                          SDL_Texture *texDemon,
                          const int tamCelda,
                          bool mostrarGrilla,
                          int luzHoverFila,
                          int luzHoverColumna)
{
    if(!renderer)
    {
        //cancelo todo
        return;
    }

    //en esta parte voy a usar estos valores para las celdas
    int tableroW = (int)tam * tamCelda;
    int tableroH = (int)tam * tamCelda;
    float offsetX = ((float)WIDTH - tableroW) / 2.0f;
    float offsetY = ((float)HEIGHT - tableroH) / 2.0f;

    //8 porciento de padding para no molestar la imagen
    const float pad = (float)(tamCelda) * 0.08f;
    const float imgW = tamCelda - 2.0f * pad;
    const float imgH = tamCelda - 2.0f * pad;

    // Colores
    // este esd fondo de cada celda
    SDL_Color celdaBg = {30, 30, 30, 255};

    // color líneas de la grilla esta acá
    SDL_Color gridColor = {60, 60, 60, 255};

    // overlay semitransparente para hover (copidado y pegaod) (alpha bajo)
    SDL_Color highlightColor = {255, 255, 255, 48};

    //ahora recorro la grilla tranquilo, dos for
    for (size_t i = 0; i < tam; ++i)
    {
        for (size_t j = 0; j < tam; ++j)
        {
            float x = offsetX + (float)j * tamCelda;
            float y = offsetY + (float)i * tamCelda;

            // este es el rectangulo que tiene celda, sirve para el fondi
            //es cuadrado
            SDL_FRect celdaRect = { x, y, (float)tamCelda, (float)tamCelda };

            // primero hagamos el fondo de la celda

            //draw color conel elegido de celda
            SDL_SetRenderDrawColor(renderer, celdaBg.r, celdaBg.g, celdaBg.b, celdaBg.a);
            //fill rect para pintarlo con el render
            SDL_RenderFillRect(renderer, &celdaRect);

            // dibujar la textura (escalada para dejar padding)
            SDL_FRect imgRect = { x + pad, y + pad, imgW, imgH };


            //IMPORTANTISMO:;
            SDL_Texture *tex = (tablero[i][j] == 'K') ? texKnight : texDemon;
            //esa ternaria se fija si la celda de la grilla tiene un demonio o un caballero y entonces elige la textura


            if (tex)
            {
                // renderizamos la textura escalada a imgRect
                SDL_RenderTexture(renderer, tex, NULL, &imgRect);
            }



            //esto es chequeo de seguridad, no debería haber llamado sin mostrar Grilla pero es util chequear
            if (mostrarGrilla)
            {

                //si se cumple entonces hay que ya cambiarlooo
                SDL_SetRenderDrawColor(renderer, gridColor.r, gridColor.g, gridColor.b, gridColor.a);
                SDL_RenderRect(renderer, &celdaRect);
            }

            //  AHORA: Que pasa si epasa el cursor sobre la celda, hay que iluminar


            //iluminacion
            if ((int)i == luzHoverFila && (int)j == luzHoverColumna)
            {
                //podia ser otra celda, pero si estoy aca dentro
                //ess porque justo es esta de aca


                SDL_SetRenderDrawColor(renderer, highlightColor.r, highlightColor.g, highlightColor.b, highlightColor.a);
                SDL_RenderRect(renderer, &celdaRect);
            }
        }
    }
    //ya salido de ese for, ahora que se vea un borde adicional para todo el tablero
    if(mostrarGrilla)
    {
        //menos mal que guarde todo en tableroW y tableroH
        SDL_FRect boardRect = { offsetX, offsetY, (float)tableroW, (float)tableroH };
        SDL_SetRenderDrawColor(renderer, gridColor.r, gridColor.g, gridColor.b, gridColor.a);
        SDL_RenderRect(renderer, &boardRect);
    }
}
bool hizoClickEnTablero(int mouseX,int mouseY,char** tablero,size_t tamTablero, int tamCelda)
{
    float offsetEnX = (WIDTH - tamTablero*tamCelda)/2;
    float offsetEnY = (HEIGHT - tamTablero*tamCelda)/2;

    //este if basicamente pregunta si estoy dentro de la grilla, por muy raro que parezca.
    //el offset es donde empieza la grilla
    //si le sumo tamTablero*tamCelda, sumo toda la linea o toda la columna, miden lo mismo
     if (mouseX >= offsetEnX && mouseX <= offsetEnX + tamTablero*tamCelda &&
        mouseY >= offsetEnY && mouseY <= offsetEnY + tamTablero*tamCelda)
    {
        return true;
    }
    else
    {
        return false;
    }
}
int ejecutarJugadaSDL(const int orden,char** mat, int tamCelda, tJugada *jug)
{
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
