#include "tablero.h"
#include <iostream>
#include <cstdlib>

unsigned char* crear_tablero(int filas, int columnas) {
    int totalFichas = filas * columnas;
    int totalBytes = bytes_necesarios(totalFichas);
    unsigned char* tablero = new unsigned char[totalBytes];
    for (int i = 0; i < totalBytes; i++) {
        tablero[i] = 0;
    }
    return tablero;
}

void generar_aleatorio(unsigned char* tablero, int filas, int columnas) {
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            unsigned char valor = (unsigned char) (rand() % 6); // 0..5, distribución uniforme
            escribir_ficha(tablero, f, c, columnas, valor);
        }
    }
}

char ficha_a_caracter(unsigned char valor) {
    switch (valor) {
    case FICHA_A: return '@';
    case FICHA_B: return '#';
    case FICHA_C: return '$';
    case FICHA_D: return '%';
    case FICHA_E: return '&';
    case FICHA_F: return '*';
    case ESTADO_VACIO: return '.';
    default: return '?'; // ESTADO_ESPECIAL u otro valor no esperado
    }
}

// Imprime una linea separadora tipo +---+---+---+ para 'columnas' celdas,
// cada una de 'anchoCelda' caracteres de ancho.
void imprimir_linea_separadora(int columnas, int anchoCelda) {
    for (int c = 0; c < columnas; c++) {
        std::cout << "+";
        for (int i = 0; i < anchoCelda; i++) {
            std::cout << "-";
        }
    }
    std::cout << "+" << std::endl;
}

void imprimir_bytes(const unsigned char* tablero, int filas, int columnas) {
    int totalBytes = bytes_necesarios(filas * columnas);
    std::cout << "--- Trama fisica (bytes) ---" << std::endl;
    for (int i = 0; i < totalBytes; i++) {
        unsigned char b = tablero[i];
        for (int bit = 7; bit >= 0; bit--) {
            std::cout << (((b >> bit) & 1) ? '1' : '0');
        }
        std::cout << " ";
    }
    std::cout << std::endl;
}

void imprimir_bits_por_posicion(const unsigned char* tablero, int filas, int columnas) {
    std::cout << "--- Tablero en bits (por posicion) ---" << std::endl;
    imprimir_linea_separadora(columnas, 5); // celda de 5: " 010 "
    for (int f = 0; f < filas; f++) {
        std::cout << "|";
        for (int c = 0; c < columnas; c++) {
            unsigned char valor = leer_ficha(tablero, f, c, columnas);
            std::cout << " ";
            for (int bit = 2; bit >= 0; bit--) {
                std::cout << (((valor >> bit) & 1) ? '1' : '0');
            }
            std::cout << " |";
        }
        std::cout << std::endl;
        imprimir_linea_separadora(columnas, 5);
    }
}

void imprimir_visual(const unsigned char* tablero, int filas, int columnas) {
    std::cout << "--- Tablero visual ---" << std::endl;
    imprimir_linea_separadora(columnas, 3); // celda de 3: " $ "
    for (int f = 0; f < filas; f++) {
        std::cout << "|";
        for (int c = 0; c < columnas; c++) {
            unsigned char valor = leer_ficha(tablero, f, c, columnas);
            std::cout << " " << ficha_a_caracter(valor) << " |";
        }
        std::cout << std::endl;
        imprimir_linea_separadora(columnas, 3);
    }
}

void mostrar_tablero(const unsigned char* tablero, int filas, int columnas) {
    imprimir_bytes(tablero, filas, columnas);
    imprimir_bits_por_posicion(tablero, filas, columnas);
    imprimir_visual(tablero, filas, columnas);
}

bool eliminar_ficha(unsigned char* tablero, int filas, int columnas, int fila, int columna) {
    if (fila < 0 || fila >= filas || columna < 0 || columna >= columnas) {
        return false;
    }
    escribir_ficha(tablero, fila, columna, columnas, ESTADO_VACIO);
    return true;
}