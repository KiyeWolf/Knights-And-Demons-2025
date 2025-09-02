#include <stdio.h>

#include <stdlib.h>
#include <stdbool.h>
#include "creditos.c"
#include "TDAJuego.c"
#include "scores.c"



int solicitarDificultad();
char menu(char* msj, char* opciones);
int main() {
    Admin elAdmin;
    // TODO: Implement menu logic here
    // Example menu loop
    char mensajeMenu[1000]={"------------------------------------------------\n\tBienvenido a Knights && Demons (2025)\n------------------------------------------------\n\t1. CARGAR PARTIDA \n\t2. NUEVO JUEGO\n\t3. CREDITOS\n\t4. TABLA DE PUNTAJES\n\t5. SALIR"};
    char opcion = menu(mensajeMenu, "12345");
    while(opcion!='5')
    {
        if(opcion=='1')
        {
            char guardado[4];
            //empieza el juego
            puts("CARGAR PARTIDA");
            printf("Ingresa el nombre con el que se guardo la partida(3 caracteres alfanumericos):\n");
            fflush(stdin);
            gets(guardado);
            cargarPartida(&elAdmin, guardado);
            //break;
        }
        if(opcion=='2')
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
        }
        if(opcion=='3')
        {
            mostrarCreditos();
        }
        if(opcion=='4')
        {
            mostrarTablaDePuntajes();
            fflush(stdin);
            getchar();
            system("cls");
        }
        opcion = menu(mensajeMenu, "12345");
        
    }
    puts("Gracias por jugar");
    fflush(stdin);
    getchar();
    return 0;
}
char menu(char* msj, char* opciones)
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
    char d = menu("Ingrese la dificultad para jugar:\n1. FACIL\n2. MEDIO\n3. DIFICIL","123");
    return atoi(&d);
}
