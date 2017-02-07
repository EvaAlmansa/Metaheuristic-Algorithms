#ifndef __GENETIC_H
#define __GENETIC_H

#include "algoritmosbusqueda.h"
#include <ctime>

#define CROMOSOMAS 50

/********************************************************************************/
//					Algortimos Genéticos -  Genetic Algorithm (GA)
/********************************************************************************/

/*
  Componentes comunes 
*/
void generate_Poblation(BaseDatos &train, BaseDatos &testVal, vector<vector<int>> &Poblacion, int features);
void populationEvaluation(BaseDatos &train, BaseDatos &testVal, vector<Features> &Pactual, Features &features_mejor, Features &features_peor, int &evaluaciones);
void selection(BaseDatos &train, BaseDatos &testVal, vector<Features> &Pactual, vector<Features> &Ppadres, int k_torneo, int activarGeneracional);
void crossing(vector<Features> &Ppadres, vector<Features> &Pintermedia, double prob_c, int genes);
void uniformMutation(vector<Features> &Pintermedia, vector<Features> &Phijos, double prob_g, int genes, int estacional);

/*
  Resto componentes
*/
// Uso en el algoritmo genético estacional
void seasonalReplacement(vector<Features> &Phijos, vector<Features> &Pactual);
// Uso en el algoritmo genético generacional con elitismo
void elitismReplacement(vector<Features> &Phijos, vector<Features> &Pactual);

/*
  Operadores
*/
bool findFeatures(vector<Features> &poblacion, Features &features);
bool findElement(vector<int> v, int elemento);

/*
  Algoritmos Genéticos
*/
void AGE(BaseDatos &train, BaseDatos &testVal, vector<int> &sol_mejor, double &prob_c, double &prob_g, int k_torneo=2);
void AGG(BaseDatos &train, BaseDatos &testVal, vector<int> &sol_mejor, double &prob_c, double &prob_g, int k_torneo=2);

#endif