#include <stdio.h>
#include "menu.h"
int main() {
    Admin elAdmin;
    char opcionElegida = mostarMenuPrincipalConMensaje(MENSAJE_DEL_MENU_PRINCIPAL, "12345");
    while(opcionElegida!=OPCIONES_VALIDAS)
    {
        if(opcionElegida==CARGAR_PARTIDA)
        {
            char guardado[4];
            //empieza el juego
            puts("CARGAR PARTIDA");
            printf("Ingresa el nombre con el que se guardo la partida(3 caracteres alfanumericos):\n");
            fflush(stdin);
            gets(guardado);
            cargarPartida(&elAdmin, guardado);
            jugar(&elAdmin);
            //break;
        }
        if(opcionElegida==INICIAR_NUEVA_PARTIDA)
        {
            char guardado[4];
            puts("INICIO");
            printf("Ingresa el nombre con el que se inicie la partida(3 caracteres alfanumericos):\n");
            fflush(stdin);
            gets(guardado);
            int d = solicitarDificultad();
            //printf("%d", d);
            iniciarJuegoNuevo(&elAdmin, &d, guardado);
            jugar(&elAdmin);
            colocarJugadorEnTablaDePuntajes(&(elAdmin.jugador));
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
        opcionElegida = mostarMenuPrincipalConMensaje(MENSAJE_DEL_MENU_PRINCIPAL, "12345");
        
    }
    puts("Gracias por jugar");
    fflush(stdin);
    getchar();
    return 0;
}
char mostarMenuPrincipalConMensaje(char* msj, char* opciones)
{
    char opc;
    puts(msj);
    fflush(stdin);
    scanf("%c", &opc);
    while(!strchr(opciones,opc))
    {
        system("cls");
        puts("Porfavor Ingrese una de las opciones mostradas");
        fflush(stdin);
        getchar();
        system("cls");
        puts(msj);
        fflush(stdin);
        scanf("%c", &opc);
    }
    system("cls");
    return opc;
}
int solicitarDificultad()
{
    char dificultadElegidaPorElUsuario = mostarMenuPrincipalConMensaje("Ingrese la dificultad para jugar:\n1. FACIL\n2. MEDIO\n3. DIFICIL","123");
    return atoi(&dificultadElegidaPorElUsuario);
}
void pausaYLimpiadoDePantalla()
{
    fflush(stdin);
    getchar();
    system("cls");
}