#ifndef __BLPRIMERMEJOR_H
#define __BLPRIMERMEJOR_H

#include "algoritmosbusqueda.h"

/********************************************************************************/
//					B�squeda Local: B�squeda Local del primer mejor (BL)
/********************************************************************************/

// Versi�n para los algoritmos BMB y Grasp, con soluci�n inicial ya generada en la llamada a esta funci�n.
void LSfirst(BaseDatos &train, BaseDatos &test, vector<int> &sol_act, double &fobj_act, int &evaluaciones, int MAX_EVA=200);

#endif