#ifndef ESTRUCTURA_H
#define ESTRUCTURA_H

#include "tablero.h"

// Inserta una fila nueva (llena de fichas aleatorias) en 'posicionFila'.
// posicionFila puede ir de 0 a 'filas' inclusive (filas == insertar al
// final). SIEMPRE reconstruye el tablero con memoria nueva (insertar
// nunca puede reusar el bloque viejo, porque necesita mas espacio).
// Actualiza tablero, filas y capacidadBytes por referencia.
// Devuelve false si posicionFila es invalida (no cambia nada en ese caso).
bool insertar_fila(unsigned char*& tablero, int& filas, int& columnas,int& capacidadBytes, int posicionFila);

// Elimina la fila en 'posicionFila' (0 a filas-1). Solo reasigna memoria
// fisica (delete[] + new de un bloque mas chico) cuando la ocupacion
// resultante cae por debajo del 65% de 'capacidadBytes'; si no, reusa el
// mismo bloque (mas grande de lo necesario, como permite el documento).
// Devuelve false si la posicion es invalida o si el tablero solo tiene
// 1 fila (no se permite dejarlo en 0 filas).
bool eliminar_fila(unsigned char*& tablero, int& filas, int& columnas,int& capacidadBytes, int posicionFila);

// Igual que insertar_fila, pero para columnas. Nota: insertar/eliminar
// una columna es mas costoso que una fila, porque cambia 'columnas' y
// eso mueve la posicion de TODAS las fichas del tablero (la formula
// indice = fila*columnas+columna depende de columnas).
bool insertar_columna(unsigned char*& tablero, int& filas, int& columnas,int& capacidadBytes, int posicionColumna);

bool eliminar_columna(unsigned char*& tablero, int& filas, int& columnas,int& capacidadBytes, int posicionColumna);

#endif