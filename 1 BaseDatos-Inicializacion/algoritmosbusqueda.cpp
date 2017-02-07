#include "algoritmosbusqueda.h"


/********************************************************************************/
//  			Componentes comunes de los Algoritmos de Búsqueda
/********************************************************************************/

/*
  Generación de la solución inicial:
  La solución inicial se generará de forma aleatoria en todos los casos.
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
  Esquema de generación de vecinos:
  Se empleará el movimiento de cambio de pertenencia Flip(s, i) que 
  altera la selección de la característica correspondiente a la 
  posición i en la solución s (pasa de 0 a 1 o viceversa). Su aplicación
  concreta dependerá del algoritmo específico.
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

