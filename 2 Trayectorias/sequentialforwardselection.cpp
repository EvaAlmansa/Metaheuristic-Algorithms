#include "sequentialforwardselection.h"

/********************************************************************************/
//					Solución Greedy: Sequential Forward Selection (SFS)
/********************************************************************************/

/*
  Ordena de menor a mayor el vector F, a partir del máximo en 
  la función evaluación de la clase BaseDatos. 
  Para ello, es necesario tener una clase train (entrenamiento)
  y otra de test (prueba).
  La función evaluación devuelve el porcentaje de aciertos.

  Devuelve por referencia: 
  pos --> la posición con el mejor porcentaje con la siguiente característica
  fobj_actual --> el porcentaje de la siguiente mejor combinación de características
*/
void caracteristica_mas_prometedora(BaseDatos &train, BaseDatos &test, vector<int> &sol_actual, vector<int> &F, int &pos, double &fobj_actual)
{
	vector<E_dist> ordenadas;
	E_dist sobre_porcentajes;

	for(int i=0; i<F.size(); i++)
	{
		if(F[i] != 0)
		{
			sol_actual[i] = 1;
			//Se obtiene el porcentaje (reutilizo la estructura creada para distancia euclidea)
			sobre_porcentajes.distance = train.EvaluationFunction(sol_actual, test);
			sobre_porcentajes.pos = i;

			ordenadas.push_back(sobre_porcentajes);

			sol_actual[i] = 0;
		}
	}
	
	// Se ordenan de menor a mayor porcentaje, lo cual interesa para el back
	sort(ordenadas.begin(), ordenadas.end(), compare);

	pos = ordenadas.back().pos;
	fobj_actual = ordenadas.back().distance;
}

/*
  Obtiene una característica de las que quedan en el vector F,
  que tiene el máximo en la función evaluación de la clase 
  BaseDatos. 
  Para ello, es necesario tener una clase train (entrenamiento)
  y otra de test (prueba).
  La función evaluación devuelve el porcentaje de aciertos.
*/
/*int caracteristica_mas_prometedora(vector<int> &F, BaseDatos &train, BaseDatos &test)
{
	double max_porcentaje=-1;
	int max_caracteristica=-1;
	vector<int>caracteristicas;

	for(int i=0; i<F.size(); i++)
		caracteristicas.push_back(0);

	for(int i=0; i<F.size(); i++)
	{
		if(F[i]!=0)
		{
			caracteristicas[i] = 1;
			double porcentaje = train.EvaluationFunction(caracteristicas,test);
			if(porcentaje>max_porcentaje)
			{
				max_porcentaje = porcentaje;
				max_caracteristica = i;
			}
			caracteristicas[i] = 0;
		}
	}
	return max_caracteristica;
}*/

/*
  Función que comprueba sí aún queda alguna características.
  Si estan todas a cero, devuelve true: Esta vacía.
*/
bool vacia(vector<int> &F)
{
	for(int i=0; i<F.size(); i++)
	{
		if(F[i]==1)
			return false;
	}
	return true;
}

/*
  Solución Greedy: Método Sequential Forward Selection (SFS)
  1. Parte de conjunto vacío de características.
  2. En cada paso añade la característica más prometedora.
  3. La más prometedora es la que produce una mayor ganancia 
  en la función de evaluación con respecto al conjunto de
  características ya seleccionadas.
  4. El algoritmo para cuando ninguna de las características
  que quedan por seleccionar produce una mejora en la función
  de evaluación.
  5. Usaremos la función de evaluación indicada, basada en el 
  acierto de clasificación de k-NN con k=3.
*/
void SFS(BaseDatos &train, BaseDatos &test, vector<int> &sol_actual)
{
	vector<int> F;
	bool fin = false;
	int fp;
	double fobj_actual = 0.0, fobj_mejor = 0.0;

	//Inicialización, con todas las características a 1 para F
	sol_actual.clear();
	for(int i=0; i<train.getFeatures(); i++)
	{
		F.push_back(1);

		sol_actual.push_back(0);
	}

	cout << "Sequential Forward Selection..." << endl;
	//Búsqueda Greedy: SFS
	while(!vacia(F) && !fin)
	{
		//fp = caracteristica_mas_prometedora(F, train, test);

		caracteristica_mas_prometedora(train, test, sol_actual, F, fp, fobj_actual);

		if(fobj_actual > fobj_mejor)
		{
			// Informe en pantalla
			//printf("\nCaracterística: %d.\n", fp);
			//printf("   Mejor Sol. = %12.6f. Sol. Actual = %12.6f \n", fobj_actual, fobj_mejor);

			//Actualizar solución mejor
			sol_actual[fp] = 1;
			fobj_mejor = fobj_actual;
		}
		else
			fin = true;
		F[fp] = 0;
	}

	cout << "...Done" << endl;
}