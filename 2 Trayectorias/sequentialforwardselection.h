#ifndef __SEQUENTIALFORWARDSELECTION_H
#define __SEQUENTIALFORWARDSELECTION_H

#include "algoritmosbusqueda.h"

/********************************************************************************/
//					Solución Greedy: Sequential Forward Selection (SFS)
/********************************************************************************/

//int caracteristica_mas_prometedora(vector<int> &F, BaseDatos &train, BaseDatos &test);
void caracteristica_mas_prometedora(BaseDatos &train, BaseDatos &test, vector<int> &sol_actual, vector<int> &F, int &pos, double &fobj_actual);
bool vacia(vector<int> &F);
void SFS(BaseDatos &train, BaseDatos &test, vector<int> &mejor_sol);

#endif