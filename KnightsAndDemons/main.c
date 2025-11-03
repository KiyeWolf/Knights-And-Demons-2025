#include "menu.h"
#include "./Juego/TDAJuego.h"
#include "./Juego/scores.h"
#include "./Juego/creditos.h"
#include "./Partida/TDAPartida.h"
#include "./Tablero/TDATablero.h"

#include "./SDL3-Archivos/Visual/Visual.h"
int main(int argc, char *argv[]) {
    Admin elAdmin = {0};
    system("chcp 65001 >nul");
    system("cls");

    //PRUEBA SDL
    if(funcionDePrueba()!=0)
    {
        return 10;
    }


    char opcionElegida = mostrarMenuPrincipalConMensaje(MENSAJE_DEL_MENU_PRINCIPAL, OPCIONES_MENU_PRINCIPAL);

    while(opcionElegida!=SALIR)
    {
            if(opcionElegida==CARGAR_PARTIDA)
            {
                char guardado[TAM_NOMBRE+1]={0};
                system("cls");
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
                }

            }


            if(opcionElegida==INICIAR_NUEVA_PARTIDA)
            {
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
            }
            if(opcionElegida==CREDITOS)
            {
                system("cls");
                mostrarCreditos();
            }
            if(opcionElegida==TABLA_DE_PUNTAJES)
            {
                system("cls");
                mostrarTablaDePuntajes();
                pausaYLimpiadoDePantalla();
            }
            system("cls");
            opcionElegida = mostrarMenuPrincipalConMensaje(MENSAJE_DEL_MENU_PRINCIPAL, OPCIONES_MENU_PRINCIPAL);

    }
        puts("Gracias por jugar");
        limpiarBuffer();
        getchar();
        return 0;
}

/*Se encarga de mostrar un menu recibido de parámetro, y buscar entre las opciones disponibles
también recibidas por parámetro. Además valida si la opción elegida por el usuario no es las propuestas.*/
char mostrarMenuPrincipalConMensaje(char* msj, char* opciones)
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
}

/*Espera una entrada por teclado del usuario, y según esa entrada es el tipo retorno,
en este caso cada número de retorno representa una dificultad, por ejemplo 1 Facil*/
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
