#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include "combinaciones.h"
#include "estructura.h"

bool leer_entero(int &valor) {
    std::cin >> valor;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    return true;
}

// Corre el ciclo de cascadas y acumula las metricas de la partida.
void procesar_y_reportar(unsigned char* tablero, int filas, int columnas,
                         int &totalFichasEliminadas, int &totalCascadas,
                         int &totalCombinaciones, int &puntuacion) {
    int cascadasEstaJugada = 0;
    int combinacionesEstaJugada = 0;
    int eliminadasEnCascadas = procesar_cascadas(tablero, filas, columnas,
                                                 &cascadasEstaJugada,
                                                 &combinacionesEstaJugada);

    totalFichasEliminadas += eliminadasEnCascadas;
    totalCascadas += cascadasEstaJugada;
    totalCombinaciones += combinacionesEstaJugada;
    puntuacion += eliminadasEnCascadas * 10 + cascadasEstaJugada * 25;

    mostrar_tablero(tablero, filas, columnas);

    std::cout << std::endl << "--- Estado de la partida ---" << std::endl;
    std::cout << "Dimensiones: " << filas << " x " << columnas << std::endl;
    std::cout << "Fichas eliminadas (total): " << totalFichasEliminadas << std::endl;
    std::cout << "Combinaciones detectadas (total): " << totalCombinaciones << std::endl;
    std::cout << "Cascadas en esta jugada: " << cascadasEstaJugada
              << " (total acumulado: " << totalCascadas << ")" << std::endl;
    std::cout << "Puntuacion: " << puntuacion << std::endl;
}

int main() {
    srand((unsigned int) time(0));

    int filas, columnas;
    std::cout << "Filas del tablero: ";
    while (!leer_entero(filas)) std::cout << "Entrada invalida. Filas del tablero: ";
    std::cout << "Columnas del tablero: ";
    while (!leer_entero(columnas)) std::cout << "Entrada invalida. Columnas del tablero: ";

    unsigned char* tablero = crear_tablero(filas, columnas);
    generar_aleatorio(tablero, filas, columnas);
    int capacidadBytes = bytes_necesarios(filas * columnas);

    int totalMovimientos = 0, totalFichasEliminadas = 0;
    int totalCascadas = 0, totalCombinaciones = 0, puntuacion = 0;

    mostrar_tablero(tablero, filas, columnas);

    bool jugando = true;
    while (jugando) {
        std::cout << std::endl << "--- Menu ---" << std::endl;
        std::cout << "1) Eliminar ficha" << std::endl;
        std::cout << "2) Insertar fila" << std::endl;
        std::cout << "3) Eliminar fila" << std::endl;
        std::cout << "4) Insertar columna" << std::endl;
        std::cout << "5) Eliminar columna" << std::endl;
        std::cout << "-1) Salir" << std::endl;
        std::cout << "Opcion: ";

        int opcion;
        if (!leer_entero(opcion)) { std::cout << "Entrada invalida." << std::endl; continue; }

        if (opcion == -1) { jugando = false; continue; }

        if (opcion == 1) {
            int fila, columna;
            std::cout << "Fila: ";
            if (!leer_entero(fila)) { std::cout << "Entrada invalida." << std::endl; continue; }
            std::cout << "Columna: ";
            if (!leer_entero(columna)) { std::cout << "Entrada invalida." << std::endl; continue; }

            if (!eliminar_ficha(tablero, filas, columnas, fila, columna)) {
                std::cout << "Coordenada fuera de rango." << std::endl;
                continue;
            }
            totalMovimientos++;
            totalFichasEliminadas++;
            aplicar_gravedad(tablero, filas, columnas);
            rellenar_vacios(tablero, filas, columnas);
            procesar_y_reportar(tablero, filas, columnas, totalFichasEliminadas,
                                totalCascadas, totalCombinaciones, puntuacion);

        } else if (opcion == 2 || opcion == 3) {
            int posicion;
            std::cout << "Posicion de fila (0 a " << (opcion == 2 ? filas : filas - 1) << "): ";
            if (!leer_entero(posicion)) { std::cout << "Entrada invalida." << std::endl; continue; }

            bool exito = (opcion == 2)
                             ? insertar_fila(tablero, filas, columnas, capacidadBytes, posicion)
                             : eliminar_fila(tablero, filas, columnas, capacidadBytes, posicion);

            if (!exito) { std::cout << "Operacion invalida (posicion fuera de rango, o el tablero quedaria sin filas)." << std::endl; continue; }

            procesar_y_reportar(tablero, filas, columnas, totalFichasEliminadas,
                                totalCascadas, totalCombinaciones, puntuacion);

        } else if (opcion == 4 || opcion == 5) {
            int posicion;
            std::cout << "Posicion de columna (0 a " << (opcion == 4 ? columnas : columnas - 1) << "): ";
            if (!leer_entero(posicion)) { std::cout << "Entrada invalida." << std::endl; continue; }

            bool exito = (opcion == 4)
                             ? insertar_columna(tablero, filas, columnas, capacidadBytes, posicion)
                             : eliminar_columna(tablero, filas, columnas, capacidadBytes, posicion);

            if (!exito) { std::cout << "Operacion invalida (posicion fuera de rango, o el tablero quedaria sin columnas)." << std::endl; continue; }

            procesar_y_reportar(tablero, filas, columnas, totalFichasEliminadas,
                                totalCascadas, totalCombinaciones, puntuacion);

        } else {
            std::cout << "Opcion no reconocida." << std::endl;
        }
    }

    delete[] tablero;
    return 0;
}