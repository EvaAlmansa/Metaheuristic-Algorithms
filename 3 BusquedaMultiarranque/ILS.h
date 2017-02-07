#ifndef __ILS_H
#define __ILS_H

#include "algoritmosbusqueda.h"
#include "blprimermejor.h"

#define PROB10 0.1

/********************************************************************************/
//			Búsqueda Local Reiterada - Iterative Local Search (ILS)
/********************************************************************************/

void ILS(BaseDatos &train, BaseDatos &testVal, vector<int> &sol_actual);

#endif