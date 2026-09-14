#include <iostream>
#include "funciones.h"

// Imprime un byte en binario, MSB primero (para comparar visualmente
// con las figuras del documento).
void imprimir_byte(unsigned char b) {
    for (int bit = 7; bit >= 0; bit--) {
        std::cout << (((b >> bit) & 1) ? '1' : '0');
    }
}

void imprimir_tablero_bytes(const unsigned char* tablero, int totalBytes) {
    for (int i = 0; i < totalBytes; i++) {
        imprimir_byte(tablero[i]);
        std::cout << " ";
    }
    std::cout << std::endl;
}

int main() {
    // Reproduce EXACTAMENTE el ejemplo de la Figura 2 del documento:
    // fichas lógicas: A B C D E F A B  (1 fila, 8 columnas)
    // códigos:        0 1 2 3 4 5 0 1
    // resultado esperado en bytes: 00000101 00111001 01000001
    int filas = 1, columnas = 8;
    int totalFichas = filas * columnas;
    int totalBytes = bytes_necesarios(totalFichas);

    unsigned char* tablero = new unsigned char[totalBytes];
    for (int i = 0; i < totalBytes; i++) tablero[i] = 0;

    unsigned char secuencia[8] = {
        FICHA_A, FICHA_B, FICHA_C, FICHA_D, FICHA_E, FICHA_F, FICHA_A, FICHA_B
    };

    for (int c = 0; c < columnas; c++) {
        escribir_ficha(tablero, 0, c, columnas, secuencia[c]);
    }

    std::cout << "Bytes obtenidos:  ";
    imprimir_tablero_bytes(tablero, totalBytes);
    std::cout << "Bytes esperados:  00000101 00111001 01000001" << std::endl;

    // Verificación automática byte a byte.
    unsigned char esperado[3] = {0x05, 0x39, 0x41};
    bool ok = true;
    for (int i = 0; i < totalBytes; i++) {
        if (tablero[i] != esperado[i]) {
            ok = false;
            std::cout << "  -> DIFERENCIA en byte " << i << std::endl;
        }
    }

    // Verificación de lectura: cada posición debe devolver el valor escrito.
    for (int c = 0; c < columnas; c++) {
        unsigned char leido = leer_ficha(tablero, 0, c, columnas);
        if (leido != secuencia[c]) {
            ok = false;
            std::cout << "  -> DIFERENCIA leyendo columna " << c << std::endl;
        }
    }

    // Prueba de escritura puntual: cambiar la ficha en (0,5) y confirmar
    // que las demás posiciones no se corrompen (caso típico de ficha que
    // cruza dos bytes, ver Figura 3).
    escribir_ficha(tablero, 0, 5, columnas, ESTADO_VACIO);
    if (leer_ficha(tablero, 0, 5, columnas) != ESTADO_VACIO) ok = false;
    if (leer_ficha(tablero, 0, 4, columnas) != FICHA_E) ok = false; // vecina intacta
    if (leer_ficha(tablero, 0, 6, columnas) != FICHA_A) ok = false; // vecina intacta

    std::cout << (ok ? "OK: todas las pruebas pasaron."
                     : "FALLO: revisar el modulo de bits.") << std::endl;

    delete[] tablero;
    return ok ? 0 : 1;
}