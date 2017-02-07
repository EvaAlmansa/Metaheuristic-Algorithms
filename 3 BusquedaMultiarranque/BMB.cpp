#include "BMB.h"

/********************************************************************************/
//							Búsqueda Multiarranque Básica (BMB)
/********************************************************************************/

/*
  Búsqueda Multiarranque Básica (BMB).
  El algoritmo BMB consistirá simplemente en generar un determinado número de
  soluciones aleatorias iniciales y optimizar cada una de ellas con el algoritmo de BL
  considerado. Se devolverá la mejor solución encontrada en todo el proceso.
*/
void BMB(BaseDatos &train, BaseDatos &testVal, vector<int> &sol_actual)
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

	cout << "BMB..." << endl;
	while(evaluaciones<SOLUCIONES)
	{
		generate_InitialSolution(sol_actual, features);
		
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

	cout << "BMB ...Done" << endl;
}