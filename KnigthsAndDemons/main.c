#include "menu.h"
#include "./Juego/TDAJuego.h"
#include "./Juego/scores.h"
#include "./Juego/creditos.h"
#include "./Partida/TDAPartida.h"
#include "./Tablero/TDATablero.h"
int main() {
    Admin elAdmin = {0};
    system("chcp 65001 >nul");
    system("cls");
    char opcionElegida = mostrarMenuPrincipalConMensaje(MENSAJE_DEL_MENU_PRINCIPAL, "12345");

    while(opcionElegida!=OPCIONES_VALIDAS)
    {
        if(opcionElegida==CARGAR_PARTIDA)
        {
            char guardado[4];
            system("cls");
            puts("CARGAR PARTIDA");
            printf("Ingresa el nombre con el que se guardo la partida(3 caracteres alfanumericos):\n");
            limpiarBuffer();
            fgets(guardado, sizeof(guardado), stdin);
            guardado[strcspn(guardado, "\n")] = '\0'; //saco el \n y lo cambio por el nulo
            //VALIDAR QUE NO ENCUENTRA PARTIDA
            if(cargarPartida(&elAdmin, guardado)!=TODO_OK)
            {
                puts("Lo siento, no se encontró la partida Cargada");
                system("pause");
            }
            else
            {
                jugar(&elAdmin);

            }

        }
        if(opcionElegida==INICIAR_NUEVA_PARTIDA)
        {
            char guardado[4];
            system("cls");
            puts("INICIO");
            printf("Ingresa el nombre con el que se inicie la partida(3 caracteres alfanumericos):\n");
            limpiarBuffer();
            fgets(guardado, sizeof(guardado), stdin);
            guardado[strcspn(guardado, "\n")] = '\0';
            if (strlen(guardado) < MIN_CANTIDAD_LETRAS_NOMBRE) {
                puts("Nombre demasiado corto. Se usarán valores por defecto.");
                strcpy(guardado, "AAA");
                system("pause");
            }
            system("cls");
            int d = solicitarDificultad();
            iniciarJuegoNuevo(&elAdmin, &d, guardado);

            jugar(&elAdmin);
            //colocarJugadorEnTablaDePuntajes(elAdmin.jugador.nombre,elAdmin.jugador.TotalestadoUno,elAdmin.jugador.TotalestadoDos,elAdmin.jugador.pikasRestantes,elAdmin.jugador.tiempoDeJuego);
        }
        if(opcionElegida==CREDITOS)
        {
            mostrarCreditos();
        }
        if(opcionElegida==TABLA_DE_PUNTAJES)
        {
            mostrarTablaDePuntajes();
            pausaYLimpiadoDePantalla();
        }
        system("cls");
        opcionElegida = mostrarMenuPrincipalConMensaje(MENSAJE_DEL_MENU_PRINCIPAL, "12345");

    }
    puts("Gracias por jugar");
    limpiarBuffer();
    getchar();
    return 0;
}
char mostrarMenuPrincipalConMensaje(char* msj, char* opciones)
{
    char opc;
    puts(msj);
    scanf(" %c", &opc);
    while(!strchr(opciones,opc))
    {
       //system("cls");
        puts("Porfavor Ingrese una de las opciones mostradas");
        limpiarBuffer();
        //system("cls");
        puts(msj);
        scanf(" %c", &opc);
    }
    //system("cls");
    return opc;
}
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
void pausaYLimpiadoDePantalla()
{
    limpiarBuffer();
    getchar();
 //   system("cls");
}
void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
