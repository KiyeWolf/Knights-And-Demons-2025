#include "menu.h"
#include "./Juego/TDAJuego.h"
#include "./Juego/scores.h"
#include "./Juego/creditos.h"
#include "./Partida/TDAPartida.h"
#include "./Tablero/TDATablero.h"

#include "./SDL3-Archivos/Visual/Visual.h"
int main(int argc, char *argv[]) {
    Admin elAdmin = {0};
    //system("chcp 65001 >nul");
    //system("cls");
    size_t retornos;
    //PRUEBA SDL
    /*if(funcionDePrueba()!=0)
    {
        return 10;
    }*/

    //MENU ANTIGUOOOOO
    //char opcionElegida = mostrarMenuPrincipalConMensaje(MENSAJE_DEL_MENU_PRINCIPAL, OPCIONES_MENU_PRINCIPAL);

    //MENU NUEVOOO
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("Knights && Demons", WIDTH, HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    TTF_Font* font = TTF_OpenFont(RUTA_ARCHIVO_FUENTES_DE_TEXTO_1, 28);


    //SIEMPRE QUE HAYAN PROBLEMAS, QUITAR LA PANTALLA COMPLETA PARA DEBUG
    //SDL_SetWindowFullscreen(window, true);

    mostrarPantallaBienvenida(renderer,font);
    char opcionElegida;
    mostrarMenuPrincipal(renderer,font,&opcionElegida);
    while(opcionElegida!=SALIR)
    {
            if(opcionElegida==CARGAR_PARTIDA)
            {

                /*system("cls");
                puts("CARGAR PARTIDA");
                printf("Ingresa el nombre con el que se guardo la partida(%d caracteres alfanumericos):\n", TAM_NOMBRE);
                fflush(stdin);
                gets(guardado);
                if(strlen(guardado)>TAM_NOMBRE || strlen(guardado)<TAM_NOMBRE)
                {
                    while(strlen(guardado)>TAM_NOMBRE || strlen(guardado)<TAM_NOMBRE)
                    {
                        puts("Error, el nombre es inválido, respeta la longitud.\n");
                        system("pause");
                        system("cls");
                        printf("Ingresa el nombre con el que se guardo la partida(%d caracteres alfanumericos):\n", TAM_NOMBRE);
                        fflush(stdin);
                        gets(guardado);
                    }
                }
                guardado[strcspn(guardado, "\n")] = '\0'; //saco el \n y lo cambio por el nulo
                //VALIDAR QUE NO ENCUENTRA PARTIDA
                if(cargarPartida(&elAdmin, guardado)!=TODO_OK)
                {
                    puts("Lo siento, no se encontró la partida Cargada");
                    system("pause");
                }
                else
                {
                    if(jugar(&elAdmin)==JUEGO_COMPLETADO)
                    {
                        colocarJugadorEnTablaDePuntajes(elAdmin.jugador.nombre,elAdmin.jugador.TotalestadoUno,elAdmin.jugador.TotalestadoDos,elAdmin.jugador.pikasRestantes,elAdmin.jugador.nivelesCompletados);
                    }
                }*/

                //NUEVO SDL3
                char nombre[TAM_NOMBRE+1]={0};
                mostrarMensajeEnVentanaYBorrarDespuesDeTecla(renderer,font,"[ Debes ingresar el nombre con el que se  guardo la partida]");
                mostrarPantallaNombre(renderer,font,nombre, window);
                if(cargarPartida(&elAdmin, nombre)!=TODO_OK)
                {
                    mostrarMensajeEnVentanaYBorrarDespuesDeTecla(renderer,font,"Lo siento, no se encontró la partida Cargada");
                }
                else
                {
                    if(jugar(&elAdmin)==JUEGO_COMPLETADO)
                    {
                        colocarJugadorEnTablaDePuntajes(elAdmin.jugador.nombre,elAdmin.jugador.TotalestadoUno,elAdmin.jugador.TotalestadoDos,elAdmin.jugador.pikasRestantes,elAdmin.jugador.nivelesCompletados);
                    }
                }
            }


            if(opcionElegida==INICIAR_NUEVA_PARTIDA)
            {
                //ANTIGUO USO
                /*
                char guardado[TAM_NOMBRE+1]={0};
                system("cls");
                puts("INICIO");
                printf("Ingresa el nombre con el que se inicie la partida(%d caracteres alfanumericos):\n", TAM_NOMBRE-1);
                fflush(stdin);
                gets(guardado);
                if(strlen(guardado)>TAM_NOMBRE || strlen(guardado)<TAM_NOMBRE)
                {
                    while(strlen(guardado)>TAM_NOMBRE || strlen(guardado)<TAM_NOMBRE)
                    {
                        puts("Error, el nombre es inválido, respeta la longitud.\n");
                        system("pause");
                        system("cls");
                        printf("Ingresa el nombre con el que desea iniciar la partida(%d caracteres alfanumericos):\n", TAM_NOMBRE);
                        fflush(stdin);
                        gets(guardado);
                    }
                }
                guardado[strcspn(guardado, "\n")] = '\0';
                system("cls");
                int d = solicitarDificultad();
                iniciarJuegoNuevo(&elAdmin, &d, guardado);
                mostrarHistoriaInicial();
                if(jugar(&elAdmin)==JUEGO_COMPLETADO)
                {
                    colocarJugadorEnTablaDePuntajes(elAdmin.jugador.nombre,elAdmin.jugador.TotalestadoUno,elAdmin.jugador.TotalestadoDos,elAdmin.jugador.pikasRestantes,elAdmin.jugador.nivelesCompletados);
                }
                */

                //NUEVO CON SDL3
                char nombre[TAM_NOMBRE+1];
                int d;
                mostrarPantallaNombre(renderer,font,nombre, window);
                mostrarPantallaDificultad(renderer,font,&d);
                iniciarJuegoNuevo(&elAdmin, &d, nombre);
                mostrarPantallaHistoriaInicial(renderer,font,&retornos);
                if(jugar(&elAdmin)==JUEGO_COMPLETADO)
                {
                    colocarJugadorEnTablaDePuntajes(elAdmin.jugador.nombre,elAdmin.jugador.TotalestadoUno,elAdmin.jugador.TotalestadoDos,elAdmin.jugador.pikasRestantes,elAdmin.jugador.nivelesCompletados);
                }
            }
            if(opcionElegida==CREDITOS)
            {
                mostrarCreditosEnPantalla(renderer,font);
            }
            if(opcionElegida==TABLA_DE_PUNTAJES)
            {
                /*system("cls");
                mostrarTablaDePuntajes();
                pausaYLimpiadoDePantalla();
                */
               mostrarTablaDePuntajesDeArchivo(renderer,font);
            }
            mostrarMenuPrincipal(renderer,font,&opcionElegida);;

    }
        mostrarMensajeEnVentanaYBorrarDespuesDeTecla(renderer,font,"Gracias Por jugar");

        //destrozar los usos de sdl2
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();


        return 0;
}

/*Se encarga de mostrar un menu recibido de parámetro, y buscar entre las opciones disponibles
también recibidas por parámetro. Además valida si la opción elegida por el usuario no es las propuestas.*/

//ANTIGUA FUNCION, YA REEMPLAZADA EN SDL3
/*char mostrarMenuPrincipalConMensaje(char* msj, char* opciones)
{
    char opc;
    puts(msj);
    scanf(" %c", &opc);
    while(!strchr(opciones,opc))
    {
        puts("Porfavor Ingrese una de las opciones mostradas");
        limpiarBuffer();
        puts(msj);
        scanf(" %c", &opc);
    }
    return opc;
}*/

/*Espera una entrada por teclado del usuario, y según esa entrada es el tipo retorno,
en este caso cada número de retorno representa una dificultad, por ejemplo 1 Facil*/
//ANTIGUA FUNCION. Ya reemplazada en SDL3
/*
int solicitarDificultad()
{
    char dificultadElegidaPorElUsuario = mostrarMenuPrincipalConMensaje("Ingrese la dificultad para jugar:\n1. FACIL\n2. MEDIO\n3. DIFICIL","123");
    switch(dificultadElegidaPorElUsuario)
    {
        case '1':
        return 1;
        break;
        case '2':
        return 2;
        break;
        case '3':
        return 3;
        break;
        default:
        return 1; //auqnque no debería pasar
    }
    return 1; //no debería suceder
}
*/
/*Limpia el salto de línea del teclado y luego hace una pausa en el mismo.*/
void pausaYLimpiadoDePantalla()
{
    limpiarBuffer();
    getchar();
}

/*Limpia el salto de linea al cometer un input del usuario*/
void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
