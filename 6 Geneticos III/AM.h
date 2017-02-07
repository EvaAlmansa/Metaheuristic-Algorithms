#ifndef __AM_H
#define __AM_H

#include "algoritmosbusqueda.h"
#include "genetics.h"
#include "blprimermejor.h"

/********************************************************************************/
//					Algortimo Memético (AM)
/********************************************************************************/

// Algoritmo Genético Generacional con elitismo
void AM(BaseDatos &train, BaseDatos &testVal, vector<int> &sol_mejor, double &prob_c, double &prob_g, int cromosomas, int generaciones, double prob_bl, int k_torneo=2);


#endif