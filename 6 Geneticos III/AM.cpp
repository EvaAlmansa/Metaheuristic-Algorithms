#include "AM.h"

/*
  Funci�n que aplica la BL del primer mejor al n�mero de soluciones indicadas previamente.

  Devuelve por referencia:
  Pactual -> Modificada con la BL primer mejor seg�n corresponda.
  features_mejor -> S�lo lo actualiza en el caso de que la prob_bl > 1
  features_peor -> S�lo lo actualiza en el caso de que la prob_bl > 1
*/
void aplicarBL(BaseDatos &train, BaseDatos &testVal, vector<Features> &Pactual, Features &features_mejor, Features &features_peor, int &evaluaciones, double prob_bl)
{
	int next, genes = Pactual[0].features.size(), cromosomas=Pactual.size(), i;
	double m;

	if(prob_bl <= 1.0)
	{
		// Calculo del cromosoma en el que se realizara la primera mutacion y la posicion en la que se producir�
		// Ceil() -> redondea hacia arriba siempre que sea positivo (sino hacia abajo), sea cual sea el decimal devuelve el siguiente entero
		if (prob_bl < 1.0) // Como es el 0.1 en prob_g, siempre se ejecuta la parte del if
			next = ceil(log(Rand()) / log(1.0 - prob_bl)); 
		else
			next = 0;
	 
		if (prob_bl>0)
		{
			 while (next<cromosomas)
			 {
				// Se determina el cromosoma y el gen que corresponden a la posicion que se va a mutar 
				//i=next/cromosomas;

				// Aplicar la BL primer mejor
				LSfirst(train, testVal, Pactual[next].features, Pactual[next].fobjetivo, evaluaciones);
				Pactual[next].evaluada = 1;

				// Se calcula la siguiente posicion a mutar 
				if (prob_bl<1)
				{
					m = Rand();
					next += ceil (log(m) / log(1.0 - prob_bl));
				}
				else
					next += 1;
			}
		}
	}
	else{ //prob_bl > 1 -> prob_bl*num_caracter�sticas = num_mejores
		populationEvaluation(train, testVal, Pactual, features_mejor, features_peor, evaluaciones);

		vector<E_dist> num_mejores;
		E_dist d;
		for(int n=0; n<Pactual.size(); n++)
		{			
			d.distance = Pactual[n].fobjetivo;
			d.pos = n;

			if(num_mejores.size()<prob_bl)
			{
				num_mejores.push_back(d);
				if(num_mejores.size()==prob_bl)
					sort(num_mejores.begin(), num_mejores.end(), compare2);
			}
			else if(num_mejores[prob_bl-1].distance>d.distance)
			{
				num_mejores.pop_back();
				num_mejores.push_back(d);
				sort(num_mejores.begin(), num_mejores.end(), compare);
			}
		}

		//Aplicar la BL primer mejor
		for(int n=0; n<num_mejores.size(); n++)
		{
			int pos = num_mejores[n].pos;
			// Aplicar la BL primer mejor
			LSfirst(train, testVal, Pactual[pos].features, Pactual[pos].fobjetivo, evaluaciones);
			Pactual[pos].evaluada = 1;
		}
	}
}

/*
  Funci�n para actualizar la mejor y peor de las soluciones encontradas.


* /
void updateBestWorst(vector<Features> &Pactual, Features &features_mejor, Features &features_peor)
{
	double fobj_mejor=-1, fobj_peor=101;
	int pos_mejor=-1, pos_peor=-1;

	// B�squeda del mejor y peor en la poblaci�n
	for(int i=0; i<Pactual.size(); i++)
	{
		if(Pactual[i].fobjetivo < fobj_peor)
		{
			fobj_peor = Pactual[i].fobjetivo;
			pos_peor = i;
		}

		if(Pactual[i].fobjetivo > fobj_mejor)
		{
			fobj_mejor = Pactual[i].fobjetivo;
			pos_mejor = i;
		}

		// Reinicializar en la poblaci�n actual el mejor y peor
		Pactual[i].fobj_mejor = false;
		Pactual[i].fobj_peor = false;
	}

	// Actualizar la mejor soluci�n global
	if(mejor)
	{		
		Pactual[pos_mejor].fobj_mejor = true;
		features_mejor = Pactual[pos_mejor];
	}else{
		// En otro caso, buscar la mejor soluci�n de la poblaci�n
		for(int i=0; i<Pactual.size(); i++)
		{
			if(Pactual[i].fobjetivo>fob_mejor)
			{
				fob_mejor = Pactual[i].fobjetivo;
				pos_mejor = i;
			}
		}
		Pactual[pos_mejor].fobj_mejor = true;
	}

	// Actualizar la peor soluci�n global auxiliar
	if(peor)
	{
		Pactual[pos_peor].fobj_peor = true;
		features_peor = Pactual[pos_peor];
	}else{
		// En otro caso, buscar la mejor soluci�n de la poblaci�n
		for(int i=0; i<Pactual.size(); i++)
		{
			if(Pactual[i].fobjetivo<fobj_peor)
			{
				fobj_peor = Pactual[i].fobjetivo;
				pos_peor = i;
			}
		}
		Pactual[pos_peor].fobj_peor = true;

		// La soluci�n peor es s�lo auxiliar, por lo tanto se actualiza, no tiene porqu� mantenerse
		features_peor = Pactual[pos_peor];
	}
}*/

/*
  Funci�n que aplica la BL del primer mejor al n�mero de soluciones indicadas previamente V2.

  Devuelve por referencia:
  Pactual -> Modificada con la BL primer mejor seg�n corresponda.
  features_mejor -> S�lo lo actualiza en el caso de que la prob_bl > 1
  features_peor -> S�lo lo actualiza en el caso de que la prob_bl > 1
*/
void aplicarBL_V2(BaseDatos &train, BaseDatos &testVal, vector<Features> &Pactual, Features &features_mejor, Features &features_peor, int &evaluaciones, double prob_bl)
{
	//Cuando la probabilidad es entre [0-1]
	if(prob_bl <= 1.0)
	{
		for(int i=0; i<Pactual.size(); i++)
		{
			double prob=1;
			if (prob_bl<1)
				prob = Rand();
			
			if(prob <= prob_bl)
			{
				LSfirst(train, testVal, Pactual[i].features, Pactual[i].fobjetivo, evaluaciones);
				Pactual[i].evaluada = 1;
			}
		}
	}
	else{ //prob_bl > 1 -> prob_bl*num_caracter�sticas = num_mejores
		populationEvaluation(train, testVal, Pactual, features_mejor, features_peor, evaluaciones);

		vector<E_dist> num_mejores;
		E_dist d;
		for(int n=0; n<Pactual.size(); n++)
		{			
			d.distance = Pactual[n].fobjetivo;
			d.pos = n;

			if(num_mejores.size()<prob_bl)
			{
				num_mejores.push_back(d);
				if(num_mejores.size()==prob_bl && prob_bl<Pactual.size())
					sort(num_mejores.begin(), num_mejores.end(), compare2);
			}
			else if(num_mejores[prob_bl-1].distance<d.distance)
			{
				num_mejores.pop_back();
				num_mejores.push_back(d);
				sort(num_mejores.begin(), num_mejores.end(), compare);
			}
		}

		//Aplicar la BL primer mejor
		for(int n=0; n<num_mejores.size(); n++)
		{
			int pos = num_mejores[n].pos;
			// Aplicar la BL primer mejor
			LSfirst(train, testVal, Pactual[pos].features, Pactual[pos].fobjetivo, evaluaciones);
			Pactual[pos].evaluada = 1;
		}
	}

	// Evaluaci�n para actualizar la mejor y peor de las caracter�sticas, ya que se supone que est�n ya evaluadas.
	populationEvaluation(train, testVal, Pactual, features_mejor, features_peor, evaluaciones);
}

/*
  Algoritmo Mem�tico:
	Algoritmo G�netico Generacional con Elitismo con BL.
  
  Los pasos sobre la poblaci�n (P) son los siguientes:
  1. Pactual(t) = {C1, ..., Cm}
  2. Selecci�n -> Ppadres={C'1, ..., C'm}, los C' son copias de los C
  3. Cruce -> Pintermedia={C''1, C'2, ..., C''m} con prob Pc
  4. Mutaci�n -> Phijos={H1=C'1, H2=C'2, ..., Hm=C''m} con prob Pm
  5. Reemplazamiento -> Pactual(t+1)={H1, H2, ..., Hm-1, Cmejor} con elitismo 
  (se mantiene el mejor de P(t)), en nuestro caso sustituir� completamente 
  la Pactual la Phijos. Es decir, Phijos pasar� a ser Pactual, lo cual se
  supone que es la versi�n "mejorada" de los padres, elitismo.
*/
void AM(BaseDatos &train, BaseDatos &testVal, vector<int> &sol_mejor, double &prob_c, double &prob_g, int cromosomas, int generaciones, double prob_bl, int k_torneo)
{
	vector<Features> Pactual, Ppadres, Pintermedia, Phijos;
	int genes=train.getFeatures(), evaluaciones=0, cont_generaciones=1;
	Features features_mejor, features_peor;

	// Inicializar la combinaci�n peor, auxiliar
	features_peor.fobjetivo = 101;

	// Inicializar la Poblaci�n actual
	generate_Poblationvector(Pactual, genes, cromosomas);

	// Evaluar la Poblaci�n actual generada
	populationEvaluation(train, testVal, Pactual, features_mejor, features_peor, evaluaciones);

	cout << "Algoritmo Memetico " << endl;
	cout << "Generaciones: " << generaciones << ", Probabilidad BL: " << prob_bl << endl;
	do
	{
		// Selecci�n de padres
		selection(train, testVal, Pactual, Ppadres, k_torneo, cromosomas, 1);

		// Cruze de los padres
		crossing(Ppadres, Pintermedia, prob_c, genes, cromosomas);

		// Mutaci�n
		uniformMutation(Pintermedia, Phijos, prob_g, genes, cromosomas);

		// Aplicar la BL primer mejor 
		if(cont_generaciones%generaciones==0)
			aplicarBL_V2(train, testVal, Phijos, features_mejor, features_peor, evaluaciones, prob_bl);

		// Reemplazamiento
		elitismReplacement(train, testVal, Phijos, Pactual, evaluaciones);

		// Evaluaci�n 
		populationEvaluation(train, testVal, Pactual, features_mejor, features_peor, evaluaciones);

		// Aumentar una generaci�n
		cont_generaciones++;

	}while(evaluaciones<NMAX);
	
	// Actualizar la mejor soluci�n encontrada
	sol_mejor.clear();
	for(int i=0; i<features_mejor.features.size(); i++)
		sol_mejor.push_back(features_mejor.features[i]);

}