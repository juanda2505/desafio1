#ifndef FICHAS_H
#define FICHAS_H

// ---------------------------------------------------------------------------
// Códigos de 3 bits para cada ficha (Figura 1 del documento).
// La asignación concreta es libre; esta es una posible.
// ---------------------------------------------------------------------------
const unsigned char FICHA_A       = 0; // 000
const unsigned char FICHA_B       = 1; // 001
const unsigned char FICHA_C       = 2; // 010
const unsigned char FICHA_D       = 3; // 011
const unsigned char FICHA_E       = 4; // 100
const unsigned char FICHA_F       = 5; // 101
const unsigned char ESTADO_VACIO  = 6; // 110 - posición vacía tras eliminar
const unsigned char ESTADO_ESPECIAL = 7; // 111 - reservado para uso futuro

const int BITS_POR_FICHA = 3;

// ---------------------------------------------------------------------------
// Aritmética de localización (Figura 4 del documento).
// ---------------------------------------------------------------------------

// índice = fila * columnas + columna
int calcular_indice(int fila, int columna, int columnas);

// bit_inicial = índice * 3
int calcular_bit_inicial(int indice);

// Bytes mínimos necesarios para almacenar 'totalFichas' fichas de 3 bits.
int bytes_necesarios(int totalFichas);

// ---------------------------------------------------------------------------
// Acceso a nivel de bits. 'tablero' es la tira continua de bytes en memoria
// dinámica. Estas funciones son las ÚNICAS que deben tocar directamente los
// bits del tablero; todo lo demás debe llamarlas.
// ---------------------------------------------------------------------------

// Extrae el valor de 3 bits (0-7) de la posición (fila,columna).
unsigned char leer_ficha(const unsigned char* tablero, int fila, int columna, int columnas);

// Escribe 'valor' (solo se usan sus 3 bits bajos) en (fila,columna).
void escribir_ficha(unsigned char* tablero, int fila, int columna, int columnas, unsigned char valor);

// Versiones por índice plano, útiles al reorganizar el tablero linealmente.
unsigned char leer_ficha_por_indice(const unsigned char* tablero, int indice);
void escribir_ficha_por_indice(unsigned char* tablero, int indice, unsigned char valor);

#endif