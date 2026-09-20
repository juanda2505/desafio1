#include "combinaciones.h"
#include <cstdlib>
//Busca secuencias de 3 o mas fichas iguales en linea recta (horizontal y vertical)
//y las "pinta" en una matriz borrador llamada marcado
int detectar_combinaciones(const unsigned char* tablero, int filas, int columnas, unsigned char* marcado)
{
    for (int i = 0; i < filas * columnas; i++)
    {
        marcado[i] = 0;  //arreglo en ceros
    }

    int gruposEncontrados = 0;

    // Barrido horizontal: dentro de cada fila, busca corridas de 3+ iguales.
    //se fija f y se recorre c
    for (int f = 0; f < filas; f++)
    {
        int c = 0;
        while (c < columnas)
        {
            unsigned char valor = leer_ficha(tablero, f, c, columnas);
            if (valor == ESTADO_VACIO || valor == ESTADO_ESPECIAL) //se decide el valo a comparar
            {
                c++;
                continue;
            }
            int inicio = c;
            while (c + 1 < columnas && leer_ficha(tablero, f, c + 1, columnas) == valor)//se pregunta si la c siguiente es < a columnas y si esigual que la c actual
            {
                c++;
            }
            int largo = c - inicio + 1; //calculamos cuantas posiciones tiene la racha
            if (largo >= 3) //si largo >=3 contamos un grupo nuevo
            {
                gruposEncontrados++;
                for (int k = inicio; k <= c; k++)//con un for recorremos desde inicio hasta c marcando cada una de esas posiciones
                {
                    marcado[calcular_indice(f, k, columnas)] = 1;//coloca unos en las combinaciones de 3+ y cero en las restantes
                }
            }
            c++;
        }
    }

    // Barrido vertical: dentro de cada columna, busca corridas de 3+ iguales.
    //misma logica del horizontal pero ahora se fica c y se recorre f
    for (int c = 0; c < columnas; c++)
    {
        int f = 0;
        while (f < filas)
        {
            unsigned char valor = leer_ficha(tablero, f, c, columnas);
            if (valor == ESTADO_VACIO || valor == ESTADO_ESPECIAL)
            {
                f++;
                continue;
            }
            int inicio = f;
            while (f + 1 < filas && leer_ficha(tablero, f + 1, c, columnas) == valor)
            {
                f++;
            }
            int largo = f - inicio + 1;
            if (largo >= 3)
            {
                gruposEncontrados++;
                for (int k = inicio; k <= f; k++)
                {
                    marcado[calcular_indice(k, c, columnas)] = 1;
                }
            }
            f++;
        }
    }

    return gruposEncontrados;
}
//convertir en ESTADO_VACIO todo lo que detectar_combinaciones dejó marcado, y contar cuantas fichas se eliminaron
int eliminar_marcadas(unsigned char* tablero, const unsigned char* marcado, int filas, int columnas)
{
    int contador = 0;
    for (int f = 0; f < filas; f++)
    {
        for (int c = 0; c < columnas; c++)
        {
            int idx = calcular_indice(f, c, columnas);
            if (marcado[idx])
            {
                escribir_ficha(tablero, f, c, columnas, ESTADO_VACIO);
                contador++;
            }
        }
    }
    return contador;
}

void aplicar_gravedad(unsigned char* tablero, int filas, int columnas)
{
    for (int c = 0; c < columnas; c++)
    {
        int destino = filas - 1; // arranca en la fila mas baja de la columna
        for (int origen = filas - 1; origen >= 0; origen--)
        {
            unsigned char valor = leer_ficha(tablero, origen, c, columnas);
            if (valor != ESTADO_VACIO)
            {
                if (destino != origen)
                {
                    escribir_ficha(tablero, destino, c, columnas, valor);
                    escribir_ficha(tablero, origen, c, columnas, ESTADO_VACIO);
                }
                destino--;
            }
        }
        // Todo lo que quede entre la fila 0 y 'destino' (inclusive) esta vacio.
    }
}

void rellenar_vacios(unsigned char* tablero, int filas, int columnas)
{
    for (int f = 0; f < filas; f++)
    {
        for (int c = 0; c < columnas; c++)
        {
            if (leer_ficha(tablero, f, c, columnas) == ESTADO_VACIO)
            {
                unsigned char valor = (unsigned char) (rand() % 6);
                escribir_ficha(tablero, f, c, columnas, valor);
            }
        }
    }
}

int procesar_cascadas(unsigned char* tablero, int filas, int columnas, int* numCascadas, int* numCombinaciones)
{
    int totalEliminadas = 0;
    *numCascadas = 0;
    *numCombinaciones = 0;

    unsigned char* marcado = new unsigned char[filas * columnas];

    int combosEnEstaVuelta;
    while ((combosEnEstaVuelta = detectar_combinaciones(tablero, filas, columnas, marcado)) > 0)
    {
        *numCombinaciones += combosEnEstaVuelta;
        totalEliminadas += eliminar_marcadas(tablero, marcado, filas, columnas);
        (*numCascadas)++;
        aplicar_gravedad(tablero, filas, columnas);
        rellenar_vacios(tablero, filas, columnas);
    }

    delete[] marcado;
    return totalEliminadas;
}