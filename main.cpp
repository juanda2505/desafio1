#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include "combinaciones.h"

// Lee un entero de forma segura. Si el usuario escribe algo que no es un
// numero, limpia el error de cin y descarta lo que quedo en el buffer,
// para que el programa no quede en ciclo infinito leyendo un stream roto.
bool leer_entero(int &valor) {
    std::cin >> valor;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    return true;
}

int main() {
    srand((unsigned int) time(0));

    int filas, columnas;
    std::cout << "Filas del tablero: ";
    while (!leer_entero(filas)) {
        std::cout << "Entrada invalida. Filas del tablero: ";
    }
    std::cout << "Columnas del tablero: ";
    while (!leer_entero(columnas)) {
        std::cout << "Entrada invalida. Columnas del tablero: ";
    }

    unsigned char* tablero = crear_tablero(filas, columnas);
    generar_aleatorio(tablero, filas, columnas);

    // Metricas de la partida completa (Fase 5 del documento).
    int totalMovimientos = 0;
    int totalFichasEliminadas = 0;
    int totalCascadas = 0;
    int totalCombinaciones = 0;
    int puntuacion = 0;

    mostrar_tablero(tablero, filas, columnas);

    bool jugando = true;
    while (jugando) {
        int fila, columna;
        std::cout << std::endl << "Fila a eliminar (-1 para salir): ";
        if (!leer_entero(fila)) {
            std::cout << "Entrada invalida, intenta de nuevo." << std::endl;
            continue;
        }
        if (fila == -1) {
            jugando = false;
            continue;
        }
        std::cout << "Columna a eliminar: ";
        if (!leer_entero(columna)) {
            std::cout << "Entrada invalida, intenta de nuevo." << std::endl;
            continue;
        }

        if (!eliminar_ficha(tablero, filas, columnas, fila, columna)) {
            std::cout << "Coordenada fuera de rango." << std::endl;
            continue;
        }

        totalMovimientos++;
        totalFichasEliminadas++; // la ficha que el jugador elimino a mano

        // La eliminacion manual siempre dispara gravedad + relleno,
        // haya o no combinacion (asi funciona Sweet Crush: se elimina
        // una sola ficha directamente, no se espera a que haga match).
        aplicar_gravedad(tablero, filas, columnas);
        rellenar_vacios(tablero, filas, columnas);

        // A partir de ahi, puede que el relleno haya generado
        // combinaciones nuevas: se procesan en cascada hasta estabilizar.
        int cascadasEstaJugada = 0;
        int combinacionesEstaJugada = 0;
        int eliminadasEnCascadas = procesar_cascadas(tablero, filas, columnas,
                                                     &cascadasEstaJugada,
                                                     &combinacionesEstaJugada);

        totalFichasEliminadas += eliminadasEnCascadas;
        totalCascadas += cascadasEstaJugada;
        totalCombinaciones += combinacionesEstaJugada;

        // Criterio de puntuacion (documentado): 10 puntos por cada ficha
        // eliminada en cascada, mas un bono de 25 puntos por cada nivel
        // de cascada extra, para premiar reacciones en cadena largas.
        puntuacion += eliminadasEnCascadas * 10 + cascadasEstaJugada * 25;

        mostrar_tablero(tablero, filas, columnas);

        std::cout << std::endl << "--- Estado de la partida ---" << std::endl;
        std::cout << "Dimensiones: " << filas << " x " << columnas << std::endl;
        std::cout << "Movimientos del jugador: " << totalMovimientos << std::endl;
        std::cout << "Fichas eliminadas (total): " << totalFichasEliminadas << std::endl;
        std::cout << "Combinaciones detectadas (total): " << totalCombinaciones << std::endl;
        std::cout << "Cascadas en esta jugada: " << cascadasEstaJugada
                  << " (total acumulado: " << totalCascadas << ")" << std::endl;
        std::cout << "Puntuacion: " << puntuacion << std::endl;
    }

    delete[] tablero;
    return 0;
}