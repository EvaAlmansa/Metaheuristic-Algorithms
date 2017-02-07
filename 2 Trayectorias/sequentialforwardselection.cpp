#include "sequentialforwardselection.h"

/********************************************************************************/
//					Soluci�n Greedy: Sequential Forward Selection (SFS)
/********************************************************************************/

/*
  Ordena de menor a mayor el vector F, a partir del m�ximo en 
  la funci�n evaluaci�n de la clase BaseDatos. 
  Para ello, es necesario tener una clase train (entrenamiento)
  y otra de test (prueba).
  La funci�n evaluaci�n devuelve el porcentaje de aciertos.

  Devuelve por referencia: 
  pos --> la posici�n con el mejor porcentaje con la siguiente caracter�stica
  fobj_actual --> el porcentaje de la siguiente mejor combinaci�n de caracter�sticas
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
  Obtiene una caracter�stica de las que quedan en el vector F,
  que tiene el m�ximo en la funci�n evaluaci�n de la clase 
  BaseDatos. 
  Para ello, es necesario tener una clase train (entrenamiento)
  y otra de test (prueba).
  La funci�n evaluaci�n devuelve el porcentaje de aciertos.
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
  Funci�n que comprueba s� a�n queda alguna caracter�sticas.
  Si estan todas a cero, devuelve true: Esta vac�a.
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
  Soluci�n Greedy: M�todo Sequential Forward Selection (SFS)
  1. Parte de conjunto vac�o de caracter�sticas.
  2. En cada paso a�ade la caracter�stica m�s prometedora.
  3. La m�s prometedora es la que produce una mayor ganancia 
  en la funci�n de evaluaci�n con respecto al conjunto de
  caracter�sticas ya seleccionadas.
  4. El algoritmo para cuando ninguna de las caracter�sticas
  que quedan por seleccionar produce una mejora en la funci�n
  de evaluaci�n.
  5. Usaremos la funci�n de evaluaci�n indicada, basada en el 
  acierto de clasificaci�n de k-NN con k=3.
*/
void SFS(BaseDatos &train, BaseDatos &test, vector<int> &sol_actual)
{
	vector<int> F;
	bool fin = false;
	int fp;
	double fobj_actual = 0.0, fobj_mejor = 0.0;

	//Inicializaci�n, con todas las caracter�sticas a 1 para F
	sol_actual.clear();
	for(int i=0; i<train.getFeatures(); i++)
	{
		F.push_back(1);

		sol_actual.push_back(0);
	}

	cout << "Sequential Forward Selection..." << endl;
	//B�squeda Greedy: SFS
	while(!vacia(F) && !fin)
	{
		//fp = caracteristica_mas_prometedora(F, train, test);

		caracteristica_mas_prometedora(train, test, sol_actual, F, fp, fobj_actual);

		if(fobj_actual > fobj_mejor)
		{
			// Informe en pantalla
			//printf("\nCaracter�stica: %d.\n", fp);
			//printf("   Mejor Sol. = %12.6f. Sol. Actual = %12.6f \n", fobj_actual, fobj_mejor);

			//Actualizar soluci�n mejor
			sol_actual[fp] = 1;
			fobj_mejor = fobj_actual;
		}
		else
			fin = true;
		F[fp] = 0;
	}

	cout << "...Done" << endl;
}