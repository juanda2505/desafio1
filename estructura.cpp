#include "estructura.h"
#include <cstdlib>

bool insertar_fila(unsigned char*& tablero, int& filas, int& columnas,
                   int& capacidadBytes, int posicionFila) {
    if (posicionFila < 0 || posicionFila > filas) {
        return false; // posicion invalida: no toca nada
    }

    int nuevasFilas = filas + 1;
    int nuevosBytesNecesarios = bytes_necesarios(nuevasFilas * columnas);

    unsigned char* nuevoTablero = new unsigned char[nuevosBytesNecesarios];
    for (int i = 0; i < nuevosBytesNecesarios; i++) {
        nuevoTablero[i] = 0;
    }

    // Reconstruimos fila por fila con las dimensiones NUEVAS, leyendo del
    // tablero VIEJO (que todavia usa las dimensiones viejas) cuando
    // corresponde a una fila que ya existia.
    for (int f = 0; f < nuevasFilas; f++) {
        for (int c = 0; c < columnas; c++) {
            unsigned char valor;
            if (f < posicionFila) {
                valor = leer_ficha(tablero, f, c, columnas); // misma fila de antes
            } else if (f == posicionFila) {
                valor = (unsigned char) (rand() % 6); // fila nueva, ficha aleatoria
            } else {
                valor = leer_ficha(tablero, f - 1, c, columnas); // se corrio una fila
            }
            escribir_ficha(nuevoTablero, f, c, columnas, valor);
        }
    }

    delete[] tablero;
    tablero = nuevoTablero;
    filas = nuevasFilas;
    capacidadBytes = nuevosBytesNecesarios; // insertar siempre reasigna
    return true;
}

bool eliminar_fila(unsigned char*& tablero, int& filas, int& columnas,
                   int& capacidadBytes, int posicionFila) {
    if (posicionFila < 0 || posicionFila >= filas) {
        return false;
    }
    if (filas <= 1) {
        return false; // no se permite dejar el tablero sin filas
    }

    int nuevasFilas = filas - 1;
    int nuevosBytesNecesarios = bytes_necesarios(nuevasFilas * columnas);

    // Construimos el resultado en un bloque temporal (no podemos escribir
    // "encima" del tablero viejo mientras seguimos leyendo de el).
    unsigned char* temporal = new unsigned char[nuevosBytesNecesarios];
    for (int i = 0; i < nuevosBytesNecesarios; i++) {
        temporal[i] = 0;
    }

    for (int f = 0; f < nuevasFilas; f++) {
        int fOrigen = (f < posicionFila) ? f : f + 1; // saltar la fila eliminada
        for (int c = 0; c < columnas; c++) {
            unsigned char valor = leer_ficha(tablero, fOrigen, c, columnas);
            escribir_ficha(temporal, f, c, columnas, valor);
        }
    }

    // Regla del 65%: solo reducimos la memoria FISICA si la ocupacion
    // resultante cae por debajo del 65% de lo que ya teniamos reservado.
    double ocupacion = (double) nuevosBytesNecesarios / (double) capacidadBytes;

    if (ocupacion < 0.65) {
        delete[] tablero;
        tablero = temporal;
        capacidadBytes = nuevosBytesNecesarios;
    } else {
        // Seguimos usando el mismo bloque (mas grande de lo necesario):
        // copiamos los bytes validos al inicio y liberamos el temporal.
        for (int i = 0; i < nuevosBytesNecesarios; i++) {
            tablero[i] = temporal[i];
        }
        delete[] temporal;
        // capacidadBytes no cambia
    }

    filas = nuevasFilas;
    return true;
}

bool insertar_columna(unsigned char*& tablero, int& filas, int& columnas,
                      int& capacidadBytes, int posicionColumna) {
    if (posicionColumna < 0 || posicionColumna > columnas) {
        return false;
    }

    int columnasViejas = columnas;
    int nuevasColumnas = columnas + 1;
    int nuevosBytesNecesarios = bytes_necesarios(filas * nuevasColumnas);

    unsigned char* nuevoTablero = new unsigned char[nuevosBytesNecesarios];
    for (int i = 0; i < nuevosBytesNecesarios; i++) {
        nuevoTablero[i] = 0;
    }

    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < nuevasColumnas; c++) {
            unsigned char valor;
            if (c < posicionColumna) {
                valor = leer_ficha(tablero, f, c, columnasViejas);
            } else if (c == posicionColumna) {
                valor = (unsigned char) (rand() % 6);
            } else {
                valor = leer_ficha(tablero, f, c - 1, columnasViejas);
            }
            escribir_ficha(nuevoTablero, f, c, nuevasColumnas, valor);
        }
    }

    delete[] tablero;
    tablero = nuevoTablero;
    columnas = nuevasColumnas;
    capacidadBytes = nuevosBytesNecesarios;
    return true;
}

bool eliminar_columna(unsigned char*& tablero, int& filas, int& columnas,
                      int& capacidadBytes, int posicionColumna) {
    if (posicionColumna < 0 || posicionColumna >= columnas) {
        return false;
    }
    if (columnas <= 1) {
        return false; // no se permite dejar el tablero sin columnas
    }

    int columnasViejas = columnas;
    int nuevasColumnas = columnas - 1;
    int nuevosBytesNecesarios = bytes_necesarios(filas * nuevasColumnas);

    unsigned char* temporal = new unsigned char[nuevosBytesNecesarios];
    for (int i = 0; i < nuevosBytesNecesarios; i++) {
        temporal[i] = 0;
    }

    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < nuevasColumnas; c++) {
            int cOrigen = (c < posicionColumna) ? c : c + 1;
            unsigned char valor = leer_ficha(tablero, f, cOrigen, columnasViejas);
            escribir_ficha(temporal, f, c, nuevasColumnas, valor);
        }
    }

    double ocupacion = (double) nuevosBytesNecesarios / (double) capacidadBytes;

    if (ocupacion < 0.65) {
        delete[] tablero;
        tablero = temporal;
        capacidadBytes = nuevosBytesNecesarios;
    } else {
        for (int i = 0; i < nuevosBytesNecesarios; i++) {
            tablero[i] = temporal[i];
        }
        delete[] temporal;
    }

    columnas = nuevasColumnas;
    return true;
}