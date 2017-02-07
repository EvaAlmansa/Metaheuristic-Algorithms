#include "basedatos.h"

/*
  Función que compara dos distancias para conseguir que la menor distancia 
  se encuentre en primer lugar.
*/
bool compare(const E_dist &a, const E_dist &b)
{
	return a.distance < b.distance;
}

/*
  Función que compara dos distancias para conseguir que la mayor distancia 
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

//Constructor de copia, donde el parámetro max no se incluye.
BaseDatos::BaseDatos(BaseDatos &otra, int min, int max, char *nombre_bd)
{
	bool crear_vector = true, crear_clasificador = false;
	vector<double> v_num;

	setName(nombre_bd); 
	setClassificator(otra.getNameClassificator());

	if(max<min)
	{
		//Con cada una de las características
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
  Constructor de copia, dado una BD demasiado grande, hacer más pequeña 
  indicada en el parámetro tope.
  Además normaliza los datos.
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

	//Con cada una de las características
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

  Parámetro por defecto:
  reducir=0 --> si es distinto de cero, evita normalizar la BD de datos grande.
*/
void BaseDatos::createBD(string ruta, int reducir)
{
	openFile(ruta);
	if(reducir==0)
		normalizeBD();
}

/*
  Función sin sesgo, a partir de una base de datos dada
  asignar el 50% a train y el otro 50% a test.
  Para cada fila de la base de datos asigna a train o a test
  según una probabilidad, menor que 0.5 y mayor igual que 0.5, 
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
				// Obtener el dato de la característica y posición seleccionada
				v_num.push_back(getValuePredictor(f,seleccion_train[i]));
				trainBD.predictores.value.push_back(v_num);
				v_num.clear();
			}else{
				numero = getValuePredictor(f,seleccion_train[i]);
				trainBD.predictores.value[f].push_back(numero);
			}
			// El clasificador sólo contiene un vector
			if(crear_clasificador)
				trainBD.clasificador.value.push_back(getValueClassificator(seleccion_train[i]));
			
			crear_vector=false;	
		}

		crear_vector = true;
		for(int i=0; i<seleccion_test.size(); i++)
		{
			if(crear_vector)
			{
				// Obtener el dato de la característica y posición seleccionada
				v_num.push_back(getValuePredictor(f,seleccion_test[i]));
				testBD.predictores.value.push_back(v_num);
				v_num.clear();
			}else{
				numero = getValuePredictor(f,seleccion_test[i]);
				testBD.predictores.value[f].push_back(numero);
			}
			// El clasificador sólo contiene un vector
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
		cout << "Error: No esta añadiendo nombre a la Base de Datos" << endl;
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
		cout << "Error: No esta añadiendo nombre a la variable predictor" << endl;
		return;
	}
}

//Add the name of the predictor. 
void BaseDatos::addPredictor(string n)
{
	if(n.size()>0)
		predictores.name.push_back(n);
	else{
		cout << "Error: No esta añadiendo nombre a la variable predictor" << endl;
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
		cout << "Error: No esta añadiendo nombre a la variable clasificador." << endl;
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
  Obtener el nombre del predictor, según indique la posición
  en el parámetro de entrada pos.
*/
string BaseDatos::getNamePredictor(int pos)
{
	return predictores.name[pos];
}

/*
  Obtener el nombre de la característica clasificador.
*/
char* BaseDatos::getNameClassificator()
{
	return clasificador.name;
}

/*
  Obtener el valor/dato que pertenece a la característica y posición
  indicada por parámetro.

  Parámetros de entrada:
  var -> Posición de la característica, el número que le corresponda
		 en la columna de la base de datos.
  pos -> Posición del dato correspondiente
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
  Búsqueda del mínimo.
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
  Búsqueda del máximo.
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
  Distancia Euclídea:
  d(xi,xj) = sqrt((xi1 - xj1)^2+(xi2 - xj2)^2+...+(xin - xjn)^2)
  Tal que i <> j y n = |X| (n = al número total de atributos
  sin contar la variable de clasificación).

  Parámetros:
  features -> solución de características seleccionadas
  x1 -> vector del dato a comparar
  pos -> posición del dato en la base de datos a comparar
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
  Función Evaluación:
  1.Rendimiento promedio de un clasificador 3-NN (K=3 vecinos)
  2.tasa_clas -> mide el porcentaje de instancias correctamente
  clasificadas pertenecientes a T:
  tasa_clas = 100*((nº instancias bien clasificadas de T)/(nº instancias en T))
  3.El objetivo es obtener el subconjunto de características que
  maximiza esta función.

  Parámetros:
  F -> características a evaluar
  test -> base de datos de prueba, para realizar la clasificación

  Devuelve por referencia:
  F.fobjetivo -> el valor correspondiente a la tasa de clasificación

  Parámetro por defecto:
  validar -> será igual a cero cuando se trate de estudiar el modelo (entrenamiento), y
  si es igual a 1, se tratará de validar el modelo obtenido (el vector de características)
  en el entrenamiento con la base de datos "nueva" para comprobar cómo de bueno es el modelo.
*/
void BaseDatos::EvaluationFunctionFeatures(Features &F, BaseDatos &test, int validar)
{
	vector<E_dist> distancias; //Por ser k==3, no debe de tener más de 3 distancias
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

		//Clasificar según el mayor número de coincidencias
		string clase = distancias[0].classification;
		//Un voto, porque la clase de inicio es igual que ella misma
		int voto = 1;
		for(int c=1; c<K; c++)
		{
			if((clase.compare(distancias[c].classification)) == 0)
				voto++;
		}
		//Si es >= 2, pertenece a la clase de inicialización.
		if(voto>=2)
		{
			if((test.getValueClassificator(j).compare(clase)) == 0)
				bien_clasificadas++;
		}
		else if((test.getValueClassificator(j).compare(clase)) != 0)
			bien_clasificadas++;
		
		distancias.clear();
	}

	// Añade a la solución su valor de la función evaluación
	F.fobjetivo = (bien_clasificadas*1.0/test.getSizeBD()*1.0)*100;
}

/*
  Función Evaluación:
  1.Rendimiento promedio de un clasificador 3-NN (K=3 vecinos)
  2.tasa_clas -> mide el porcentaje de instancias correctamente
  clasificadas pertenecientes a T:
  tasa_clas = 100*((nº instancias bien clasificadas de T)/(nº instancias en T))
  3.El objetivo es obtener el subconjunto de características que
  maximiza esta función.

  Parámetros:
  features -> características a evaluar
  test -> base de datos de prueba, para realizar la clasificación
  Parámetro por defecto:
  validar -> será igual a cero cuando se trate de estudiar el modelo, y
  si es igual a 1, se tratará de validar el modelo obtenido (el vector de características)
  en el entrenamiento con la base de datos "nueva" para comprobar cómo de bueno es el modelo.
*/
double BaseDatos::EvaluationFunction(vector<int> &features, BaseDatos &test, int validar)
{
	//vector<double> x1;
	vector<E_dist> distancias; //Por ser k==3, no debe de tener más de 3 distancias
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

		//Clasificar según el mayor número de coincidencias
		string clase = distancias[0].classification;
		//Un voto, porque la clase de inicio es igual que ella misma
		int voto = 1;
		for(int c=1; c<K; c++)
		{
			if((clase.compare(distancias[c].classification)) == 0)
				voto++;
		}
		//Si es >= 2, pertenece a la clase de inicialización.
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
  Obtiene la solución con todas las características seleccionadas.

  Devuelve por referencia:
  solucion -> todas las características seleccionadas.
*/
void BaseDatos::Knn(vector<int> &solucion)
{
	solucion.clear();
	for(int i=0; i<getFeatures(); i++)
		solucion.push_back(1);
}

/********************************************************************************/
//					Métodos Privados
/********************************************************************************/
/*
  Lectura de un archivo .arff e inicialización de datos.
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
					f >> contenido; //por la coma de separación
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
  Sólo se normaliza cuando el mínimo<0 o el máximo>1.
*/
void BaseDatos::normalizeBD()
{
	double menor, mayor;

	//Para cada una de las características
	for(int i=0; i<predictores.value.size(); i++)
	{
		//Obtener el mínimo y el máximo
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
