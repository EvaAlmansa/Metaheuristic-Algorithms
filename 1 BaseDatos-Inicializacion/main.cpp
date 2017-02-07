//Algoritmos de búsqueda
#include "AM.h"

//Algoritmos de búsqueda para comparar tiempos
#include "sequentialforwardselection.h"

#include <ctime>

/*
  Función que obtiene un mínimo aleatorio entre [0, size_bd].
  Y calcula el máximo teniendo en cuenta que ha de ser el
  50% de datos para la BD, por tanto calcula la mitad de
  size_bd, para asignar el máximo según marque el mínimo donde
  corresponda.

  Devuelve por referencia: min, max
*/
void randomMitad(int size_bd, int &min, int &max)
{
	min = Randint(0, size_bd-1);
	if((min+size_bd)/2>size_bd)
		max = size_bd/2 - (size_bd-min);
	else if((min+size_bd/2)==size_bd)
			max = 0;
	else 
		max = min + size_bd/2;
}

/*
  Función que obtiene un mínimo y el máximo, a partir del
  mínimo y máximo anterior. Además, tiene en cuenta para el 
  máximo que ha de ser la mitad de size_bd.

  Devuelve por referencia: min, max
*/
void restoMitad(int size_bd, int &min, int &max)
{
	if(min<max)
		min = max;
	if((min+size_bd/2)>size_bd)
		max = size_bd/2 - (size_bd-min);
	else if((min+size_bd/2)==size_bd)
		max = 0;
	else 
		max = min+size_bd/2;
	//Si es par
	if((size_bd%2) == 0)
	{
		if(max==size_bd)
			max=0;
		//else
			//max--;
	}else
	{
		if((min+size_bd/2)>size_bd)
			max = size_bd/2 - (size_bd-min);
		else if((min+size_bd/2)==size_bd)
			max = 0;
		else 
			max = min+size_bd/2;
	}
} 

/*
  Función que copia una nueva BaseDatos a partir de "otra", con un
  "nombre" y con un min que marca el inicio desde donde empieza 
  y un max hasta donde acaba la copia. 

  Parámetros: BaseDatos, char* nombre, int min, int max
  Devuelve: BaseDatos
*/
BaseDatos obtenerMitad(BaseDatos &otra, char *nombre, int min, int max)
{
	BaseDatos nueva(otra, min, max, nombre);

	return nueva;
}

/*
  Función que reduce una BD dada con el tope que se indica en el 
  parámetro. También se puede personalizar poniéndole un nombre...
*/
BaseDatos obtenerBD_reducida(BaseDatos &otra, int tope, char *nombre)
{
	BaseDatos nueva(otra, tope, nombre);

	return nueva;
}
/*
  Función que muestra la salida por pantalla (consola) de la mejor solución encontrada.
*/
void mostrarResultados(vector<int> &sol_mejor, BaseDatos &train, BaseDatos &test, double &duration)
{
	int predictores_select = 0;
	//cout << "Variables seleccionadas: " << endl;
	for(int i=0; i<sol_mejor.size(); i++)
	{
		if(sol_mejor[i]==1)
		{
			cout << train.getNamePredictor(i) << ", ";
			predictores_select++;
		}
	}
	cout << endl;
	cout << "Con un total de variables: " << predictores_select << "." << endl;
	cout << "Porcentaje de tasa_clas: " << train.EvaluationFunction(sol_mejor, test, 1) << "%." << endl;
	cout << "Porcentaje de tasa_red: " << 100*((train.getFeatures()*1.0-predictores_select*1.0)/train.getFeatures()*1.0) << "%." << endl;
	cout << "Total tiempo de CPU: " << duration << " segundos." << endl;
}

/*********************************/

int main(int argc, char *argv[])
{
	BaseDatos datos, train, test;
	int size_bd, min, max, options;
	unsigned long semilla=111;
	vector<int> sol_mejor;

	char bd[50];
	string ruta;

	clock_t start;
	double duration, prob_c, prob_g, prob_bl; //Tres últimas variables, probalidades AM
	int cromosomas, generaciones;

	/********************************************************************************/
	//					Lectura de parámetros por programa
	/********************************************************************************/

	if (argc==1)
	{
		//
        cout << "Menu - Elija una opcion teniendo en cuenta el numero que le precede a cada una:" << endl;
	    cout << "1) Ozone-Balanceado" << endl;
		cout << "2) Sonar" << endl;
		cout << "3) Spambase-460" << endl;
		cout << "Por favor elija una opcion: ";
		cin >> options; 
		
		if(options == 1)
		{
			cout << "Ozone-Balanceado" << endl;
			datos.createBD("/instancias/ozone-balanceado.arff");
		}
		else if(options == 2)
		{
			cout << "Sonar" << endl;
			datos.createBD("/instancias/sonar.arff");
		}
		else if(options == 3)
		{
			cout << "Spambase-460" << endl;
			datos.createBD("/instancias/spambase-460.arff");
		}else
		{
			cout << "ERROR: Opcion invalida.\n" << endl;
			cout << "Por favor vuelva a ejecutar el programa." << endl;
			return 1;
		}

		cout << "4) Ejecutar solo una vez cada algoritmo." << endl;
		cout << "5) Ejecutar por cada algoritmo el Cross 5x2" << endl;
		cout << "Por favor elija una opcion: ";
		cin >> options; 

		if(options!=4 && options !=5)
		{
			cout << "ERROR: Opcion invalida.\n" << endl;
			cout << "Por favor vuelva a ejecutar el programa." << endl;
			return 1;
		}
	}else if(argc==3)
	{
		// Se almacena el primer parametro que se corresponde con la base de datos
		ruta = argv[1];
		strncpy(bd, ruta.c_str(), sizeof(bd));
		if((strcmp (bd,"/instancias/ozone-balanceado.arff") != 0) && (strcmp (bd,"/instancias/sonar.arff") != 0) && (strcmp (bd,"/instancias/spambase-460.arff") != 0))
		{
			cout << "ERROR: Base de datos inexistente.\n" << endl;
			cout << "Uso: main.exe <Base Datos> \n" << endl;
			cout << "Bases Datos: ozone-balanceado.arff sonar.arff spambase-460.arff \n" << endl;
			return 1;
		}else
		{
			cout << ruta << endl;
			datos.createBD(ruta);
		}

		options = atoi(argv[2]);

		if(options!=4 && options !=5)
		{
			cout << "ERROR: Opcion invalida.\n" << endl;
			cout << "Por favor vuelva a ejecutar el programa." << endl;
			return 1;
		}
	}else
	{
		/**/
		cout << "ERROR: Numero de parametros erroneos.\n" << endl;
		cout << "Tiene dos opciones:\n" << endl;
		cout << "1. Sin ningun parametro, se abrira el menu. Ejecutar: main.exe\n" << endl;
		cout << "2. Ejecutar: main.exe <Base Datos> <4 o 5> \n" << endl;
		cout << "Por favor vuelva a ejecutar el programa." << endl;
		return 1;

		/*
		  Pruebas
		* /
		cout << "Ozone-balanceado" << endl;
		datos.createBD("/instancias/ozone-balanceado.arff");
		//cout << "Sonar" << endl;
		//datos.createBD("/instancias/sonar.arff");
		//cout << "Spambase-460" << endl;
		//datos.createBD("/instancias/spambase-460.arff");
		options = 4;
		/******/
	}
	
	
	//datos.createBD(ruta);
	//datos.printBD();
	size_bd = datos.getSizeBD();

	if(options == 4)
	{
		/****************************************************************************************************************************/
		//	
		// 
		//										EJECUCIÓN DE CADA UNO DE LOS ALGORITMOS A LA VEZ
		//
		//
		/***************************************************************************************************************************/
		semilla=111;
		Set_random (semilla);
		/** /
		// Forma sesgada de partir" la base de datos en el 50% para cada uno de los subconjuntos
		randomMitad(size_bd, min, max);
		cout << "Train --> Min: " << min << ", Max-1: " << max << endl;
		train = obtenerMitad(datos, "train", min, max);

		restoMitad(size_bd, min, max);
		cout << "Test --> Min: " << min << ", Max-1: " << max << endl;
		
		test = obtenerMitad(datos, "test", min, max);
		/**/
		
		// Crear los subconjuntos de train y test
		datos.sinSesgo(train, test);
		cout << "Train --> size: " << train.getSizeBD() << endl;
		cout << "Test --> size: " << test.getSizeBD() << endl;

		// 3 Nearest Neighbors (3-nn) ----------------------------------------------------
		/**/
		datos.Knn(sol_mejor);
		start = clock();
		train.EvaluationFunction(sol_mejor, test);
		duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
		mostrarResultados(sol_mejor, train, test, duration);
		/**/

		// Solución Greedy: Sequential Forward Selection (SFS) ----------------------------------
		/**/
		start = clock();
		SFS(train, train, sol_mejor);
		duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
		mostrarResultados(sol_mejor, train, test, duration);
		/**/

		// Variables para las tres pruebas de AM -----------------------------------------------
		//Probabilidad de gen muy baja no permite mucha divergencia en las soluciones
		prob_c = 0.7;
		prob_g = 0.001;
		cromosomas = 10;
		generaciones = 10;

		// Algoritmo Memético - A. Génetico Generacional con Elitismo y BL, AM(10, 1.0)-----------------------------------------
		/**/
		prob_bl = 1.0;
		start = clock();
		AM(train, train, sol_mejor, prob_c, prob_g, cromosomas, generaciones, prob_bl);
		duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
		mostrarResultados(sol_mejor, train, test, duration);

		// Algoritmo Memético - A. Génetico Generacional con Elitismo y BL, AM(10, 0.1)-----------------------------------------
		/**/
		prob_bl = 0.1;
		start = clock();
		AM(train, train, sol_mejor, prob_c, prob_g, cromosomas, generaciones, prob_bl);
		duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
		mostrarResultados(sol_mejor, train, test, duration);

		// Algoritmo Memético - A. Génetico Generacional con Elitismo y BL, AM(10, 1.0mejor)-----------------------------------------
		/**/
		prob_bl = 1*cromosomas;
		start = clock();
		AM(train, train, sol_mejor, prob_c, prob_g, cromosomas, generaciones, prob_bl);
		duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
		mostrarResultados(sol_mejor, train, test, duration);

		/***************************************************************************************************************************/

	}else {
		/****************************************************************************************************************************/
		//	
		// 
		//											CROSS 5X2
		//
		//
		/***************************************************************************************************************************/
		
		// Variables para las tres pruebas de AM

		prob_c = 0.7;
		prob_g = 0.001;
		cromosomas = 10;
		generaciones = 10;

		/********************************************************************************/
		//				K vecinos más cercanos - K Nearest Neigboors (K-nn)
		/********************************************************************************/
		semilla=111;
		datos.Knn(sol_mejor);
		for(int i=0; i<5; i++)
		{
			Set_random (semilla);
			cout << "Ejecucion: " << i+1 << "/5, con semilla: " << semilla << endl;
			// Crear los subconjuntos de train y test
			datos.sinSesgo(train, test);
			cout << "Train --> size: " << train.getSizeBD() << endl;
			cout << "Test --> size: " << test.getSizeBD() << endl;
	
			start = clock();
			train.EvaluationFunction(sol_mejor, test);
			duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
			mostrarResultados(sol_mejor, train, test, duration);

			start = clock();
			test.EvaluationFunction(sol_mejor, train);
			duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
			mostrarResultados(sol_mejor, test, train, duration);

			semilla *= 5;
		}

		/********************************************************************************/
		//					Solución Greedy: Sequential Forward Selection (SFS)
		/********************************************************************************/
		semilla=111;
		for(int i=0; i<5; i++)
		{
			Set_random (semilla);
			cout << "Ejecucion: " << i+1 << "/5, con semilla: " << semilla << endl;
			// Crear los subconjuntos de train y test
			datos.sinSesgo(train, test);
			cout << "Train --> size: " << train.getSizeBD() << endl;
			cout << "Test --> size: " << test.getSizeBD() << endl;
	
			start = clock();
			SFS(train, train, sol_mejor);
			duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;

			mostrarResultados(sol_mejor, train, test, duration);

			start = clock();
			SFS(test, test, sol_mejor);
			duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
			mostrarResultados(sol_mejor, test, train, duration);

			semilla *= 5;
		}

		/********************************************************************************/
		//  Algoritmo Memético - A. Génetico Generacional con Elitismo y BL, AM(10, 1.0)
		/********************************************************************************/

		prob_bl = 1.0;
		semilla=111;
		for(int i=0; i<5; i++)
		{
			Set_random (semilla);
			cout << "Ejecucion: " << i+1 << "/5, con semilla: " << semilla << endl;
			// Crear los subconjuntos de train y test
			datos.sinSesgo(train, test);
			cout << "Train --> size: " << train.getSizeBD() << endl;
			cout << "Test --> size: " << test.getSizeBD() << endl;
	
			start = clock();
			AM(train, train, sol_mejor, prob_c, prob_g, cromosomas, generaciones, prob_bl);
			duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;

			mostrarResultados(sol_mejor, train, test, duration);
			
			start = clock();
			AM(train, train, sol_mejor, prob_c, prob_g, cromosomas, generaciones, prob_bl);
			duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;

			mostrarResultados(sol_mejor, test, train, duration);

			semilla *= 5;
		}

		/********************************************************************************/
		//	Algoritmo Memético - A. Génetico Generacional con Elitismo y BL, AM(10, 0.1) 
		/********************************************************************************/

		prob_bl = 0.1;
		semilla=111;
		for(int i=0; i<5; i++)
		{
			Set_random (semilla);
			cout << "Ejecucion: " << i+1 << "/5, con semilla: " << semilla << endl;
			// Crear los subconjuntos de train y test
			datos.sinSesgo(train, test);
			cout << "Train --> size: " << train.getSizeBD() << endl;
			cout << "Test --> size: " << test.getSizeBD() << endl;
	
			start = clock();
			AM(train, train, sol_mejor, prob_c, prob_g, cromosomas, generaciones, prob_bl);
			duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;

			mostrarResultados(sol_mejor, train, test, duration);

			start = clock();
			AM(train, train, sol_mejor, prob_c, prob_g, cromosomas, generaciones, prob_bl);
			duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;

			mostrarResultados(sol_mejor, test, train, duration);

			semilla *= 5;
		}

		/********************************************************************************/
		//Algoritmo Memético - A. Génetico Generacional con Elitismo y BL, AM(10, 1.0mejor)
		/********************************************************************************/

		prob_bl = 1*cromosomas;
		semilla=111;
		for(int i=0; i<5; i++)
		{
			Set_random (semilla);
			cout << "Ejecucion: " << i+1 << "/5, con semilla: " << semilla << endl;
			// Crear los subconjuntos de train y test
			datos.sinSesgo(train, test);
			cout << "Train --> size: " << train.getSizeBD() << endl;
			cout << "Test --> size: " << test.getSizeBD() << endl;
	
			start = clock();
			AM(train, train, sol_mejor, prob_c, prob_g, cromosomas, generaciones, prob_bl);
			duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;

			mostrarResultados(sol_mejor, train, test, duration);

			start = clock();
			AM(train, train, sol_mejor, prob_c, prob_g, cromosomas, generaciones, prob_bl);
			duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;

			mostrarResultados(sol_mejor, test, train, duration);

			semilla *= 5;
		}

		/********************************************************************************/
	}
	
	return 0;
}