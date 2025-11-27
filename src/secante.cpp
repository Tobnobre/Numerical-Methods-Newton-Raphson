#include "../include/secante.h"
#include "../include/math_funcs.h"
#include <cmath>

ResultadoMetodo secante(double a, double d0, double d1, double precisao, int maxIter) {
    // Verifica se o d0 já é a raiz
    if (std::abs(funcao(a, d0)) < precisao) {
        return {d0, 0, 0.0, true, ""};
    }

    // Verifica se o d1 já é a raiz ou se o intervalo é muito pequeno
    if (std::abs(funcao(a, d1)) < precisao || std::abs(d1 - d0) < precisao) {
        return {d1, 0, std::abs(d1 - d0), true, ""};
    }
    
    // Dados iniciais
    int passos = 0;
    double dNmenosUm = d0;
    double dN = d1;
    double dNmaisUm;
    double erro_atual = 0.0;

    // Laço principal de iterações definidas pelo usuário
    while (passos < maxIter) {
        passos++;

        // Atribui funções
        double f_dN = funcao(a, dN);
        double f_dNmenosUm = funcao(a, dNmenosUm);

        // Verifica se o denominador é zero
        if (std::abs(f_dN - f_dNmenosUm) < 1e-20) return {
            -999.0, passos, 0.0, false, "Denom. prox. zero"
        };

        // Método da secante
        dNmaisUm = ((f_dN * dNmenosUm - f_dNmenosUm * dN) / (f_dN - f_dNmenosUm));

        // Verifica se o exp é muito alto ou infinito
        if (std::isnan(dNmaisUm) || std::isinf(dNmaisUm)) return {
            dNmaisUm, passos, 0.0, false, "Divergiu (NaN/Inf)"
        };

        erro_atual = std::abs(dNmaisUm - dN);

        // Critério de parada
        if ((erro_atual < precisao) || (std::abs(funcao(a, dNmaisUm)) < precisao)) {
            return {dNmaisUm, passos, erro_atual, true, ""};
        }

        dNmenosUm = dN;
        dN = dNmaisUm;
    }

    // Caso passe o número máximo de iterações sai do loop
    return {dNmaisUm, passos, erro_atual, false, "Excedeu iteracoes"};
}