#ifndef __ALGORITMOSBUSQUEDA_H
#define __ALGORITMOSBUSQUEDA_H

#include <vector>
#include "basedatos.h"

// Constantes para los distintos criterios de parada
#define NMAX 15000 
#define SOLUCIONES 25

using namespace std;

/********************************************************************************/
//					Componentes de los Algoritmos
/********************************************************************************/

/*
  Generación de la solución inicial:
  La solución inicial se generará de forma aleatoria en todos los casos.
*/
void generate_InitialSolution(vector<int> &sol_inicial, int features);

/*
  Esquema de generación de vecinos:
  Se empleará el movimiento de cambio de pertenencia Flip(s, i) que 
  altera la selección de la característica correspondendiente a la 
  posición i en la solución s (pasa de 0 a 1 o viceversa). Su aplicación
  concreta dependerá del algoritmo específico.
*/
void generate_Neighbors(vector<int> &sol_actual, vector<int> &vecino, int);

/*
  Criterio de aceptación:
  Se considera una mejora cuando se reduce el valor global de la función
  objetivo.
*/

/*
  Criterio de parada:
  Se detendrá la ejecución del algoritmo bien cuando no se encuentre mejora
  todo el entorno (BL) o bien cuando se hayan evaluado 15000 soluciones
  distintas  (en cualquier caso, en la BL, se parará también después de 15000
  evaluaciones aunque siguiera habiendo soluciones mejores en el entorno).
*/

#endif