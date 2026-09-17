#ifndef COMBINACIONES_H
#define COMBINACIONES_H

#include "tablero.h"

// Recorre filas y columnas buscando 3 o mas fichas iguales consecutivas.
// Marca con 1 cada posicion que debe eliminarse en 'marcado' (arreglo del
// caller, tamaño filas*columnas, se reinicia a 0 dentro de esta funcion).
// Una ficha en un cruce horizontal Y vertical queda marcada una sola vez,
// pero cada combinacion (horizontal o vertical) cuenta por separado.
// Devuelve cuantas combinaciones (grupos) se encontraron; 0 si no hay.
int detectar_combinaciones(const unsigned char* tablero, int filas, int columnas, unsigned char* marcado);

// Recorre 'marcado' y pone ESTADO_VACIO en el tablero donde corresponda.
// Devuelve cuantas fichas se eliminaron.
int eliminar_marcadas(unsigned char* tablero, const unsigned char* marcado, int filas, int columnas);

// Para cada columna, compacta las fichas no vacias hacia abajo (algoritmo
// de dos punteros), dejando los huecos agrupados arriba.
void aplicar_gravedad(unsigned char* tablero, int filas, int columnas);

// Llena con fichas aleatorias (0 a 5) todas las posiciones que sigan en
// ESTADO_VACIO (normalmente las de arriba, despues de la gravedad).
void rellenar_vacios(unsigned char* tablero, int filas, int columnas);

// Ciclo completo: mientras detectar_combinaciones encuentre algo, elimina,
// aplica gravedad y rellena, y vuelve a escanear. 'numCascadas' recibe
// cuantas vueltas dio el ciclo, 'numCombinaciones' cuantos grupos en total
// se encontraron en toda la secuencia. Devuelve el total de fichas
// eliminadas por las cascadas (sin contar la eliminacion manual inicial).
int procesar_cascadas(unsigned char* tablero, int filas, int columnas, int* numCascadas, int* numCombinaciones);

#endif