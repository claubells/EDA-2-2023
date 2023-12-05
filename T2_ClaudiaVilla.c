#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "T2_ClaudiaVilla.h"

int main(){
    clock_t inicio, fin;
    double tiempoUsado;
    inicio = clock(); // Aqui empezamos a contar el tiempo

	char nombreLista[30] = "listap2.lst";
    char nombreTablero[30] = "prueba6.ini";
    int contadorPalabrasEncontradas = 0;

    FILE* archivoTableroOut = fopen("tablero.out", "w");
    if (archivoTableroOut == NULL) {
        printf("No se ha podido abrir el archivo'%s'.\n", nombreTablero);
        return 1;
    }

    archivoSalidaLista = fopen("lista.out", "w");
    if (archivoSalidaLista == NULL) {
        printf("Error al abrir el archivo 'lista.out'.\n");
        return 1;
    }

    NodoCaracter *cabeza = NULL;
    LeerTablero(nombreTablero, &cabeza);

    NodoLista* listaPalabras = LeerLista(nombreLista);
    NodoLista* palabraActual = listaPalabras;

    while (palabraActual != NULL) { //Hacemos el while solo para contar las palabras
        PosicionPalabra* posiciones = BuscarPalabraEnTablero(cabeza, filas, columnas, palabraActual->palabra);

        if (posiciones != NULL) {
            contadorPalabrasEncontradas++;
            LiberarPosiciones(posiciones);
        }
        palabraActual = palabraActual->siguiente;
    }

    fprintf(archivoSalidaLista,"%i\n", contadorPalabrasEncontradas);


    listaPalabras = LeerLista(nombreLista);
    palabraActual = listaPalabras;
    while (palabraActual != NULL) {
        PosicionPalabra* posiciones = BuscarPalabraEnTablero(cabeza, filas, columnas, palabraActual->palabra);

        if (posiciones != NULL) {
            ImprimirPosiciones(palabraActual->palabra, posiciones, &contadorPalabrasEncontradas, archivoSalidaLista);
            NodoCaracter* cabezaClonada = ClonarTablero(cabeza, filas, columnas);
            ReemplazarPalabraPorAsterisco(cabezaClonada, posiciones, palabraActual->palabra);
            EscribirTableroEnArchivo(archivoTableroOut, cabezaClonada, filas, columnas);
        }

        palabraActual = palabraActual->siguiente;
    }        

    // Liberamos Memoria de la lista enlazada 
    NodoLista* actual = listaPalabras;
    NodoLista* siguiente = NULL;

    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }

    // Liberamos memoria del tablero
    NodoCaracter* actual2 = cabeza;
    NodoCaracter* siguienteNodo = NULL;

    while (actual2 != NULL) {
        siguienteNodo = actual2->siguiente;
        free(actual2);
        actual2 = siguienteNodo;
    }
    // Cerramos los archivos
    fclose(archivoTableroOut);
    fclose(archivoSalidaLista);

    fin = clock();
    tiempoUsado = ((double) (fin - inicio)) / CLOCKS_PER_SEC;
    printf("Tiempo usado: %f segundos\n", tiempoUsado);
    return 0;
}