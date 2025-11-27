#ifndef NEWTON_MODIFICADO_H
#define NEWTON_MODIFICADO_H
#include "common.h"

// Header Método de Newton Modificado
ResultadoMetodo newtonModificado(double a, double d_inicial, double precisao, double fder_d, int maxIter);

#endif