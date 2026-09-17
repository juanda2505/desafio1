#include "fichas.h"
//funcion traduce una posicion 2D (fila, columna) a una posicion 1D (un solo numero)
int calcular_indice(int fila, int columna, int columnas)
{
    return fila * columnas + columna;
}
//funcion que dice en que bit exacto de toda la tira empieza esa ficha
int calcular_bit_inicial(int indice)
{
    return indice * bits_por_ficha;
}

int bytes_necesarios(int totalFichas)
{
    int totalBits = totalFichas * bits_por_ficha;
    // Redondeo hacia arriba a bytes completos: (bits + 7) / 8
    return (totalBits + 7) / 8;
}

// Convención de empaquetado (según la Figura 2 del documento): la PRIMERA
// ficha ocupa los bits MÁS significativos del primer byte, y la trama
// continua "cae" hacia los bits menos significativos, cruzando a los
// siguientes bytes en el mismo sentido. Por eso 'bitOffset' se cuenta
// desde el MSB (bit 7) del byte, no desde el LSB.
//
// Para no tratar por separado el caso "cabe en un byte" y el caso "cruza
// dos bytes", combinamos byte actual + byte siguiente en un entero de 16
// bits (byte actual en la mitad alta) y trabajamos ahí; el byte siguiente
// solo se toca cuando la ficha realmente lo necesita.

unsigned char leer_ficha_por_indice(const unsigned char* tablero, int indice)
{
    int bitInicial   = calcular_bit_inicial(indice);
    int byteIndex    = bitInicial / 8;
    int bitOffset    = bitInicial % 8;      // bits ya consumidos en este byte, contados desde el MSB
    bool cruzaByte   = (bitOffset + bits_por_ficha) > 8;

    unsigned int combinado = ((unsigned int) tablero[byteIndex]) << 8;
    if (cruzaByte)
    {
        combinado = combinado | (unsigned int) tablero[byteIndex + 1];
    }

    // Los 16 bits de 'combinado' van de la posición 15 (MSB del byte actual)
    // a la 0 (LSB del byte siguiente). La ficha empieza en la posición
    // (15 - bitOffset) y ocupa 3 bits, así que para alinearla al extremo
    // derecho desplazamos (15 - bitOffset - 2) = (13 - bitOffset) lugares.
    int desplazamiento = 13 - bitOffset;
    return (unsigned char) ((combinado >> desplazamiento) & 0x07);
}

void escribir_ficha_por_indice(unsigned char* tablero, int indice, unsigned char valor)
{
    valor = valor & 0x07; // nos aseguramos de usar solo los 3 bits bajos

    int bitInicial = calcular_bit_inicial(indice);
    int byteIndex  = bitInicial / 8;
    int bitOffset  = bitInicial % 8;
    bool cruzaByte = (bitOffset + bits_por_ficha) > 8;

    unsigned int combinado = ((unsigned int) tablero[byteIndex]) << 8;
    if (cruzaByte)
    {
        combinado = combinado | (unsigned int) tablero[byteIndex + 1];
    }

    int desplazamiento = 13 - bitOffset;
    unsigned int mascara         = ((unsigned int) 0x07) << desplazamiento;
    unsigned int valorDesplazado = ((unsigned int) valor) << desplazamiento;

    combinado = (combinado & ~mascara) | valorDesplazado;

    tablero[byteIndex] = (unsigned char) ((combinado >> 8) & 0xFF);
    if (cruzaByte)
    {
        tablero[byteIndex + 1] = (unsigned char) (combinado & 0xFF);
    }
}

unsigned char leer_ficha(const unsigned char* tablero, int fila, int columna, int columnas)
{
    int indice = calcular_indice(fila, columna, columnas);
    return leer_ficha_por_indice(tablero, indice);
}

void escribir_ficha(unsigned char* tablero, int fila, int columna, int columnas, unsigned char valor)
{
    int indice = calcular_indice(fila, columna, columnas);
    escribir_ficha_por_indice(tablero, indice, valor);
}