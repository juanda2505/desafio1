#ifndef TABLERO_H
#define TABLERO_H
#include "fichas.h"

// reserva memoria dinamica para un tablero de filas x columnas y lo
// deja sin inicializar (todo en 0). Usa bytes_necesarios() de fichas.h,
unsigned char* crear_tablero(int filas, int columnas);

// llena cada posicion del tablero con una ficha aleatoria (0 a 5)
void generar_aleatorio(unsigned char* tablero, int filas, int columnas);

// traduce el codigo de 3 bits de una ficha a un carácter visible
char ficha_a_caracter(unsigned char valor);

// Primera vista: la trama fisica real, byte por byte, en binario.
void imprimir_bytes(const unsigned char* tablero, int filas, int columnas);

// Segunda vista: la matriz logica interpretada, un caracter por ficha
void imprimir_visual(const unsigned char* tablero, int filas, int columnas);

// junta las dos vistas en el orden que pide el enunciado
void mostrar_tablero(const unsigned char* tablero, int filas, int columnas);

// elimina la ficha en (fila,columna): la deja en ESTADO_VACIO
// Devuelve false si la coordenada está fuera de rango (y no hace nada).
bool eliminar_ficha(unsigned char* tablero, int filas, int columnas, int fila, int columna);

#endif