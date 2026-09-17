#ifndef TABLERO_H
#define TABLERO_H
#include "fichas.h"

// Reserva memoria dinámica para un tablero de 'filas' x 'columnas' y lo
// deja SIN inicializar (todo en 0). Usa bytes_necesarios() de fichas.h,
// así que respeta el tamaño mínimo exacto, sin sobredimensionar.
unsigned char* crear_tablero(int filas, int columnas);

// Llena cada posición del tablero con una ficha aleatoria (0 a 5),
// nunca con ESTADO_VACIO ni ESTADO_ESPECIAL. Escribe directo con
// escribir_ficha, nunca pasa por una estructura auxiliar.
void generar_aleatorio(unsigned char* tablero, int filas, int columnas);

// Traduce el código de 3 bits de una ficha a un carácter visible.
char ficha_a_caracter(unsigned char valor);

// Primera vista: la trama física real, byte por byte, en binario.
void imprimir_bytes(const unsigned char* tablero, int filas, int columnas);

// Segunda vista: la matriz lógica interpretada, un carácter por ficha.
void imprimir_visual(const unsigned char* tablero, int filas, int columnas);

// Junta las dos vistas en el orden que pide el enunciado (bytes primero).
void mostrar_tablero(const unsigned char* tablero, int filas, int columnas);

// Elimina la ficha en (fila,columna): la deja en ESTADO_VACIO.
// Devuelve false si la coordenada está fuera de rango (y no hace nada).
bool eliminar_ficha(unsigned char* tablero, int filas, int columnas, int fila, int columna);

#endif