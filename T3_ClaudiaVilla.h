#ifndef T3_CLAUDIAVILLA_H
#define T3_CLAUDIAVILLA_H

/*------------- Variables Globales -------------*/
int numPalabrasBuscar = 0; // Numero de palabras a buscar en el tablero
int filas = 0;
int columnas = 0;
int palabrasEncontradas = 0;
FILE* archivoListaOut;
FILE* archivoTableroOut;
/*------------- Estructura de datos -------------*/
typedef struct nodoh{ // nodo para la tabla hash
	char palabra[100];// tamaño de la palabra
	struct nodoh* siguiente;
} nodoh;

typedef struct nodografo{ //Nodo para el grafo (el cual será el tablero)
	char letra;
	int visitado;
	struct nodografo* arriba;
	struct nodografo* abajo;
	struct nodografo* izquierda;
	struct nodografo* derecha;
	struct nodografo* derecha_arriba;
	struct nodografo* derecha_abajo;
	struct nodografo* izquierda_arriba;
	struct nodografo* izquierda_abajo;
} nodografo;

/*----------------- Operaciones -----------------*/
nodoh* TablaHash[SIZE]; // Creamos la tabla hash


/*Descripción: se incializa la tabla hash.
Entrada y Retorno: no hay*/
void InicializarTablaHash(){
	for(int i = 0; i < SIZE; i++){
		TablaHash[i] = NULL;
	}
}


/*Entrada: una palabra a buscar.
Descripción: Suma los codigos ASCII de los caracteres y luego toma el modulo 
con el tamaño de la tabla hash.
Retorno: el numero resultante.*/
int FuncionHash(char* palabra){ 
	int hash = 0;
	for(int i=0; palabra[i] != '\0'; i++){
		hash = (hash + palabra[i]) % numPalabrasBuscar;
	}
	return hash;
}


/*Entrada: una palabra a buscar.
Descripción: insertar la palabra en la tabla hash
Retorno: no hay.*/
void insertarHash(char* palabra) {
    int hash = FuncionHash(palabra);
    nodoh* nuevoNodoh = (nodoh*) malloc(sizeof(nodoh));

    strcpy(nuevoNodoh->palabra, palabra); //copiamos palabra a nuevoNodoh->palabra
    nuevoNodoh->siguiente = NULL;

    // Manejar colisiones con encadenamiento
    if (TablaHash[hash] == NULL) { // Sino hay ninguna lista enlazada en hash
        TablaHash[hash] = nuevoNodoh; // le asignamos el nuevo nodo
    } else { // Caso de choque
        nuevoNodoh->siguiente = TablaHash[hash]; // el nuevo nodo lo insertamos
        TablaHash[hash] = nuevoNodoh; // al incio, como una pila
    }
} 


/*Entrada: la lista con las palabras a buscar.
Descripción: guardar la cantidad de palabras a buscar e insertar
las palabras en la tabla hash.*/
void LeerLista(const char* lista){
	printf("\n/*-------------------- Resultados --------------------*/\n");
	FILE* archivoLista = fopen(lista, "r");// Abrimos el archivo
	// Verificar si se abrió correctamente
    if (archivoLista == NULL) {
        printf("Error al abrir el archivo '%s'\n.", lista);
        return;
    }else{
    	printf("Se ha podido leer correctamente el archivo '%s'.\n", lista);
    }
    // Leer la cantidad de palabras a buscar, primer numero
    fscanf(archivoLista, "%d", &numPalabrasBuscar);

    char palabra[100];
    while(fscanf(archivoLista, "%s", palabra)!= EOF){// EOF "End Of File"
    	insertarHash(palabra);
    }

    fclose(archivoLista);
}

/*Entrada: el nombre del tablero.
Descripción: guardamos los valores de filas y columnas en las variables globales
estos valores estan en el archivo tablero.
retorno: No hay. */
void LeerTableroNum(FILE* archivotablero){ 
    fscanf(archivotablero, "%i %i", &filas, &columnas);
}


/*Descripción: Crearemos un tablero de tipo nodografo, lo inicializamos todo en 0
o en NULL segun corresponda.
retorno: el tablero de tipo nodografo. */
nodografo* CrearTablero(){
	nodografo* tablero = (nodografo*)malloc(filas*columnas*sizeof(nodografo));
	if (tablero == NULL){
		printf("Error al asignar memoria para tablero. Funcion crearTablero.\n");
		return NULL;
	}

	for(int i = 0; i < filas*columnas; i++){ //Inicializamos todo en cero
		tablero[i].letra = 0;
		tablero[i].visitado = 0;
		tablero[i].arriba = NULL;
        tablero[i].abajo = NULL;
        tablero[i].izquierda = NULL;
        tablero[i].derecha = NULL;
        tablero[i].derecha_arriba = NULL;
        tablero[i].derecha_abajo = NULL;
        tablero[i].izquierda_arriba = NULL;
        tablero[i].izquierda_abajo = NULL;
	}
	return tablero;
}

/*Descripción: guardamos los valores de filas y columnas en las variables globales
estos valores estan en el archivo tablero.*/
void ConectarNodos(nodografo* tablero){
	for(int i = 0; i< filas; i++){
		for (int j = 0; j < columnas; j++){
			int posicion = (i * columnas)+ j;

			if (i > 0)
				tablero[posicion].arriba = &tablero[posicion-columnas];

			if (i < filas-1)
				tablero[posicion].abajo = &tablero[posicion+columnas];

			if(j < columnas - 1)
				tablero[posicion].derecha = &tablero[posicion+1];

			if(j > 0)
				tablero[posicion].izquierda = &tablero[posicion-1];

			if(j < columnas - 1 && i > 0)
				tablero[posicion].derecha_arriba = &tablero[posicion+1-columnas];

			if(j < columnas - 1 && i < filas-1)
				tablero[posicion].derecha_abajo = &tablero[posicion+1+columnas];

			if (j > 0 && i > 0)
				tablero[posicion].izquierda_arriba = &tablero[posicion-1-columnas];

			if (j > 0 && i < filas-1)
				tablero[posicion].izquierda_abajo = &tablero[posicion-1+columnas];
		}
	}
}

/*Entrada: el archivo del tablero y el tablero.
Descripción: guardamos los valores de filas y columnas en las variables globales
estos valores estan en el archivo tablero.
retorno: No hay. */
void LeerTablero(FILE* archivotablero, nodografo* tablero){
	char letra;
    for(int i = 0; i< filas; i++){
		for (int j = 0; j < columnas; j++){
			fscanf(archivotablero, " %c", &letra);

			int posicion = i * columnas + j;
			tablero[posicion].letra = letra;
		}
    }	
}


/*Entrada: el tablero, la palabra encontrada, su fila, columna, y la dirección en que se encontro la palabra
Descripción: se copia el tablero para proceder a reemplazar la palabra encontrada
por *, se clasifica con un switch para saber en que dirección se deberá rellenar con *
retorno: No hay. */
void ImprimirTablero(nodografo* tablero, const char* palabra, int fila, int columna, int direccion){
	nodografo* tableroCopia = (nodografo*)malloc(filas * columnas * sizeof(nodografo));
    if (tableroCopia == NULL) {
        printf("Error al asignar memoria.\n");
        return;
    }

    // Copiar el contenido del tablero original a la copia
    memcpy(tableroCopia, tablero, filas * columnas * sizeof(nodografo));

    int largoPalabra = strlen(palabra);
    for (int k = 0; k < largoPalabra; k++) {
    	int posicion = 0;
	    switch(direccion){
	    	case 1: // Se encontro en direccion derecha
		    	posicion = (fila * columnas) + columna + k;
		        break;
			case 2: // Se encontro en direccion izquierda
				posicion = (fila * columnas) + columna - k;
		        break;
			case 3: // Se encontro en direccion arriba
				posicion = ((fila - k) * columnas) + columna;
		        break;
			case 4: // Se encontro en direccion abajo
				posicion = ((fila + k) * columnas) + columna;
		        break;
			case 5: // Se encontro en direccion derecha arriba
				posicion = ((fila - k) * columnas) + columna + k;
		        break;
			case 6: // Se encontro en direccion derecha abajo
				posicion = ((fila + k) * columnas) + columna + k;
		        break;
			case 7: // Se encontro en direccion izquierda arriba
				posicion = ((fila - k) * columnas) + columna - k;
		        break;
			case 8: // Se encontro en direccion izquierda abajo
		    	posicion = ((fila + k) * columnas) + columna - k;
		        break;
		    default:
	        	break;
	    }
	    tableroCopia[posicion].letra = '*';
	}	

	// Imprimir y escribir en archivo
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            fprintf(archivoTableroOut, "%c ", tableroCopia[i * columnas + j].letra);
        }
        fprintf(archivoTableroOut, "\n");
    }

    fprintf(archivoTableroOut, "\n");
	// Liberar la memoria de la copia del tablero
    free(tableroCopia);
}

int buscarDesdeCelda(nodografo* tablero, int filaInicio, int colInicio, const char* palabra) {
    int largoPalabra = strlen(palabra);
    
    // Buscar derecha
    int encontrado = 1; // Asume que la palabra se encuentra hasta que se demuestre lo contrario
	if (colInicio + largoPalabra <= columnas) {
	    for (int k = 0; k < largoPalabra && encontrado; k++) {
	        if (tablero[(filaInicio * columnas) + colInicio + k].letra != palabra[k]) {
	            encontrado = 0; // Palabra no coincide
	        }
	    }
	    if (encontrado) {
	    	ImprimirTablero(tablero, palabra, filaInicio, colInicio, 1); // 1 va a significar derecha
	        return 1; // Palabra encontrada hacia la derecha
	    }
	}
    encontrado = 1;

    //Buscar izquierda
	if (colInicio - largoPalabra >= -1) {
	    for (int k = 0; k < largoPalabra && encontrado; k++) {
	        if (tablero[(filaInicio * columnas) + colInicio - k].letra != palabra[k]) {
	            encontrado = 0;
	        }
		}
	    if (encontrado) {
	    	ImprimirTablero(tablero, palabra, filaInicio, colInicio, 2); // 2 va a significar izquierda
	        return 1; // Palabra encontrada hacia la izquierda
	    }
	}
	encontrado = 1;


	//Buscar hacia arriba
	if (filaInicio - largoPalabra >= -1) {
	    for (int k = 0; k < largoPalabra && encontrado; k++) {
	        if (tablero[(filaInicio - k) * columnas + colInicio].letra != palabra[k]) {
	            encontrado = 0;
	        }
	    }
	    if (encontrado) {
	    	ImprimirTablero(tablero, palabra, filaInicio, colInicio, 3); // 3 va a significar arriba
	        return 1; // Palabra encontrada hacia arriba
	    }
	}
	encontrado = 1;

	// Buscar hacia abajo
	if (filaInicio + largoPalabra <= filas) {
	    for (int k = 0; k < largoPalabra && encontrado; k++) {
	        if (tablero[(filaInicio + k) * columnas + colInicio].letra != palabra[k]) {
	            encontrado = 0;
	        }
	    }
	    if (encontrado) {
	        ImprimirTablero(tablero, palabra, filaInicio, colInicio, 4); // 4 va a significar abajo
	        return 1; // Palabra encontrada hacia abajo
	    }
	}
	encontrado = 1;

	//Buscar derecha arriba
	if (filaInicio - largoPalabra >= -1 && colInicio + largoPalabra <= columnas) {
	    for (int k = 0; k < largoPalabra && encontrado; k++) {
	        if (tablero[(filaInicio - k) * columnas + colInicio + k].letra != palabra[k]) {
	            encontrado = 0;
	        }
	    }
	    if (encontrado) {
	        ImprimirTablero(tablero, palabra, filaInicio, colInicio, 5); // 5 va a significar derecha arriba
	        return 1; // Palabra encontrada derecha arriba
	    }
	}
	encontrado = 1;

	// Buscar derecha abajo
	if (filaInicio + largoPalabra <= filas && colInicio + largoPalabra <= columnas) {
	    for (int k = 0; k < largoPalabra && encontrado; k++) {
	        if (tablero[(filaInicio + k) * columnas + colInicio + k].letra != palabra[k]) {
	            encontrado = 0;
	        }
	    }
	    if (encontrado) {
	        ImprimirTablero(tablero, palabra, filaInicio, colInicio, 6); // 6 va a significar derecha abajo
	        return 1; // Palabra encontrada derecha abajo
	    }
	}
	encontrado = 1;

	//Buscar izquierda arriba
	if (filaInicio - largoPalabra >= -1 && colInicio - largoPalabra >= -1) {
	    for (int k = 0; k < largoPalabra && encontrado; k++) {
	        if (tablero[(filaInicio - k) * columnas + colInicio - k].letra != palabra[k]) {
	            encontrado = 0;
	        }
	    }
	    if (encontrado) {
	    	ImprimirTablero(tablero, palabra, filaInicio, colInicio, 7); // 7 va a significar izquierda arriba
	        return 1; // Palabra encontrada izquierda arriba
	    }
	} 
	encontrado = 1;

	// Buscar izquierda abajo
	if (filaInicio + largoPalabra <= filas && colInicio - largoPalabra >= -1) {
	    for (int k = 0; k < largoPalabra && encontrado; k++) {
	        if (tablero[(filaInicio + k) * columnas + colInicio - k].letra != palabra[k]) {
	            encontrado = 0;
	        }
	    }
	    if (encontrado) {
	    	ImprimirTablero(tablero, palabra, filaInicio, colInicio, 8); // 8 va a significar izquierda abajo
	        return 1; // Palabra encontrada izquierda abajo
	    }
	}

    return 0; // Palabra no encontrada en ninguna dirección desde esta celda
}

/*Entrada: la palabra encontrada, su fila y columna de la primera letra.
Descripcion: Se imprimen las coordenadas de la palabra encontrada con
la palabra encontrada
*/
void ImprimirList(const char* palabra, int fila, int columna){
	fprintf(archivoListaOut, "%i %i (%s)\n", fila+1, columna+1, palabra);
}

/*Entrada: tablero, palabra a buscar
Descripcion: se hace una iteracion en el tablero para buscar la coincidencia
de la primera letra de la palabra a buscar con el nodo del tablero, leugo se
llama a la funcion buscar desde Celda, si esta resulta ser 1 se llama a la funcion
ImprimirList y se retorna 1, en caso de que no este la palabra en el tablero se
retorna 0
Salidas: 1 si se encuentra la palabra, 0 si no.
*/
int BuscarUnaPalabraEnTablero(nodografo* tablero, const char* palabra) {
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            if (tablero[i * columnas + j].letra == palabra[0]) { // Si la primera letra coincide
                if (buscarDesdeCelda(tablero, i, j, palabra)) {
                	ImprimirList(palabra, i, j);
                    return 1; // Palabra encontrada
                }
            }
        }
    }
    return 0; // Palabra no encontrada
}

/*Entrada: El tablero y la tabla hash
Descripcion: se hace un bucle para abarcar todos los valores que contiene
la tabla hash y se analiza para saber si esa palabra esta en el tablero. En
caso de que este se añade 1 al contador de palabras encontradas.
*/
void BuscarPalabras(nodografo* tablero, nodoh** tablaHash){
	for (int i = 0; i < SIZE; i++) {
        nodoh* actual = tablaHash[i];
        while (actual != NULL) {
            if (BuscarUnaPalabraEnTablero(tablero, actual->palabra)) {
            	palabrasEncontradas = palabrasEncontradas +1; 
            }
            actual = actual->siguiente;
        }
    }
    fprintf(archivoListaOut, "%i", palabrasEncontradas);
    printf(" -Palabras Encontradas: %i.\n", palabrasEncontradas);
}
#endif 
