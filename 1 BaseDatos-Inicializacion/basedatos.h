#ifndef __BASEDATOS_H
#define __BASEDATOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>

#include "random_ppio.h"

//Bilblioteca para manejo archivos 
#include <fstream>
#include <iostream>

//Indica el número de vecinos que se tendrán en cuenta
#define K 3

using namespace std;

struct variable{
	vector<string> name;
	vector<vector<double>> value;
};

//Sólo admite una única variable clasificadora
struct variableClass{
	char name[50];
	vector<string> value;
};

struct E_dist{
	int pos;
	string classification;
	double distance;
};

// Estructura para mejorar la eficiencia
struct Features{
	vector<int> features;
	double fobjetivo;
	bool evaluada;
	bool fobj_mejor, fobj_peor;

	// Constructor
	Features()
	{
		evaluada = false;
		fobjetivo = -1;
		fobj_mejor = false;
		fobj_peor = false;
	}
};

bool compare(const E_dist &a, const E_dist &b);
bool compare2(const E_dist &a, const E_dist &b);
//bool compare(const Features &a, const Features &b);

class BaseDatos
{
	private:
		void normalizeBD();
		void openFile(string ruta);

	public:
		//Constructors
		BaseDatos();
		BaseDatos(BaseDatos &otra, int, int, char *);
		BaseDatos(BaseDatos &otra, int, char *);
		~BaseDatos();
		void createBD(string ruta, int reducir=0);
		void sinSesgo(BaseDatos &train, BaseDatos &test);

		//Set functions
		void setName(char *n);
		void addPredictor(char *n);
		void addPredictor(string n);
		void setClassificator(char *n);
		//void addDato();

		//Get functions
		char* getName();
		string getNamePredictor(int pos);
		char* getNameClassificator();
		double getValuePredictor(int var, int pos);
		string getValueClassificator(int pos);
		int getSizeBD();
		int getFeatures();
		void printBD();
		
		//Operators
		double min(vector<double> &s, int ini, int fin);
		double max(vector<double> &s, int ini, int fin);
		double Euclideandistance(vector<int> &, BaseDatos &, int, int);
		double EvaluationFunction(vector<int> &, BaseDatos &, int validar=0);
		void EvaluationFunctionFeatures(Features &features, BaseDatos &test, int validar=0);
		void Knn(vector<int> &solucion);
		
	private:
		char name[50];
		variable predictores;
		variableClass clasificador;
		//static int num_BD;
};

#endif
