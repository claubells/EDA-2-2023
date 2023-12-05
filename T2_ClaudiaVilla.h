#ifndef T2_CLAUDIAVILLA_H
#define T2_CLAUDIAVILLA_H


/*------------- Variables Globales -------------*/
int numPalabrasBuscar = 0; // Numero de palabras a buscar en el tablero
int filas = 0;
int columnas = 0;
FILE* archivoSalidaLista;
/*------------- Estructura de datos -------------*/

// Estructura para representar un nodo en el tablero, la cual se representara en una lista larga
typedef struct NodoCaracter{
    char caracter;
    struct NodoCaracter* siguiente; // encontrar la palabra en el tablero
} NodoCaracter;

// Estructura para representar un nodo en el la lista de palabras a buscar
typedef struct NodoLista{
    char palabra[60];
    struct NodoLista* siguiente;
} NodoLista;

// Estructura para representar un nodo en el la lista del tablero
typedef struct PosicionPalabra{
    int fila;
    int columna;
    int direccionFila;
    int direccionColumna;
    struct PosicionPalabra* siguiente;
} PosicionPalabra;
/*----------------- Operaciones -----------------*/

/*
Entrada: Una cadena de caracteres que representa el nombre del archivo a leer.
Descripción: Lee una lista de palabras desde un archivo '.lst'. Construye y retorna una lista enlazada donde cada nodo contiene una de las palabras leídas.
Retorno: Un puntero a la cabeza de la lista enlazada de palabras, o nulo sino se pudo leer.
*/
NodoLista* LeerLista(const char * lista){
    
    NodoLista* cabeza = NULL;
    NodoLista* actual = NULL;

    FILE* archivoLista = fopen(lista, "r");// Abrimos el archivo

    // Verificar si se abrió correctamente
    if (archivoLista == NULL) {
        printf("Error al abrir el archivo '%s'\n.", lista);
        return NULL;
    }

    // Leer la cantidad de palabras a buscar, primer numero
    fscanf(archivoLista, "%d", &numPalabrasBuscar);

    // Leer las palabras y construir la lista enlazada con ellas
    for (int i = 0; i < numPalabrasBuscar; i++) {
        NodoLista* nuevoNodo = (NodoLista*)malloc(sizeof(NodoLista));
        if (nuevoNodo == NULL) {
            printf("Error al asignar memoria");
            return NULL;
        }

        fscanf(archivoLista, "%s", nuevoNodo->palabra);
        nuevoNodo->siguiente = NULL;

        if (cabeza == NULL) {
            cabeza = nuevoNodo;
            actual = nuevoNodo;
        } else {
            actual->siguiente = nuevoNodo;
            actual = nuevoNodo;
        }
    }
    
    fclose(archivoLista);// Cerrar el archivo

    return cabeza;
}

/*
Entrada: Un carácter que será el valor del nodo.
Descripción: Crea un nuevo nodo de tipo NodoCaracter con el carácter proporcionado
Retorno: Un puntero al nuevo nodo creado. Retorna NULL si la asignación de memoria falla.
*/
NodoCaracter* CrearNodo(char caracter) {
    NodoCaracter *nuevoNodo = (NodoCaracter *)malloc(sizeof(NodoCaracter));
    if (nuevoNodo == NULL) {
        printf("Error al crear un nuevo nodo");
        return NULL;
    }
    nuevoNodo->caracter = caracter;
    nuevoNodo->siguiente = NULL;
    return nuevoNodo;
}

/*
Entrada:-Un puntero doble a NodoCaracter que representa la cabeza de la lista enlazada.
        -El carácter a agregar en la lista.
Descripción: Añade un nuevo nodo con el carácter proporcionado al final de la lista enlazada.
Retorno: No retorna nada,
*/
void AgregarNodo(NodoCaracter **cabeza, char caracter) {
    NodoCaracter *nuevoNodo = CrearNodo(caracter);
    if (*cabeza == NULL) {
        *cabeza = nuevoNodo;
    } else {
        NodoCaracter *temp = *cabeza;
        while (temp->siguiente != NULL) {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevoNodo;
    }
}

/*
Entrada:-Un puntero a FILE para escribir el carácter.
        -El carácter para escribir en el archivo.
        -Un puntero a char para almacenar el siguiente carácter.
Descripción: Escribe un carácter en el archivo y maneja caracteres especiales.
Retorno: No retorna nada,
*/
void EscribirCaracter(FILE* archivo, char c, char* siguienteCaracter) {
    static char ultimoCaracter = '\0';
    
    if (ultimoCaracter != '\0') {
        fputc(ultimoCaracter, archivo); 
    }
    ultimoCaracter = c;

    *siguienteCaracter = ultimoCaracter; // Actualiza el siguiente carácter
}

// Función para leer el archivo tablero.ini y almacenar su contenido en la lista enlazada
void LeerTablero(const char *nombreArchivo, NodoCaracter **cabeza) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo '%s'.\n", nombreArchivo);
        return;
    }
    // Leer las dimensiones del tablero y guardarlas
    fscanf(archivo, "%d %d\n", &filas, &columnas);

    char caracter;
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            fscanf(archivo, "%c ", &caracter);
            AgregarNodo(cabeza, caracter);
        }
    }
    fclose(archivo); //cerramos el archivo
}


/*
Entrada: -Un puntero a NodoCaracter que representa la cabeza de la lista enlazada del tablero.
         -Un entero que representa la fila en el tablero.
         -Un entero que representa la columna en el tablero.
Descripción: Obtiene el nodo en la posición especificada de la matriz.
Retorno: Un puntero a NodoCaracter en la posición dada.
*/
NodoCaracter* ObtenerNodoEnPosicion(NodoCaracter* cabeza, int fila, int columna) {
    int posicion = fila * columnas + columna;
    NodoCaracter* actual = cabeza;

    for (int i = 0; i < posicion && actual != NULL; i++) {
        actual = actual->siguiente;
    }
    
    return actual;
}

/*
Entrada: -Un puntero a NodoCaracter que representa la cabeza de la lista enlazada del tablero.
         -Un entero que indica el número de filas del tablero.
         -Un entero que indica el número de columnas del tablero.
         -Una cadena de caracteres que representa la palabra a buscar en el tablero.
Descripción: Busca una palabra en las 8 direcciones posibles, guardando las posiciones de inicio si se encuentra la palabra.
Retorno: Un puntero a PosicionPalabra que representa la lista de posiciones donde se encontró la palabra.
*/
PosicionPalabra* BuscarPalabraEnTablero(NodoCaracter* cabeza, int filas, int columnas, const char* palabra) {
    PosicionPalabra* posiciones = NULL;
    // Direcciones posibles
    int direcciones[][2] = {
        {0, 1},  // Horizontal derecha
        {1, 0},  // Vertical abajo
        {1, 1},  // Diagonal derecha abajo
        {1, -1}, // Diagonal izquierda abajo
        {0, -1}, // Horizontal izquierda
        {-1, 0}, // Vertical arriba
        {-1, 1}, // Diagonal derecha arriba
        {-1, -1} // Diagonal izquierda arriba
    };

    // Iteramos sobre todas las celdas del tablero
    for (int fila = 0; fila < filas; fila++) {
        for (int columna = 0; columna < columnas; columna++) {
            // Buscamos en las 8 posiciones si existe la palabra
            for (int dir = 0; dir < 8; dir++) {
                int actualFila = fila;
                int actualColumna = columna;
                int dirFila = direcciones[dir][0];
                int dirColumna = direcciones[dir][1];
                NodoCaracter* actualNodo = ObtenerNodoEnPosicion(cabeza, actualFila, actualColumna);

                int cnt = 0;
                while (actualNodo != NULL && palabra[cnt] != '\0' && actualNodo->caracter == palabra[cnt]) {
                    // Mover a la siguiente celda en la dirección actual
                    if (cnt == strlen(palabra) - 1) {
                        // La palabra se encontró completamente
                        PosicionPalabra* nuevaPosicion = (PosicionPalabra*)malloc(sizeof(PosicionPalabra));
                        if (nuevaPosicion == NULL) {
                            printf("Error al asignar memoria.\n");
                            return NULL;
                        }
                        nuevaPosicion->fila = fila;
                        nuevaPosicion->columna = columna;
                        nuevaPosicion->direccionFila = dirFila;
                        nuevaPosicion->direccionColumna = dirColumna;
                        nuevaPosicion->siguiente = posiciones;
                        posiciones = nuevaPosicion;
                        break;
                    }

                    actualFila += dirFila;
                    actualColumna += dirColumna;
                    if (actualFila < 0 || actualFila >= filas || actualColumna < 0 || actualColumna >= columnas) {
                        break;
                    }

                    actualNodo = ObtenerNodoEnPosicion(cabeza, actualFila, actualColumna);
                    
                    cnt++;
                }
            }
        }
    }
    return posiciones;
}


/*
Entrada: -Una cadena de caracteres que representa la palabra encontrada.
         -Un puntero a PosicionPalabra que representa la lista de posiciones donde se encontró la palabra.
         -Un puntero a un entero que lleva la cuenta de las palabras encontradas.
         -Un puntero a FIlE archivoSalidaLista, para escribir en él
Descripción: Imprime las posiciones de inicio de las palabras encontradas y acumula la salida en un buffer.
Retorno: No retorna 
*/
void ImprimirPosiciones(const char* palabra, PosicionPalabra* posiciones, int* contadorPalabrasEncontradas, FILE* archivoSalidaLista) {
    PosicionPalabra* actual = posiciones;
    while (actual != NULL) {
        fprintf(archivoSalidaLista, "%d %d (%s)\n", actual->fila + 1, actual->columna + 1, palabra);
        actual = actual->siguiente;
        (*contadorPalabrasEncontradas)++;
    }
}

void EscribirEnArchivoSalida(FILE* archivoSalidaLista, int contadorPalabrasEncontradas) {
    fprintf(archivoSalidaLista, "%d\n", contadorPalabrasEncontradas);
}

/*
Entrada:-Un puntero a FILE que representa el archivo donde se escribirá el tablero.
        -Un puntero a NodoCaracter que representa la cabeza de la lista enlazada del tablero.
        -Un entero que representa el número de filas del tablero.
        -Un entero que representa el número de columnas del tablero.
Descripción: Escribe la representación del tablero en el archivo dado. El tablero está representado por una lista enlazada de NodoCaracter.
Retorno: No retorna.
*/
void EscribirTableroEnArchivo(FILE* archivo, NodoCaracter* cabeza, int filas, int columnas) {
    NodoCaracter* actual = cabeza;
    char ultimoCaracter = '\0'; // Almacena el último carácter escrito

    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            if (actual != NULL) {

                fputc(actual->caracter, archivo); // Escribe el carácter actual
                fputc(' ', archivo);
                
                ultimoCaracter = actual->caracter; // Actualiza el último carácter escrito
                actual = actual->siguiente;
            }
        }
        fputc('\n', archivo); // Nueva línea al final de cada fila
    }
    fputc('\n', archivo); // Nueva línea al final de cada fila
}

// Función para liberar la memoria de la lista de posiciones
void LiberarPosiciones(PosicionPalabra* posiciones) {
    PosicionPalabra* actual = posiciones;
    PosicionPalabra* siguiente = NULL;

    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
}

// Funcion para Clonar el tablero leido
NodoCaracter* ClonarTablero(NodoCaracter* cabezaOriginal, int filas, int columnas) {
    NodoCaracter* cabezaClonada = NULL;
    NodoCaracter* actualOriginal = cabezaOriginal;
    NodoCaracter** ultimoClonado = &cabezaClonada;

    while (actualOriginal != NULL) {
        *ultimoClonado = CrearNodo(actualOriginal->caracter);
        ultimoClonado = &((*ultimoClonado)->siguiente);
        actualOriginal = actualOriginal->siguiente;
    }

    return cabezaClonada;
}

// Funcion para reemplazar palabra encontrada en el tablero por un asterisco.
void ReemplazarPalabraPorAsterisco(NodoCaracter* cabeza, PosicionPalabra* posicion, const char* palabra) {
    for (int i = 0; palabra[i] != '\0'; i++) {
        NodoCaracter* nodo = ObtenerNodoEnPosicion(cabeza, posicion->fila + i * posicion->direccionFila, posicion->columna + i * posicion->direccionColumna);
        if (nodo != NULL) {
            nodo->caracter = '*'; // Reemplazar por paréntesis
        }
    }
}

#endif 