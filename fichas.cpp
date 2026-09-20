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
//calcular cuanta memoria o cuantos bytes hay que reservar con new para guardar todas las fichas
int bytes_necesarios(int totalFichas)
{
    int totalBits = totalFichas * bits_por_ficha;
    // Redondeo hacia arriba a bytes completos: (bits + 7) / 8
    return (totalBits + 7) / 8;
}
// Estas funciones son las ÚNICAS que deben tocar directamente los
// bits del tablero; todo lo demas debe llamarlas.

//dado el indice de una ficha, extrae su valor(0 a 7) leyendo directamente
//los bits de memoria sin importar si esa ficha vive completa en un byte o esta entre dos
unsigned char leer_ficha_por_indice(const unsigned char* tablero, int indice)
{
    int bitInicial   = calcular_bit_inicial(indice);
    int byteIndex    = bitInicial / 8;       //desde que byte empieza la ficha
    int bitOffset    = bitInicial % 8;      // dentro del byte de byteIndex, cuantos bits hay usados antes de que empiece la ficha
    bool cruzaByte   = (bitOffset + bits_por_ficha) > 8; //la ficha cruza el byte TRUE or FALSE

    unsigned int combinado = ((unsigned int) tablero[byteIndex]) << 8;
    if (cruzaByte)
    {
        combinado = combinado | (unsigned int) tablero[byteIndex + 1];
    }

    // Los 16 bits de 'combinado' van de la posición 15 a la 0
    // La ficha empieza en la posición (15 - bitOffset) y ocupa 3 bits, así que para alinearla al extremo
    // derecho desplazamos (15 - bitOffset - 2) = (13 - bitOffset) lugares.
    int desplazamiento = 13 - bitOffset;
    return (unsigned char) ((combinado >> desplazamiento) & 0x07);
}
//escribir un valor nuevo de 3 bits en una posición, sin dañar los bits vecinos que no le pertenecen(limpiar y luego insertar)
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
    unsigned int mascara         = ((unsigned int) 0x07) << desplazamiento;//se ponen unos en las 3 posiciones donde vive la ficha
    unsigned int valorDesplazado = ((unsigned int) valor) << desplazamiento;

    combinado = (combinado & ~mascara) | valorDesplazado; //se invierte la mascara y con & se apagan justo esas 3 posiciones
                                                          //"valordesplazado" mueve la nueva ficha esasmismas 3 posiciones
    tablero[byteIndex] = (unsigned char) ((combinado >> 8) & 0xFF);//se vuelven a separar los bytes
    if (cruzaByte)
    {
        tablero[byteIndex + 1] = (unsigned char) (combinado & 0xFF);
    }
}
//traductores de coordenadas
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