#include "ILS.h"

/********************************************************************************/
//			Búsqueda Local Reiterada - Iterative Local Search (ILS)
/********************************************************************************/

/*
  Modificar solución - Mutación.
  Cada vez que se realiza una mutación,
  se varía la pertenencia de t=0.1*n características
*/
void mutation(vector<int> &sol_actual)
{
	int i, j, Mu_next, features = sol_actual.size(), generados=0;
	int t=PROB10*features;
	double m;

	// Calculo la característica en el que se realizara la primera mutacion y la posicion en la que se producirá
	// Ceil() -> redondea hacia arriba siempre que sea positivo (sino hacia abajo), sea cual sea el decimal devuelve el siguiente entero
	if (PROB10 < 1.0) // Como es el 0.1 en PROB10, siempre se ejecuta la parte del if
		Mu_next = ceil(log(Rand()) / log(1.0 - PROB10)); 
	else
		Mu_next = 1;
	 
	if (PROB10>0)
	{
		 while (Mu_next<features && generados<t)
		 {
			// Se determina el cromosoma y el gen que corresponden a la posicion que se va a mutar 
			i=Mu_next%features;

			// Se efectua la mutacion sobre esa característica 
			if (sol_actual[i] == 0)
				sol_actual[i] = 1;
			else
				sol_actual[i] = 0;

			// Se calcula la siguiente posicion a mutar 
			if (PROB10<1)
			{
				m = Rand();
				Mu_next += ceil (log(m) / log(1.0 - PROB10));
			}
			else
				Mu_next += 1;
			generados++;
		}
	}
}

/*
  Búsqueda Local Reiterada - Iterative Local Search (ILS).

*/
void ILS(BaseDatos &train, BaseDatos &testVal, vector<int> &sol_actual)
{
	vector<int>sol_mejor;
	int evaluaciones = 0, features = train.getFeatures();
	double fobj_actual=-1, fobj_mejor=-1;

	//Inicialización 
	sol_actual.clear();
	for(int i=0; i<features; i++)
	{
		sol_actual.push_back(0);
		sol_mejor.push_back(0);
	}

	// Inicialización aleatoria
	generate_InitialSolution(sol_actual, features);

	// Búsqueda local del primer mejor
	LSfirst(train, testVal, sol_actual, fobj_actual);

	//Inicialización de la solución mejor
	fobj_mejor = fobj_actual;
	sol_mejor = sol_actual;

	cout << "ILS..." << endl;
	while(evaluaciones<SOLUCIONES)
	{
		// Modificar según una probabilidad
		mutation(sol_actual);

		// Búsqueda local del primer mejor
		LSfirst(train, testVal, sol_actual, fobj_actual);

		if(fobj_actual>fobj_mejor)
		{
			fobj_mejor = fobj_actual;
			sol_mejor = sol_actual;
		}
		evaluaciones++;
	}

	sol_actual = sol_mejor;
	fobj_actual = fobj_mejor;

	cout << "ILS ...Done" << endl;
}