#include "../include/math_funcs.h"

// Função base
double funcao(double a, double d){
    return (a * std::exp(d) - 4 * d * d);
}

// Função derivada
double funcao_derivada(double a, double d){
    return (a * std::exp(d) - 8 * d);
}