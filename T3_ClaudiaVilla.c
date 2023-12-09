#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define SIZE 60 //tamaño de la tabla hash
#include "T3_ClaudiaVilla.h"

int main(){

	char nombreTablero[40];
	char nombreLista[40];

	printf("\n/*------------- Bienvenido a DerechoReves -------------*/\n");
	printf(" Por favor ingrese los datos con sus respectivos '.ini' o '.lst'\n");
	printf("  -Nombre del archivo.ini: ");
	scanf("%39s", nombreTablero);
	printf("  -Nombre del archivo.lst: ");
	scanf("%39s", nombreLista);

	clock_t inicio, fin;
    double tiempoUsado;
    inicio = clock(); // Aqui empezamos a contar el tiempo	

    InicializarTablaHash();
    LeerLista(nombreLista);

    FILE* archivoTablero = fopen(nombreTablero, "r");
    if (archivoTablero == NULL) {
        printf("Error al abrir el archivo '%s'\n.", nombreTablero);
        return 1;
    }else{
    	printf("Se ha podido leer correctamente el archivo '%s'.\n", nombreTablero);
    }

    LeerTableroNum(archivoTablero);
    nodografo* tablero = CrearTablero(); //Si cae a error esta dentro de la funcion.

    ConectarNodos(tablero);
    LeerTablero(archivoTablero, tablero);
    fclose(archivoTablero);

    archivoTableroOut = fopen("tablero.out", "w");
    if (archivoTableroOut == NULL) {
        printf("No se ha podido abrir el archivo 'tablero.out'.\n");
        return 1;
    }

    archivoListaOut = fopen("lista.out", "w");
    if (archivoListaOut == NULL) {
        printf("No se ha podido abrir el archivo 'lista.out'.\n");
        return 1;
    }

    printf(" -Palabras a Buscar: %i.\n", numPalabrasBuscar);
    BuscarPalabras(tablero, TablaHash);

    fin = clock();
    tiempoUsado = ((double) (fin - inicio)) / CLOCKS_PER_SEC;
    printf(" -Tiempo usado: %f segundos.\n", tiempoUsado);
	return 0;
}
