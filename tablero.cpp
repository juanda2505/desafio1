#include "tablero.h"
#include <iostream>
#include <cstdlib>
//reserva la memoria dinamica exacta que necesita el tablero, y dejarla inicializada en cero
unsigned char* crear_tablero(int filas, int columnas)
{
    int totalFichas = filas * columnas;
    int totalBytes = bytes_necesarios(totalFichas); //funcion de fichas.cpp, cuantos bytes se requiren
    unsigned char* tablero = new unsigned char[totalBytes];
    for (int i = 0; i < totalBytes; i++)
    {
        tablero[i] = 0;
    }
    return tablero;
}
//llenar cada posición del tablero recien creado con una ficha al azar
void generar_aleatorio(unsigned char* tablero, int filas, int columnas)
{
    for (int f = 0; f < filas; f++)
    {
        for (int c = 0; c < columnas; c++)
        {
            unsigned char valor = (unsigned char) (rand() % 6); // 0..5, distribución uniforme
            escribir_ficha(tablero, f, c, columnas, valor);
        }
    }
}
//traducir el código numérico (0-7) de una ficha al símbolo que se muestra en pantalla
char ficha_a_caracter(unsigned char valor)
{
    switch (valor) //compara el valor recibido contra cada constante
    {
    case FICHA_A: return '@';
    case FICHA_B: return '#';
    case FICHA_C: return '$';
    case FICHA_D: return '%';
    case FICHA_E: return '&';
    case FICHA_F: return '*';
    case ESTADO_VACIO: return '.';
    default: return '?'; // estado especial u otro valor no esperado
    }
}

// Imprime una linea separadora tipo +---+---+---+ para 'columnas' celdas,
// cada una de 'anchoCelda' caracteres de ancho.
void imprimir_linea_separadora(int columnas, int anchoCelda)
{
    for (int c = 0; c < columnas; c++)
    {
        std::cout << "+";
        for (int i = 0; i < anchoCelda; i++)
        {
            std::cout << "-";
        }
    }
    std::cout << "+" << std::endl;
}
//mostrar la trama fisica real, los bytes tal como estan en memoria, sin traducir nada
void imprimir_bytes(const unsigned char* tablero, int filas, int columnas)
{
    int totalBytes = bytes_necesarios(filas * columnas);
    std::cout << "   Trama fisica (bytes)   " << std::endl;
    for (int i = 0; i < totalBytes; i++) //recorre byte por byte
    {
        unsigned char b = tablero[i];
        for (int bit = 7; bit >= 0; bit--) //recorrido de bits para cada byte
        {
            std::cout << (((b >> bit) & 1) ? '1' : '0');// & es para aislar bits
        }
        std::cout << " ";
    }
    std::cout << std::endl;
}
//misma información que imprimir_bytes, pero organizada en la cuadricula del tablero
void imprimir_bits_por_posicion(const unsigned char* tablero, int filas, int columnas)
{
    std::cout << "   Tablero en bits (por posicion)   " << std::endl;
    imprimir_linea_separadora(columnas, 5); // celda de 5: " 010 "
    for (int f = 0; f < filas; f++)
    {
        std::cout << "|";
        for (int c = 0; c < columnas; c++)
        {
            unsigned char valor = leer_ficha(tablero, f, c, columnas);
            std::cout << " ";
            for (int bit = 2; bit >= 0; bit--)
            {
                std::cout << (((valor >> bit) & 1) ? '1' : '0');
            }
            std::cout << " |";
        }
        std::cout << std::endl;
        imprimir_linea_separadora(columnas, 5);
    }
}
// simnolos en vez de numeros
void imprimir_visual(const unsigned char* tablero, int filas, int columnas)
{
    std::cout << "--- Tablero visual ---" << std::endl;
    imprimir_linea_separadora(columnas, 3); // celda de 3: " $ "
    for (int f = 0; f < filas; f++)
    {
        std::cout << "|";
        for (int c = 0; c < columnas; c++)
        {
            unsigned char valor = leer_ficha(tablero, f, c, columnas);
            std::cout << " " << ficha_a_caracter(valor) << " |";
        }
        std::cout << std::endl;
        imprimir_linea_separadora(columnas, 3);
    }
}
//agrupar las tres vistas en el orden correcto, para no tener que llamar a las tres funciones por separado cada vez
void mostrar_tablero(const unsigned char* tablero, int filas, int columnas)
{
    imprimir_bytes(tablero, filas, columnas);
    imprimir_bits_por_posicion(tablero, filas, columnas);
    imprimir_visual(tablero, filas, columnas);
}
//elimina una ficha de la posición indicada si esta dentro del tablero
bool eliminar_ficha(unsigned char* tablero, int filas, int columnas, int fila, int columna)
{
    if (fila < 0 || fila >= filas || columna < 0 || columna >= columnas)
    {
        return false;
    }
    escribir_ficha(tablero, fila, columna, columnas, ESTADO_VACIO);
    return true;
}