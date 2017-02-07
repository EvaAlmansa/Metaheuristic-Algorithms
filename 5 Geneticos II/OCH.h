#ifndef __OCH_H
#define __OCH_H

#include "blprimermejor.h"

#define NHORMIGAS 10
#define BETA 2
#define ALFA 1
#define EVAPORACION 0.2
#define EVAPORACION_LOCAL 0.2

/********************************************************************************/
//				Sistema de Colonias de Hormigas BL - (SCH-BL)
/********************************************************************************/

void SCH_BL(BaseDatos &train, BaseDatos &test, vector<int> &mejor_sol, vector<double> &inH, double &tau0, double q0);

/********************************************************************************/
//					Sistema de Hormigas - (SHMM_BL)
/********************************************************************************/

void SHMM_BL(BaseDatos &train, BaseDatos &test, vector<int> &sol_mejor, vector<double> &inH);

#endif