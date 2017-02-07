#include "basedatos.h"

/*
  Funci�n que compara dos distancias para conseguir que la menor distancia 
  se encuentre en primer lugar.
*/
bool compare(const E_dist &a, const E_dist &b)
{
	return a.distance < b.distance;
}

/*
  Funci�n que compara dos distancias para conseguir que la mayor distancia 
  se encuentre en primer lugar.
*/
bool compare2(const E_dist &a, const E_dist &b)
{
	return a.distance > b.distance;
}

/********************************************************************************/
//					Constructor/Destructor - Inicializar
/********************************************************************************/
BaseDatos::BaseDatos()
{
	//num_BD = 0;
}

//Constructor de copia, donde el par�metro max no se incluye.
BaseDatos::BaseDatos(BaseDatos &otra, int min, int max, char *nombre_bd)
{
	bool crear_vector = true, crear_clasificador = false;
	vector<double> v_num;

	setName(nombre_bd); 
	setClassificator(otra.getNameClassificator());

	if(max<min)
	{
		//Con cada una de las caracter�sticas
		for(int i=0; i<otra.getFeatures(); i++)
		{
			crear_vector = true;
			//Add desde el inicio hasta max-1
			for(int j=0; j<max; j++)
			{
				if(crear_vector)
				{
					addPredictor(otra.getNamePredictor(i));
					v_num.push_back(otra.getValuePredictor(i, j));
					predictores.value.push_back(v_num);
					v_num.clear();
					crear_vector = false;
				}else
					predictores.value[i].push_back(otra.getValuePredictor(i, j));
				if(!crear_clasificador)
					clasificador.value.push_back(otra.getValueClassificator(j));
			}
			crear_clasificador = true;
		}

		crear_clasificador = false;

		for(int i=0; i<otra.getFeatures(); i++)
		{
			if(max==0)
				crear_vector = true;
			//Para finalmente add desde min hasta el final
			for(int j=min; j<otra.getSizeBD(); j++)
			{
				if(crear_vector)
				{
					addPredictor(otra.getNamePredictor(i));
					v_num.push_back(otra.getValuePredictor(i, j));
					predictores.value.push_back(v_num);
					v_num.clear();
					crear_vector = false;
				}else
					predictores.value[i].push_back(otra.getValuePredictor(i, j));
				if(!crear_clasificador)
					clasificador.value.push_back(otra.getValueClassificator(j));
			}
			crear_clasificador = true;
		}
	}else{
		for(int i=0; i<otra.getFeatures(); i++)
		{
			crear_vector = true;
			//Add desde min hasta max-1
			for(int j=min; j<max; j++)
			{
				if(crear_vector)
				{
					addPredictor(otra.getNamePredictor(i));
					v_num.push_back(otra.getValuePredictor(i, j));
					predictores.value.push_back(v_num);
					v_num.clear();
					crear_vector = false;
				}else
					predictores.value[i].push_back(otra.getValuePredictor(i, j));
				if(!crear_clasificador)
					clasificador.value.push_back(otra.getValueClassificator(j));
			}
			crear_clasificador = true;
		}
	}
}

/*
  Constructor de copia, dado una BD demasiado grande, hacer m�s peque�a 
  indicada en el par�metro tope.
  Adem�s normaliza los datos.
*/
BaseDatos::BaseDatos(BaseDatos &otra, int tope, char *nombre_bd){
	bool crear_vector = true, crear_classificador = false;
	vector<double> v_num;
	vector<int> posiciones;

	setName(nombre_bd); 
	setClassificator(otra.getNameClassificator());

	//Generar de forma aleatoria cada uno de los datos a escoger
	while(posiciones.size()<tope)
	{
		int max = otra.getSizeBD()-1;
		int dato = Randint(0, max);
		if(find(posiciones.begin(), posiciones.end(), dato) == posiciones.end())
			posiciones.push_back(dato);
	}

	//Con cada una de las caracter�sticas
	for(int i=0; i<otra.getFeatures(); i++)
	{
		crear_vector = true;
		//Add desde el inicio hasta tope
		for(int j=0; j<tope; j++)
		{
			
			if(crear_vector)
			{
				addPredictor(otra.getNamePredictor(i));
				v_num.push_back(otra.getValuePredictor(i, posiciones[j]));
				predictores.value.push_back(v_num);
				v_num.clear();
				crear_vector = false;
			}else
				predictores.value[i].push_back(otra.getValuePredictor(i, posiciones[j]));
			if(!crear_classificador)
				clasificador.value.push_back(otra.getValueClassificator(posiciones[j]));
		}
		crear_classificador = true;
	}
	normalizeBD();
}

BaseDatos::~BaseDatos()
{
}

/*
  Leer, crear la BD y normalizar los datos entre [0, 1].

  Par�metro por defecto:
  reducir=0 --> si es distinto de cero, evita normalizar la BD de datos grande.
*/
void BaseDatos::createBD(string ruta, int reducir)
{
	openFile(ruta);
	if(reducir==0)
		normalizeBD();
}

/*
  Funci�n sin sesgo, a partir de una base de datos dada
  asignar el 50% a train y el otro 50% a test.
  Para cada fila de la base de datos asigna a train o a test
  seg�n una probabilidad, menor que 0.5 y mayor igual que 0.5, 
  respectivamente.

  Devuelve por referencia:
  train -> base de datos con la mitad de la original.
  test -> base de datos con la otra mitad de la original.
*/
void BaseDatos::sinSesgo(BaseDatos &trainBD, BaseDatos &testBD)
{
	int tamBD = getSizeBD();
	int halfsize = tamBD/2;
	bool crear_vector = true, crear_clasificador=true;
	vector<double> v_num;
	vector<int> seleccion_train, seleccion_test;
	double numero;

	// Seleccionar aleatoriamente el 50% de las filas de la base de datos
	for(int i=0; i<tamBD; i++)
	{
		double prob = Rand();
		if(prob<0.5 && seleccion_train.size()<halfsize)
			seleccion_train.push_back(i);
		else if(seleccion_test.size()<halfsize)
			seleccion_test.push_back(i);
		else 
			seleccion_train.push_back(i);
	}

	// Inicializar/Limpiar los datos de train y test
	trainBD.clasificador.value.clear();
	testBD.clasificador.value.clear();
	trainBD.predictores.name.clear();
	testBD.predictores.name.clear();
	trainBD.predictores.value.clear();
	testBD.predictores.value.clear();

	// Para toda fila de la base de datos
	for (int f=0; f<getFeatures(); f++)
	{
		if(crear_clasificador)
		{
			trainBD.setClassificator(getNameClassificator());
			testBD.setClassificator(getNameClassificator());
		}

		trainBD.addPredictor(getNamePredictor(f));
		testBD.addPredictor(getNamePredictor(f));

		crear_vector = true;
		for(int i=0; i<seleccion_train.size(); i++)
		{
			if(crear_vector)
			{
				// Obtener el dato de la caracter�stica y posici�n seleccionada
				v_num.push_back(getValuePredictor(f,seleccion_train[i]));
				trainBD.predictores.value.push_back(v_num);
				v_num.clear();
			}else{
				numero = getValuePredictor(f,seleccion_train[i]);
				trainBD.predictores.value[f].push_back(numero);
			}
			// El clasificador s�lo contiene un vector
			if(crear_clasificador)
				trainBD.clasificador.value.push_back(getValueClassificator(seleccion_train[i]));
			
			crear_vector=false;	
		}

		crear_vector = true;
		for(int i=0; i<seleccion_test.size(); i++)
		{
			if(crear_vector)
			{
				// Obtener el dato de la caracter�stica y posici�n seleccionada
				v_num.push_back(getValuePredictor(f,seleccion_test[i]));
				testBD.predictores.value.push_back(v_num);
				v_num.clear();
			}else{
				numero = getValuePredictor(f,seleccion_test[i]);
				testBD.predictores.value[f].push_back(numero);
			}
			// El clasificador s�lo contiene un vector
			if(crear_clasificador)
				testBD.clasificador.value.push_back(getValueClassificator(seleccion_test[i]));

			crear_vector=false;	
		}

		crear_clasificador = false;
	}
}

/********************************************************************************/
//					Set Functions
/********************************************************************************/
//Add the  name of the BD
void BaseDatos::setName(char *n)
{
	if(strlen(n)>0)
		strcpy(name, n);
	else{
		cout << "Error: No esta a�adiendo nombre a la Base de Datos" << endl;
		return;
	}
}

//Add the name of the predictor. 
void BaseDatos::addPredictor(char *n)
{
	char nombre[50];
	if(strlen(n)>0)
	{
		strcpy(nombre, n);
		string str(nombre);
		predictores.name.push_back(str);
	}
	else{
		cout << "Error: No esta a�adiendo nombre a la variable predictor" << endl;
		return;
	}
}

//Add the name of the predictor. 
void BaseDatos::addPredictor(string n)
{
	if(n.size()>0)
		predictores.name.push_back(n);
	else{
		cout << "Error: No esta a�adiendo nombre a la variable predictor" << endl;
		return;
	}
}

//Add the name of the only clasificator.
void BaseDatos::setClassificator(char *n)
{
	if(strlen(n)>0)
	{
		strcpy(clasificador.name, n);
		//cout << "Add the name of the clasificator: " << clasificador.name << endl;
	}
	else{
		cout << "Error: No esta a�adiendo nombre a la variable clasificador." << endl;
		return;
	}
}

/********************************************************************************/
//					Get Functions
/********************************************************************************/

/*
  Obtener el nombre de la base de datos.
*/
char* BaseDatos::getName()
{
	return name;
}

/*
  Obtener el nombre del predictor, seg�n indique la posici�n
  en el par�metro de entrada pos.
*/
string BaseDatos::getNamePredictor(int pos)
{
	return predictores.name[pos];
}

/*
  Obtener el nombre de la caracter�stica clasificador.
*/
char* BaseDatos::getNameClassificator()
{
	return clasificador.name;
}

/*
  Obtener el valor/dato que pertenece a la caracter�stica y posici�n
  indicada por par�metro.

  Par�metros de entrada:
  var -> Posici�n de la caracter�stica, el n�mero que le corresponda
		 en la columna de la base de datos.
  pos -> Posici�n del dato correspondiente
*/
double BaseDatos::getValuePredictor(int var, int pos)
{
	return predictores.value[var][pos];
}

string BaseDatos::getValueClassificator(int pos)
{
	return clasificador.value[pos];
}

int BaseDatos::getSizeBD()
{
	return predictores.value[0].size();
}

int BaseDatos::getFeatures()
{
	return predictores.name.size();
}

/*
  Muestra por pantalla la BD que se encuentra en memoria.
*/
void BaseDatos::printBD()
{
	cout << "Nombre BD: " << name << endl;
	cout << "Atributos: " << endl;
	for(int i=0; i<predictores.name.size(); i++)
	{
		cout << getNamePredictor(i) << "  ";
	}
	cout << clasificador.name << endl;

	cout << "Datos: " << endl;
	for(int i=0; i<predictores.value.size(); i++)
	{
		for(int j=0; j<predictores.value[i].size(); j++)
		{
			cout << getValuePredictor(i, j) << "  ";
		}
		//cout << getValuePredictor(i, 0) << "  ";
		cout << endl;
	}

	for(int i=0; i<clasificador.value.size(); i++)
	{
		cout << getValueClassificator(i) << "  ";
	}
}

/********************************************************************************/
//					Operadores sobre los datos
/********************************************************************************/

/*
  B�squeda del m�nimo.
*/
double BaseDatos::min(vector<double> &s, int ini, int fin)
{
    double menor, dev;
    
    if(ini == fin)
    {
        menor = s[ini];
		dev = menor;
    }
    else{
        menor = min(s, ini + 1, fin);
        if(s[ini] < menor)
            dev = s[ini];
        else
            dev = menor;
    }
    return(dev);
}

/*
  B�squeda del m�ximo.
*/
double BaseDatos::max(vector<double> &s, int ini, int fin)
{
    double mayor, dev;
    
    if(ini == fin)
    {
        mayor = s[ini];
		dev = mayor;
    }
    else{
        mayor = max(s, ini + 1, fin);
        if(s[ini] > mayor)
            dev = s[ini];
        else
            dev = mayor;
    }
    return(dev);
}

/*
  Distancia Eucl�dea:
  d(xi,xj) = sqrt((xi1 - xj1)^2+(xi2 - xj2)^2+...+(xin - xjn)^2)
  Tal que i <> j y n = |X| (n = al n�mero total de atributos
  sin contar la variable de clasificaci�n).

  Par�metros:
  features -> soluci�n de caracter�sticas seleccionadas
  x1 -> vector del dato a comparar
  pos -> posici�n del dato en la base de datos a comparar
*/
//double BaseDatos::Euclideandistance(vector<int> &features, vector<double> &x1, int pos_x2)
double BaseDatos::Euclideandistance(vector<int> &features, BaseDatos &test, int pos_x1, int pos_x2)
{
	double distancia = 0.0;
	for(int i=0; i<features.size(); i++)
	{
		if(features[i]==1)
		{	
			double diferencia = test.getValuePredictor(i, pos_x1) - getValuePredictor(i, pos_x2);
			distancia += diferencia*diferencia;
		}
	}

	return (sqrt(distancia));
}

/*
  Funci�n Evaluaci�n:
  1.Rendimiento promedio de un clasificador 3-NN (K=3 vecinos)
  2.tasa_clas -> mide el porcentaje de instancias correctamente
  clasificadas pertenecientes a T:
  tasa_clas = 100*((n� instancias bien clasificadas de T)/(n� instancias en T))
  3.El objetivo es obtener el subconjunto de caracter�sticas que
  maximiza esta funci�n.

  Par�metros:
  F -> caracter�sticas a evaluar
  test -> base de datos de prueba, para realizar la clasificaci�n

  Devuelve por referencia:
  F.fobjetivo -> el valor correspondiente a la tasa de clasificaci�n

  Par�metro por defecto:
  validar -> ser� igual a cero cuando se trate de estudiar el modelo (entrenamiento), y
  si es igual a 1, se tratar� de validar el modelo obtenido (el vector de caracter�sticas)
  en el entrenamiento con la base de datos "nueva" para comprobar c�mo de bueno es el modelo.
*/
void BaseDatos::EvaluationFunctionFeatures(Features &F, BaseDatos &test, int validar)
{
	vector<E_dist> distancias; //Por ser k==3, no debe de tener m�s de 3 distancias
	int bien_clasificadas=0;

	//Para cada dato/fila en test
	for(int j=0; j<test.getSizeBD(); j++)
	{
		//Base de datos train
		for(int i=0; i<getSizeBD(); i++)
		{
			if((j!=i) || (validar==1))
			{
				E_dist d;			
				d.distance = Euclideandistance(F.features, test, j, i);
				d.pos = i;
				d.classification = getValueClassificator(i);

				if(distancias.size()<K)
				{
					distancias.push_back(d);
					if(distancias.size()==K)
						sort(distancias.begin(), distancias.end(), compare);
				}
				else if(distancias[K-1].distance>d.distance)
				{
					distancias.pop_back();
					distancias.push_back(d);
					sort(distancias.begin(), distancias.end(), compare);
				}
			}
		}

		//Clasificar seg�n el mayor n�mero de coincidencias
		string clase = distancias[0].classification;
		//Un voto, porque la clase de inicio es igual que ella misma
		int voto = 1;
		for(int c=1; c<K; c++)
		{
			if((clase.compare(distancias[c].classification)) == 0)
				voto++;
		}
		//Si es >= 2, pertenece a la clase de inicializaci�n.
		if(voto>=2)
		{
			if((test.getValueClassificator(j).compare(clase)) == 0)
				bien_clasificadas++;
		}
		else if((test.getValueClassificator(j).compare(clase)) != 0)
			bien_clasificadas++;
		
		distancias.clear();
	}

	// A�ade a la soluci�n su valor de la funci�n evaluaci�n
	F.fobjetivo = (bien_clasificadas*1.0/test.getSizeBD()*1.0)*100;
}

/*
  Funci�n Evaluaci�n:
  1.Rendimiento promedio de un clasificador 3-NN (K=3 vecinos)
  2.tasa_clas -> mide el porcentaje de instancias correctamente
  clasificadas pertenecientes a T:
  tasa_clas = 100*((n� instancias bien clasificadas de T)/(n� instancias en T))
  3.El objetivo es obtener el subconjunto de caracter�sticas que
  maximiza esta funci�n.

  Par�metros:
  features -> caracter�sticas a evaluar
  test -> base de datos de prueba, para realizar la clasificaci�n
  Par�metro por defecto:
  validar -> ser� igual a cero cuando se trate de estudiar el modelo, y
  si es igual a 1, se tratar� de validar el modelo obtenido (el vector de caracter�sticas)
  en el entrenamiento con la base de datos "nueva" para comprobar c�mo de bueno es el modelo.
*/
double BaseDatos::EvaluationFunction(vector<int> &features, BaseDatos &test, int validar)
{
	//vector<double> x1;
	vector<E_dist> distancias; //Por ser k==3, no debe de tener m�s de 3 distancias
	int bien_clasificadas=0;

	//Para cada dato/fila en test
	for(int j=0; j<test.getSizeBD(); j++)
	{
		//Base de datos train
		for(int i=0; i<getSizeBD(); i++)
		{
			if((j!=i) || (validar==1))
			{
				E_dist d;			
				//d.distance = Euclideandistance(features, x1, i);
				d.distance = Euclideandistance(features, test, j, i);
				d.pos = i;
				d.classification = getValueClassificator(i);

				if(distancias.size()<K)
				{
					distancias.push_back(d);
					if(distancias.size()==K)
						sort(distancias.begin(), distancias.end(), compare);
				}
				else if(distancias[K-1].distance>d.distance)
				{
					distancias.pop_back();
					distancias.push_back(d);
					sort(distancias.begin(), distancias.end(), compare);
				}
			}
		}

		//Clasificar seg�n el mayor n�mero de coincidencias
		string clase = distancias[0].classification;
		//Un voto, porque la clase de inicio es igual que ella misma
		int voto = 1;
		for(int c=1; c<K; c++)
		{
			if((clase.compare(distancias[c].classification)) == 0)
				voto++;
		}
		//Si es >= 2, pertenece a la clase de inicializaci�n.
		if(voto>=2)
		{
			if((test.getValueClassificator(j).compare(clase)) == 0)
				bien_clasificadas++;
		}
		else if((test.getValueClassificator(j).compare(clase)) != 0)
			bien_clasificadas++;
		
		distancias.clear();
	}

	return ((bien_clasificadas*1.0/test.getSizeBD()*1.0)*100);
}

/*
  Obtiene la soluci�n con todas las caracter�sticas seleccionadas.

  Devuelve por referencia:
  solucion -> todas las caracter�sticas seleccionadas.
*/
void BaseDatos::Knn(vector<int> &solucion)
{
	solucion.clear();
	for(int i=0; i<getFeatures(); i++)
		solucion.push_back(1);
}

/********************************************************************************/
//					M�todos Privados
/********************************************************************************/
/*
  Lectura de un archivo .arff e inicializaci�n de datos.
*/
void BaseDatos::openFile(string ruta)
{
	ifstream f;
	char contenido[50];

	f.open(ruta, ios::in);
	if (!f.is_open())
	{
		cout << "ERROR: no se puede abrir el fichero para leer" << endl;
		return;
	}  

	f >> contenido;
	//cout << contenido << endl;
	//cout << strlen(contenido) << endl;
	while (!f.eof())
	{
		if ((strcmp (contenido,"@relation") == 0) || (strcmp (contenido,"@RELATION") == 0))
		{
			// Leemos la siguiente palabra
			f >> contenido;
			setName(contenido);
		}else if ((strcmp (contenido,"@attribute") == 0) || (strcmp (contenido,"@ATTRIBUTE") == 0))
		{
			char nombre[50];
			// Leemos la siguiente palabra
			f >> contenido;
			strcpy(nombre, contenido);
			f >> contenido;
			if ((strcmp (contenido,"real") == 0) || (strcmp (contenido,"NUMERIC") == 0) || (strcmp (contenido,"integer") == 0))
				addPredictor(nombre);
			else
				setClassificator(nombre);
		}else if ((strcmp (contenido,"@data") == 0) || (strcmp (contenido,"@DATA") == 0))
		{
			cout << "Loading BD..." << endl;
			bool crear_vector = true;
			vector<double> v_num;
			double numero;
			string clase;
			//int r=0;
			while (!f.eof())
			{
				for(int i=0; i<predictores.name.size(); i++)
				{
					f >> numero;
					if(crear_vector)
					{
						v_num.push_back(numero);
						predictores.value.push_back(v_num);
						v_num.clear();
					}else{
						predictores.value[i].push_back(numero);
					}
					f >> contenido; //por la coma de separaci�n
				}

				f >> clase;
				clasificador.value.push_back(clase);
				//cout << clasificador.value[r] << " " << clase << endl;
				//r++;
				//addDato();
				crear_vector = false;
			} 
			cout << "...Done" << endl;
		}

		if(!f.eof())
			f >> contenido;		
	}

	f.close();
}

/* 
  Normalizar los datos entre [0, 1] -> xj = (xj - Minj)/(Maxj - Minj).
  S�lo se normaliza cuando el m�nimo<0 o el m�ximo>1.
*/
void BaseDatos::normalizeBD()
{
	double menor, mayor;

	//Para cada una de las caracter�sticas
	for(int i=0; i<predictores.value.size(); i++)
	{
		//Obtener el m�nimo y el m�ximo
		menor = min(predictores.value[i], 0, predictores.value[i].size()-1);
		mayor = max(predictores.value[i], 0, predictores.value[i].size()-1);
		//cout << "Menor: " << menor << endl;
		//cout << "Mayor: " << mayor << endl;
		if(menor<0 || mayor>1)
		{
			for(int j=0; j<predictores.value[i].size(); j++)
			{
				double num = predictores.value[i][j];
				predictores.value[i][j] = (num - menor)/(mayor - menor);
			}			
		}
	}
}
