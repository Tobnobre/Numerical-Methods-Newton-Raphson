#include "../include/newton_raphson.h"
#include "../include/math_funcs.h"
#include <cmath>
#include <iostream>

ResultadoMetodo newtonRaphson(double a, double d_inicial, double precisao, int maxIter) {

    // Verifica se o chute inicial já é a raiz
    if (std::abs(funcao(a, d_inicial)) < precisao) {
        return {d_inicial, 0, 0.0, true, ""};
    }

    // Dados iniciais
    int passos = 0;
    double d_anterior = d_inicial;
    double d_atual = d_inicial;
    double erro_atual = 0.0;

    // Laço principal de iterações definidas pelo usuário
    while (passos < maxIter) {
        passos++;

        // Atribui funções
        double f = funcao(a, d_anterior);
        double fder = funcao_derivada(a, d_anterior);

        // Verifica se a derivada (denomidor é próximo de zero)
        if (std::abs(fder) < 1e-10) return {
            -999.0, passos, 0.0, false, "Denom. prox. zero"
        };

        // Método de Newton-Raphson
        d_atual = d_anterior - (f / fder);

        // Verifica se o exp é muito alto ou infinito
        if (std::isnan(d_atual) || std::isinf(d_atual)) return {
            d_atual, passos, 0.0, false, "Divergiu (NaN/Inf)"
        };

        erro_atual = std::abs(d_atual - d_anterior);

        // Critério de parada
        if ((erro_atual < precisao) || (std::abs(funcao(a, d_atual)) < precisao)) {
            return {d_atual, passos, erro_atual, true, ""};
        }

        d_anterior = d_atual;
    }

    // Caso passe o número máximo de iterações sai do loop
    return {d_atual, passos, erro_atual, false, "Excedeu iteracoes"};
}