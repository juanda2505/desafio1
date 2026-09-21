#ifndef FICHAS_H
#define FICHAS_H

// Codigos de 3 bits para cada ficha
const unsigned char FICHA_A       = 0; // 000
const unsigned char FICHA_B       = 1; // 001
const unsigned char FICHA_C       = 2; // 010
const unsigned char FICHA_D       = 3; // 011
const unsigned char FICHA_E       = 4; // 100
const unsigned char FICHA_F       = 5; // 101
const unsigned char ESTADO_VACIO  = 6; // 110 - posicion vacia tras eliminar
const unsigned char ESTADO_ESPECIAL = 7; // 111 - reservado para uso futuro

const int bits_por_ficha = 3;

// aritmetica de localizacion
// indice = fila * columnas + columna
int calcular_indice(int fila, int columna, int columnas);

// bit_inicial = indice * 3
int calcular_bit_inicial(int indice);

// bytes minimos necesarios para almacenar "totalFichas" fichas de 3 bits
int bytes_necesarios(int totalFichas);

// acceso a nivel de bits. "tablero" es la tira continua de bytes en memoria dinamica
//Estas funciones son las unicas que deben tocar directamente los bits del tablero

// extrae el valor de 3 bits (0-7) de la posición (fila,columna)
unsigned char leer_ficha(const unsigned char* tablero, int fila, int columna, int columnas);

// Escribe "valor" (solo se usan sus 3 bits bajos) en (fila,columna)
void escribir_ficha(unsigned char* tablero, int fila, int columna, int columnas, unsigned char valor);

// Versiones por indice plano, utiles al reorganizar el tablero linealmente
unsigned char leer_ficha_por_indice(const unsigned char* tablero, int indice);
void escribir_ficha_por_indice(unsigned char* tablero, int indice, unsigned char valor);

#endif