#ifndef __BLPRIMERMEJOR_H
#define __BLPRIMERMEJOR_H

#include "algoritmosbusqueda.h"

/********************************************************************************/
//					Búsqueda Local: Búsqueda Local del primer mejor (BL)
/********************************************************************************/

// Versión para los algoritmos BMB y Grasp, con solución inicial ya generada en la llamada a esta función.
void LSfirst(BaseDatos &train, BaseDatos &test, vector<int> &sol_act, double &fobj_act, int &evaluaciones, int MAX_EVA=200);

#endif