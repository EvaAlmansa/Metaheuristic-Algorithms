#include "genetics.h"


/********************************************************************************/
//					Algortimos Genéticos -  Genetic Algorithm (GA)
/********************************************************************************/

/*
  Generación de la población inicial.
  Todos los cromosomas se generarán aleatoriamente, con un total de 50.
*/

void generate_Poblationvector(vector<Features> &Poblacion, int genes)
{
	Features C;

	Poblacion.clear();
	for(int i=0; i<genes; i++)
		C.features.push_back(0);

	for(int i=0; i<CROMOSOMAS; i++)
	{
		generate_InitialSolution(C.features, genes);
		Poblacion.push_back(C);
	}
}

/*
  Evaluación.
  Evaluar todas la población y devolver la mejor solución encontrada.

  Devuelve por referencia:
  features_mejor -> con la mejor solución encontrada.
  features_peor -> con la peor solución encontrada.
  evaluaciones -> incrementa la variable tantas veces como evaluaciones se hagan.
*/
void populationEvaluation(BaseDatos &train, BaseDatos &testVal, vector<Features> &Pactual, Features &features_mejor, Features &features_peor, int &evaluaciones)
{
	double fobj_generado, fob_mejor=-1, fobj_peor=101;
	bool mejor=false, peor=false;
	int pos_mejor=-1, pos_peor=-1;

	// Evaluar sólo aquellas combinaciones de características que no lo estén y comprobar si mejoran o empeoran las ya encontradas
	for(int i=0; i<Pactual.size(); i++)
	{
		if(!Pactual[i].evaluada)
		{
			train.EvaluationFunctionFeatures(Pactual[i], testVal);
			fobj_generado = Pactual[i].fobjetivo;
			if(fobj_generado>features_mejor.fobjetivo)
			{
				features_mejor.fobjetivo = fobj_generado;
				pos_mejor = i;
				mejor = true;
			}
			if(fobj_generado<features_peor.fobjetivo)
			{
				features_peor.fobjetivo = fobj_generado;
				pos_peor = i;
				peor = true;
			}
			Pactual[i].evaluada = true;
			evaluaciones++;
		}

		// Reinicializar en la población actual el mejor y peor
		Pactual[i].fobj_mejor = false;
		Pactual[i].fobj_peor = false;
	}

	// Actualizar la mejor solución global
	if(mejor)
	{		
		Pactual[pos_mejor].fobj_mejor = true;
		features_mejor = Pactual[pos_mejor];
	}else{
		// En otro caso, buscar la mejor solución de la población
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

	// Actualizar la peor solución global auxiliar
	if(peor)
	{
		Pactual[pos_peor].fobj_peor = true;
		features_peor = Pactual[pos_peor];
	}else{
		// En otro caso, buscar la mejor solución de la población
		for(int i=0; i<Pactual.size(); i++)
		{
			if(Pactual[i].fobjetivo<fobj_peor)
			{
				fobj_peor = Pactual[i].fobjetivo;
				pos_peor = i;
			}
		}
		Pactual[pos_peor].fobj_peor = true;

		// La solución peor es sólo auxiliar, por lo tanto se actualiza, no tiene porqué mantenerse
		features_peor = Pactual[pos_peor];
	}
}

/*
  Operador de selección.
  Se usa el torneo binario que consistente en elegir aleatoriamente dos individuos 
  de la población y seleccionar el mejor de ellos. En el esquema generacional, 
  se aplicarán tantos torneos como individuos existan en la población genética, 
  incluyendo los individuos ganadores en la población de padres. 
  En el esquema estacionario, se aplicará dos veces el torneo para elegir 
  los dos padres que serán posteriormente recombinados (cruzados).

  Parámetros:
  k_torneo -> indica el número de torneos que se realizarán.
  Ppadres -> Población devuelta por referencia, elegida por torneo.

  Parámetros por defecto:
  agGeneracional -> Se encuentra desactivada la selección generacional, 
    en la que hace tantas copias como indica el tamaño de la constante CROMOSOMAS.
    En otro caso, se generan la selección para el agoritmo génetico estacional.
*/
void selection(BaseDatos &train, BaseDatos &testVal, vector<Features> &Pactual, vector<Features> &Ppadres, int k_torneo, int agGeneracional=0)
{
	double fobj_mejor=-1, fobj_act;
	int ganador=0, torneos, valor;
	vector<int> seleccionados;

	Ppadres.clear();

	if(agGeneracional==1)
		torneos = CROMOSOMAS;
	else
		torneos=2;
	for (int i = 0; i < torneos; i++)	
	{
		int j=0;
		while(j<k_torneo)
		{
			valor = Randint(0, CROMOSOMAS-1);
			// No repetir seleccionado para el torneo
			if(!findElement(seleccionados, valor))
			{
				fobj_act = Pactual[valor].fobjetivo;
				if(fobj_act>fobj_mejor)
				{
					fobj_mejor = fobj_act;
					ganador = valor;
				}
				seleccionados.push_back(valor);
				j++;
			}
		}
		seleccionados.clear();

		Ppadres.push_back(Pactual[ganador]);
		fobj_mejor=-1;
	}
}

/*
  Operador de cruce.
  Se empleará el operador de cruce clásico en dos puntos, explicados en las 
  transparencias de teoría del tema de Algoritmos Genéticos y del Seminario 3. 
  Se escogerán aleatoriamente dos puntos de corte que determinan tres subcadenas. 
  Cada hijo contendrá la subcadena central de uno de los padres y las otras dos del otro padre.
*/
void crossing(vector<Features> &Ppadres, vector<Features> &Pintermedia, double prob_c, int genes)
{
	int parejas;
	int mom, dad, xpoint1, xpoint2;

	if(prob_c!=1)
		parejas = CROMOSOMAS*prob_c; 
	else 
		parejas=prob_c*Ppadres.size();

	// Se copia la población de los padres en la intermedia
	Pintermedia.clear();
	for(int i=0; i<Ppadres.size(); i++)
		Pintermedia.push_back(Ppadres[i]);

	// Se cruzan según una probabilidad
	for (mom=0; mom<parejas; mom+=2)
	{
		dad=mom+1;

		// Por cada uno de los hijos resultantes de cada padre

		// Se generan los dos puntos de cruce
		xpoint1= Randint (0, genes-1);
		if (xpoint1!=genes-1)
			xpoint2=Randint (xpoint1+1, genes-1);
		else
			xpoint2=genes-1;

		// Se cruza las partes contenidas entre ambos puntos 
		for (int i=xpoint1; i<=xpoint2; i++)
			Pintermedia[mom].features[i]=Ppadres[dad].features[i];

		// Se generan los dos puntos de cruce
		xpoint1= Randint (0, genes-1);
		if (xpoint1!=genes-1)
			xpoint2=Randint (xpoint1+1, genes-1);
		else
			xpoint2=genes-1;

		// Se cruza las partes contenidas entre ambos puntos 
		for (int i=xpoint1; i<=xpoint2; i++)
			Pintermedia[dad].features[i]=Ppadres[mom].features[i];

		// Se marcan los dos nuevos cromosomas para su posterior evaluacion 
		Pintermedia[mom].evaluada = false;
		Pintermedia[dad].evaluada = false;
   }
}

/*
  Operador de mutación.
  Una vez decidido que un cromosoma debe mutarse, se considerará el operador 
  de cambio de pertenencia Flip(s,i) que altera la selección de la 
  característica correspondiente al gen i del cromosoma s (pasa de 0 a 1 o
  viceversa), explicado en las transparencias del Seminario 2.a.

  Parámetro por defecto:
  estacional -> si esta a cero se generarán tantas posiciones como genes*cromosomas,
				en otro caso, sólo se generarán dos posiciones (para el caso de estacional)
*/
void uniformMutation(vector<Features> &Pintermedia, vector<Features> &Phijos, double prob_g, int genes, int estacional=0)
{
	int posiciones, i, j, Mu_next;
	double m;

	// Copia de la población intermedia en la población de los hijos
	Phijos.clear();
	for(int p=0; p<Pintermedia.size(); p++)
		Phijos.push_back(Pintermedia[p]);

	// Calculo del cromosoma en el que se realizara la primera mutacion y la posicion en la que se producirá
	// Ceil() -> redondea hacia arriba siempre que sea positivo (sino hacia abajo), sea cual sea el decimal devuelve el siguiente entero
	if (prob_g < 1.0) // Como es el 0.1 en prob_g, siempre se ejecuta la parte del if
		Mu_next = ceil(log(Rand()) / log(1.0 - prob_g)); 
	else
		Mu_next = 1;

	if(estacional==1)
		posiciones = genes*2;
	else
		posiciones=genes*CROMOSOMAS;
	 
	if (prob_g>0)
	{
		 while (Mu_next<posiciones)
		 {
			// Se determina el cromosoma y el gen que corresponden a la posicion que se va a mutar 
			i=Mu_next/genes;
			j=Mu_next%genes;

			// Se efectua la mutacion sobre ese gen 
			if (Phijos[i].features[j] == 0)
				Phijos[i].features[j] = 1;
			else
				Phijos[i].features[j] = 0;

			// Se marca el cromosoma mutado para su posterior evaluacion 
			Phijos[i].evaluada = false;

			// Se calcula la siguiente posicion a mutar 
			if (prob_g<1)
			{
				m = Rand();
				Mu_next += ceil (log(m) / log(1.0 - prob_g));
			}
			else
				Mu_next += 1;
		}
		 
		//Mu_next -= posiciones;
	}
}

/*
  Esquema de reemplazamiento.
  En el estacionario, los dos descendientes generados tras el cruce 
  y la mutación (esta última aplicada según una determinada probabilidad) 
  sustituyen a los dos peores de la población actual, en caso de ser mejores 
  que ellos.
*/
void seasonalReplacement(vector<Features> &Phijos, vector<Features> &Pactual)
{
	// Inicializo las dos peores soluciones de la población actual
	double fobj_peor1=Pactual[0].fobjetivo, fobj_peor2=Pactual[1].fobjetivo;
	int pos1=0, pos2=1;

	for(int i=2; i<Pactual.size(); i++)
	{
		if(Pactual[i].fobjetivo<fobj_peor1)
		{
			fobj_peor1=Pactual[i].fobjetivo;
			pos1 = i;
		}
		else if(Pactual[i].fobjetivo<fobj_peor2)
		{
			fobj_peor2=Pactual[i].fobjetivo;
			pos2 = i;
		}
	}

	// Los dos hijos sustituyen a los dos peores padres
	Pactual[pos1] = Phijos[0];
	Pactual[pos2] = Phijos[1];
}

/*
  Esquema de reemplazamiento.
  En el esquema generacional, la población de hijos sustituye automáticamente a 
  la actual. Para conservar el elitismo, si la mejor solución de la generación 
  anterior no sobrevive, sustituye directamente la peor solución de la nueva 
  población. 
*/
void elitismReplacement(vector<Features> &Phijos, vector<Features> &Pactual)
{
	int peor_hijo=-1;
	Features padre, hijo;
	double fobj_mejorpadre = -1, fobj_peorhijo = 101;

	// Inicializo las variables auxiliares
	for(int i=0; i<Pactual[0].features.size(); i++)
	{
		padre.features.push_back(0);
		hijo.features.push_back(0);
	}

	// Buscar el mejor padre, que ya esta etiquetado, y buscar el peor hijo,
	// que no esta actualizada la etiqueta
	for(int i=0; i<Pactual.size(); i++)
	{
		if(Pactual[i].fobj_mejor)
			padre = Pactual[i];
		if(Phijos[i].fobjetivo<fobj_peorhijo)
		{
			hijo = Phijos[i];
			peor_hijo = i;
			fobj_peorhijo = Phijos[i].fobjetivo;
		}
	}

	Pactual = Phijos;
	if(!findFeatures(Pactual, padre))
		Pactual[peor_hijo] = padre;
}

/*
  Encontrar coincidencias de un conjunto de características en un vector de soluciones.

  Devuelve:
  bool -> si ha encontrado coincidencias devuelve true en otro caso falso
*/
bool findFeatures(vector<Features> &poblacion, Features &features)
{
	for(int i=0; i<poblacion.size(); i++)
	{
		for(int j=0; j<features.features.size(); j++)
		{
			if(poblacion[i].features[j]!=features.features[j])
				break;
			if(j==features.features.size()-1)
				return true;
		}
	}

	return false;
}

/*
  Encontrar elemento.
  
  Devuelve true si ha encontrado alguna coincidencia.
*/
bool findElement(vector<int> v, int elemento)
{
	for(int i=0; i<v.size(); i++)
	{
		if(elemento==v[i])
			return true;
	}
	return false;
}


/*
  Algoritmo Génetico Estacionario.

  Los pasos sobre la población (P) son los siguientes:
  1. Pactual(t) = {C1, ..., Cm}
  2. Selección -> Dos cromosomas, Ppadres={C'1, C'2}, los C' son copias de los C
  3. Cruce -> Pintermedia={C''1, C''2} con prob 1 (siempre se cruzan),
  para ello, se coge una parte de cada uno de los padres de forma aleatoria. Centro 
  y laterales, son las partes a selecionar.
  4. Mutación -> Phijos={H1=C'1, H2=C''2} con prob Pm 
  5. Reemplazamiento -> Los dos hijos compiten para entrar en P(t),
  Pactual(t+1)={C1, H1, ..., Cm}.

  Parámetro por defecto:
  k_torneo -> indica el número de soluciones que compiten por ser padre en la 
			selección. Por defecto está a dos, por ser estacional.
*/
void AGE(BaseDatos &train, BaseDatos &testVal, vector<int> &sol_mejor, double &prob_c, double &prob_g, int k_torneo)
{
	vector<Features> Pactual, Ppadres, Pintermedia, Phijos;
	int genes=train.getFeatures(), evaluaciones=0;
	Features features_mejor, features_peor;

	clock_t start;
	double duration;

	// Inicializar la combinación peor, auxiliar
	features_peor.fobjetivo = 101;

	// Inicializar la Población actual
	generate_Poblationvector(Pactual, genes);

	// Evaluar la Población actual generada
	populationEvaluation(train, testVal, Pactual, features_mejor, features_peor, evaluaciones);

	cout << "Algoritmo Genetico estacionario " << endl;
	do
	{

		// Selección de padres
		//start = clock();
		selection(train, testVal, Pactual, Ppadres, k_torneo);
		//duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;

		//cout << "Selection: " << duration << " seg, ";

		// Cruze de los padres
		//start = clock();
		crossing(Ppadres, Pintermedia, prob_c, genes);
		//duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;

		//cout << "Crossing: " << duration << " seg, ";

		// Mutación
		// Selección de padres
		//start = clock();
		uniformMutation(Pintermedia, Phijos, prob_g, genes, 1);
		//duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;

		//cout << "Mutacion: " << duration << " seg, ";

		// Reemplazamiento
		//start = clock();
		seasonalReplacement(Phijos, Pactual);
		//duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;

		//cout << "Reemplazamiento: " << duration << " seg, ";

		// Evaluar la población actual
		//start = clock();
		populationEvaluation(train, testVal, Pactual, features_mejor, features_peor, evaluaciones);
		//duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;

		//cout << "Evaluacion: " << duration << " seg, ";

		//cout << endl;
		//cout << "Evaluacion numero: " << evaluaciones <<  ", ";
	}while(evaluaciones<NMAX);

	// Actualizar la mejor solución encontrada
	sol_mejor.clear();
	for(int i=0; i<features_mejor.features.size(); i++)
		sol_mejor.push_back(features_mejor.features[i]);

	cout << "...Done" << endl;
}

/*
  Algoritmo Génetico Generacional con Elitismo.
  
  Los pasos sobre la población (P) son los siguientes:
  1. Pactual(t) = {C1, ..., Cm}
  2. Selección -> Ppadres={C'1, ..., C'm}, los C' son copias de los C
  3. Cruce -> Pintermedia={C''1, C'2, ..., C''m} con prob Pc
  4. Mutación -> Phijos={H1=C'1, H2=C'2, ..., Hm=C''m} con prob Pm
  5. Reemplazamiento -> Pactual(t+1)={H1, H2, ..., Hm-1, Cmejor} con elitismo 
  (se mantiene el mejor de P(t)), en nuestro caso sustituirá completamente 
  la Pactual la Phijos. Es decir, Phijos pasará a ser Pactual, lo cual se
  supone que es la versión "mejorada" de los padres, elitismo.
*/
void AGG(BaseDatos &train, BaseDatos &testVal, vector<int> &sol_mejor, double &prob_c, double &prob_g, int k_torneo)
{
	vector<Features> Pactual, Ppadres, Pintermedia, Phijos;
	int genes=train.getFeatures(), evaluaciones=0;
	Features features_mejor, features_peor;

	clock_t start;
	double duration;

	// Inicializar la combinación peor, auxiliar
	features_peor.fobjetivo = 101;

	// Inicializar la Población actual
	generate_Poblationvector(Pactual, genes);

	// Evaluar la Población actual generada
	populationEvaluation(train, testVal, Pactual, features_mejor, features_peor, evaluaciones);

	cout << "Algoritmo Genetico Generacional con elitismo " << endl;
	do
	{
		// Selección de padres
		//start = clock();
		selection(train, testVal, Pactual, Ppadres, k_torneo, 1);
		//duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;

		//cout << "Selection: " << duration << " seg, ";

		// Cruze de los padres
		//start = clock();
		crossing(Ppadres, Pintermedia, prob_c, genes);
		//duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;

		// Mutación
		// Selección de padres
		//start = clock();
		uniformMutation(Pintermedia, Phijos, prob_g, genes, 1);
		//duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;

		//cout << "Mutacion: " << duration << " seg, ";

		// Reemplazamiento
		//start = clock();
		elitismReplacement(Phijos, Pactual);
		//duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;

		//cout << "Reemplazamiento: " << duration << " seg, ";

		// Evaluación 
		//start = clock();
		populationEvaluation(train, testVal, Pactual, features_mejor, features_peor, evaluaciones);
		//duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;

		//cout << "Evaluacion poblacion: " << duration << " seg, ";

		//cout << endl;
		//cout << "Evaluacion: " << evaluaciones <<  ", ";
	}while(evaluaciones<NMAX);
	
	// Actualizar la mejor solución encontrada
	sol_mejor.clear();
	for(int i=0; i<features_mejor.features.size(); i++)
		sol_mejor.push_back(features_mejor.features[i]);

	cout << "...Done" << endl;
}
