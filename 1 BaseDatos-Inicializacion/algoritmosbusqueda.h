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
  Generaci�n de la soluci�n inicial:
  La soluci�n inicial se generar� de forma aleatoria en todos los casos.
*/
void generate_InitialSolution(vector<int> &sol_inicial, int features);

/*
  Esquema de generaci�n de vecinos:
  Se emplear� el movimiento de cambio de pertenencia Flip(s, i) que 
  altera la selecci�n de la caracter�stica correspondendiente a la 
  posici�n i en la soluci�n s (pasa de 0 a 1 o viceversa). Su aplicaci�n
  concreta depender� del algoritmo espec�fico.
*/
void generate_Neighbors(vector<int> &sol_actual, vector<int> &vecino, int);

/*
  Criterio de aceptaci�n:
  Se considera una mejora cuando se reduce el valor global de la funci�n
  objetivo.
*/

/*
  Criterio de parada:
  Se detendr� la ejecuci�n del algoritmo bien cuando no se encuentre mejora
  todo el entorno (BL) o bien cuando se hayan evaluado 15000 soluciones
  distintas  (en cualquier caso, en la BL, se parar� tambi�n despu�s de 15000
  evaluaciones aunque siguiera habiendo soluciones mejores en el entorno).
*/

#endif