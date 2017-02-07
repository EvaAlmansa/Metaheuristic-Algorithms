#include "algoritmosbusqueda.h"


/********************************************************************************/
//  			Componentes comunes de los Algoritmos de B�squeda
/********************************************************************************/

/*
  Generaci�n de la soluci�n inicial:
  La soluci�n inicial se generar� de forma aleatoria en todos los casos.
*/
void generate_InitialSolution(vector<int> &sol_inicial, int features)
{
	for(int i=0; i<features; i++)
	{
		int valor = Randint(0, 1);
		sol_inicial[i] = valor;
	}
}

/*
  Esquema de generaci�n de vecinos:
  Se emplear� el movimiento de cambio de pertenencia Flip(s, i) que 
  altera la selecci�n de la caracter�stica correspondiente a la 
  posici�n i en la soluci�n s (pasa de 0 a 1 o viceversa). Su aplicaci�n
  concreta depender� del algoritmo espec�fico.
*/
void generate_Neighbors(vector<int> &sol_actual, vector<int> &vecino, int pos)
{
	// Inicializo vecino
	vecino = sol_actual;
	// Flip(s, i)
	if(sol_actual[pos] == 0)
		vecino[pos] = 1;
	else
		vecino[pos] = 0;
}

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

