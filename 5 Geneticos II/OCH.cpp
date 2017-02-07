#include "och.h"

/*
  Función que genera un número aleatorio para escoger el 
  número de características según la tabla de probabilidades.

  Devuelve por referencia:
  nc -> número de características.
*/
void ruletaNC(vector<double> &feromonas, int &features, int &nc)
{
	double prob = Rand(), acum=0.0;

	for(int i=0; i<features; i++)
	{
		acum += feromonas[i+features];
		if(prob<acum)
		{
			nc = i;
			break;
		}
	}
}

/*
  Función que selecciona por el método de la ruleta una característica.
  
  Nota: Usada en el método de regla transición.

  Devuelve por referencia:
  caracteristica -> característica seleccionada.
*/
void ruletaRT(vector<double> &feromonas, vector<double> &inH, int &features, int &caracteristica)
{
	double prob = Rand(), acum=0.0, suma=0.0;

	//Sumatoria para el calculo de la probabilidad
	for(int i=0; i<features; i++)
		suma += pow(feromonas[i],ALFA)*pow(inH[i],BETA);

	for(int i=0; i<features; i++)
	{
		acum += pow(feromonas[i],ALFA)*pow(inH[i],BETA)/suma;
		if(prob<acum)
		{
			caracteristica = i;
			break;
		}
	}
}

/*
  Función que comprueba si dado una característica se encuentra ya seleccionada.

  Devuelve:
  true -> Si no se encuentra f en C, en otro caso, devuelve false.
*/
bool notUsed(vector<int> &C, int f)
{
	for(int i=0; i<C.size(); i++)
	{
		if(C[i]==1 && f==i)
			return false;
	}
	return true;
}

/*
  Función que devuelve la siguiente característica para la hormiga.

  Selección de característica teniendo en cuenta:
	tau-car^alfa*inf.Heuristica^beta

  Devuelve por referencia:
  features -> Añade una característica 
  caracteristica -> la característica modificada
*/
void reglaTransicion(vector<double> &feromonas, vector<double> &inH, vector<int> &C, int &features, int &caracteristica, double q0=1)
{
	double q = Rand(), result=0.0;
	double mejorF=-1;

	//Escoge el movimiento la hormiga
	if(q<=q0)
	{
		for(int i=0; i<features; i++)
		{
			if(notUsed(C, i))
			{
				result = pow(feromonas[i],ALFA)*pow(inH[i],BETA);
				if(result>mejorF)
				{
					mejorF = result;
					caracteristica = i;
				}
			}
		}
	}else 
		ruletaRT(feromonas, inH, features, caracteristica);

	//Actualizar la solución 
	C[caracteristica] = 1;
}

/*
  Función que aplica la actualización local de feromona.
  
  tau(t) = (1 - evaporacion)*tau(t-1)+evaporacion*tau0

  Nota: El umbral esta establecido en tau0, nunca será el resultado de evaporación menor que tau0

  Devuelve por referencia:
  feromonas -> actualiza el feromona con respecto a la característica anterior.
*/
void actualizacionLocalFeromona(vector<double> &feromonas, int &caracteristica, double &tau0)
{
	feromonas[caracteristica] = (1-EVAPORACION_LOCAL)*feromonas[caracteristica]+EVAPORACION_LOCAL*tau0;
}

/*
  Función que aplica la actualización global de feromona a la mejor solución.

  Devuelve por referencia:
  feromonas -> actualiza según la ecuación.
*/
void actualizacionGlobalFeromona(vector<double> &feromonas, int &caracteristica, double &fobjmejor)
{
	feromonas[caracteristica] = (1-EVAPORACION)*feromonas[caracteristica]+EVAPORACION*fobjmejor;
}

/*
  Función que aplica la actualización de feromona a la mejor solución y controla que se
  encuentre entre unos límites dados Min-Max.

  Devuelve por referencia:
  feromonas -> actualiza según la ecuación.
*/
void actualizacionMaxMinFeromona(vector<double> &feromonas, int &caracteristica, double &fobjmejor, double &tmin, double &tmax)
{
	feromonas[caracteristica] = (1-EVAPORACION)*feromonas[caracteristica]+EVAPORACION*fobjmejor;

	//Comprobar que se encuentra en los límites establecidos por tmin <= tau(t) <= tmax
	if(feromonas[caracteristica]<tmin)
		feromonas[caracteristica] = tmin;
	else if(feromonas[caracteristica]>tmax)
		feromonas[caracteristica] = tmax;
}

/*
  Función que aplica la evaporación para el número de características ya usadas por las hormigas.

  Devuelve por referencia:
  feromoas -> evapora/reduce la segunda parte del vector correspondiente al número de características.
*/
void evaporacionNumCar(vector<double> &feromonas, vector<int> &nc, int &features)
{
	for(int i=0; i<nc.size(); i++)
			feromonas[nc[i]+features] = (1-EVAPORACION)*feromonas[nc[i]+features];
}

/*
  Sistema de Colonias de Hormigas BL (SCH_BL).
  
  Nota: El número de hormigas ha de ser menor que el número de características, en otro caso,
  habría que modificar el código. Último for interno al de las iteraciones, donde se actualiza el número 
  de características por cada hormiga.

  Devuelve por referencia:
  sol_mejor -> correspondiente a la mejor solución encontrada.
*/
void SCH_BL(BaseDatos &train, BaseDatos &test, vector<int> &sol_mejor, vector<double> &inH, double &tau0, double q0)
{
	int features = train.getFeatures(), num, caracteristica, evaluaciones=0;
	vector<double>feromonas;
	vector<Features>ants;
	Features C;
	vector<int> sol_actual, nc;
	double fobj_mejor=-1, fobj_actual=-1;

	//Inicialización del feromona con cada característica con frecuencia 1/n -> equiprobable
	for(int i=0; i<features*2; i++) //Feromonas(n)+NumCaract(n) = 2*n
	{
		if(i<features)
			feromonas.push_back(tau0);
		else
			feromonas.push_back(1/(features*1.0));
	}

	//Inicializar/Reservar C como el conjunto de características sin evaluar, sol_actual y sol_mejor
	sol_mejor.clear();
	for(int i=0; i<features; i++)
	{
		C.features.push_back(0);
		sol_actual.push_back(0);
		sol_mejor.push_back(0);
	}

	//Inicializar hormigas y el número de característica para cada hormiga
	for(int k=0; k<NHORMIGAS; k++)
	{
		ants.push_back(C);
		ruletaNC(feromonas, features, num);
		nc.push_back(num);
	}

	do{
		for(int i=0; i<features; i++)
		{
			for(int k=0; k<NHORMIGAS; k++)
			{
				//Si el número de características para cada hormiga es menor que la siguiente característica
				if(i<nc[k])
				{
					reglaTransicion(feromonas, inH, ants[k].features, features, caracteristica, q0);
					actualizacionLocalFeromona(feromonas, caracteristica, tau0);
				}
			}
		}

		//Evaluación de las soluciones encontradas por cada hormiga
		fobj_actual=-1;
		for(int k=0; k<NHORMIGAS; k++)
		{
			train.EvaluationFunctionFeatures(ants[k], test);
			evaluaciones++;

			//Búsqueda local primero mejor
			LSfirst(train, test, ants[k].features, ants[k].fobjetivo, evaluaciones);

			if(fobj_actual < ants[k].fobjetivo)
			{
				sol_actual = ants[k].features;
				fobj_actual = ants[k].fobjetivo;
			}
		}

		//Actualizar la solución mejor global
		if(fobj_actual > fobj_mejor)
		{
			sol_mejor = sol_actual;
			fobj_mejor = fobj_actual;
		}

		//Actualización del feromona, parte del número de características
		evaporacionNumCar(feromonas, nc, features);

		//Actualización global de feromona
		for(int i=0; i<features; i++)
		{
			//Actualizar la feromona tau-car
			if(sol_mejor[i]==1)
				actualizacionGlobalFeromona(feromonas, i, fobj_mejor);
			//Actualizar el número de características para cada hormiga a partir de la feromona tau-num y así añadir diversidad
			if(i<NHORMIGAS)
			{
				ruletaNC(feromonas, features, num);		
				nc[i] = num;
			}
		}

	}while(evaluaciones<NMAX);
}

/*
  Sistema de Hormigas MAX MIN (SHMM_BL).

  Nota: El número de hormigas ha de ser menor que el número de características, en otro caso,
  habría que modificar el código. Último for interno al de las iteraciones, donde se actualiza el número 
  de características por cada hormiga.

  Devuelve por referencia:
  sol_mejor -> correspondiente a la mejor solución encontrada.
*/
void SHMM_BL(BaseDatos &train, BaseDatos &test, vector<int> &sol_mejor, vector<double> &inH)
{
	int features = train.getFeatures(), num, caracteristica, evaluaciones = 0;
	vector<double>feromonas;
	vector<Features>ants;
	Features C;
	vector<int> sol_actual, nc;
	double fobj_mejor=-1, fobj_actual=-1, tmax, tmin;

	//Inicializar/Reserva de las soluciones
	sol_mejor.clear();
	for(int i=0; i<features; i++)
	{
		sol_actual.push_back(0);
		sol_mejor.push_back(0);
	}

	//Generación de una solución inicial aleatoria
	generate_InitialSolution(sol_actual, features);
	fobj_actual = train.EvaluationFunction(sol_actual, test);
	fobj_mejor = fobj_actual;
	sol_mejor = sol_actual;

	//Inicializar los valores
	tmax = fobj_mejor/EVAPORACION;
	tmin = tmax/500;

	//Inicialización del feromona con cada característica con frecuencia 1/n -> equiprobable
	for(int i=0; i<features*2; i++) //Feromonas(n)+NumCaract(n) = 2*n
	{
		if(i<features)
			feromonas.push_back(tmax);
		else
			feromonas.push_back(1/(features*1.0));
	}

	//Inicializar/Reservar C como el conjunto de características sin evaluar
	for(int i=0; i<features; i++)
		C.features.push_back(0);

	//Inicializar hormigas y el número de característica para cada hormiga
	for(int k=0; k<NHORMIGAS; k++)
	{
		ants.push_back(C);
		ruletaNC(feromonas, features, num);
		nc.push_back(num);
	}

	do{

		for(int i=0; i<features; i++)
		{
			for(int k=0; k<NHORMIGAS; k++)
			{
				//Si el número de características para cada hormiga es menor que la siguiente característica
				if(i<nc[k])
					reglaTransicion(feromonas, inH, ants[k].features, features, caracteristica);
			}
		}

		//Evaluación de las soluciones encontradas por cada hormiga
		fobj_actual=-1;
		for(int k=0; k<NHORMIGAS; k++)
		{
			train.EvaluationFunctionFeatures(ants[k], test);
			evaluaciones++;

			//Búsqueda local primero mejor
			LSfirst(train, test, ants[k].features, ants[k].fobjetivo, evaluaciones);

			if(fobj_actual < ants[k].fobjetivo)
			{
				sol_actual = ants[k].features;
				fobj_actual = ants[k].fobjetivo;
			}
		}

		//Actualizar la solución mejor global
		if(fobj_actual > fobj_mejor)
		{
			sol_mejor = sol_actual;
			fobj_mejor = fobj_actual;

			tmax = fobj_mejor/EVAPORACION;
			tmin = tmax/500;
		}

		//Actualización del feromona, parte del número de características
		evaporacionNumCar(feromonas, nc, features);

		//Actualización Min-Max de feromona
		for(int i=0; i<features; i++)
		{
			//Actualizar la feromona tau-car
			if(sol_mejor[i]==1)
				actualizacionMaxMinFeromona(feromonas, i, fobj_mejor, tmin, tmax);
			//Actualizar el número de características para cada hormiga a partir de la feromona tau-num y así añadir diversidad
			if(i<NHORMIGAS)
			{
				ruletaNC(feromonas, features, num);		
				nc[i] = num;
			}
		}

	}while(evaluaciones<NMAX);
}