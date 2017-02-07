#include "blprimermejor.h"

/********************************************************************************/
//					Búsqueda Local: Búsqueda Local del primer mejor (BL)
/********************************************************************************/

/*
  Búsqueda Local primero mejor - Local Search

  Solución adaptada para los algoritmos BMB y GRASP, con solución inicial ya generada 
  antes de la llamada a la búsqueda local.

  Devuelve por referencia:
  sol_act -> con la primera mejor solución encontrada en un vecindario
  obj_act -> porcentaje de evaluación en la solución.
  evaluaciones -> devuelve tantas evaluaciones como se hagan.
*/
void LSfirst(BaseDatos &train, BaseDatos &test, vector<int> &sol_act, double &fobj_act, int &evaluaciones, int MAXEVA)
{
	vector<int> vecino;
	int features = train.getFeatures(), evaluar=0;
	double fobj_vecino=-1;

	// "Reservar memoria" e inicializar a 0
	for(int i=0; i<features; i++)
		vecino.push_back(0);

	//cout << "Busqueda Local del primero mejor..." << endl;

	// Bucle principal de la BL 
	do{ 
		// Actualizar la solución actual
		if(fobj_vecino > fobj_act)
		{
			sol_act = vecino;
			fobj_act = fobj_vecino;
		}

		// Repetir para toda solución factible del entorno de la sol_act
		for(int i=0; i<features; i++)
		{
			generate_Neighbors(sol_act, vecino, i);

			fobj_vecino = train.EvaluationFunction(vecino, test);			
			evaluar++;

			if(fobj_vecino > fobj_act)
				break;
		}			

	//Hasta que el sol_mejor sea peor o igual que la sol_actual y el número de evaluaciones menor que un máximo
	}while((fobj_vecino > fobj_act) && (evaluar < MAXEVA));

	//cout << "...Done.. evaluations: " << evaluaciones << endl;

	evaluaciones += evaluar;
	}