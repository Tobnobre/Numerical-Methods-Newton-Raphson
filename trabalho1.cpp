#include <iostream>
#include <string>
#include <iostream>  // Para std::cin e std::cout (entrada/saída)
#include <cmath>     // Para funções matemáticas: exp(), pow(), abs()
#include <vector>    // Para armazenar a lista de 'n' valores de 'a'
#include <iomanip>   // Para formatar tabelas
#include <emscripten/emscripten.h> // Necessário para exportar
#include <sstream> // Usado para construir a string JSON
#include <cstring>

using namespace std;

struct ResultadoMetodo {
    double raiz;
    int iteracoes;
    double erro_final;
    bool sucesso;
    string mensagem_erro;
};


double funcao (double a, double d){
    // Retorno da função f(d) = a*(e^d) – 4d²
    return (a*exp(d) - 4*d*d);
}

double funcao_derivada (double a, double d){
    // Retorno da função f'(d) = a*e^d - 8d
    return (a*exp(d) - 8*d);
}

ResultadoMetodo newtonRaphson(double a, double d_inicial, double precisao, int maxIter) {
    int passos = 0;
    double d_anterior = d_inicial;
    double d_atual = d_inicial;
    double erro_atual = 0.0;

    // Loop princial
    while (passos < maxIter) {
        passos++;
        double f = funcao(a, d_anterior);
        double fder = funcao_derivada(a, d_anterior);

        // Verifica se a derivada é igual a zero
        if (std::abs(fder) < 1e-10) {
            return {-999.0, passos, 0.0, false, "Derivada proxima de zero"};
        }

        d_atual = d_anterior - (f / fder);

        // Verifica indeterminações
        if (std::isnan(d_atual) || std::isinf(d_atual)) {
            return {d_atual, passos, 0.0, false, "Divergiu (NaN/Inf)"};
        }

        erro_atual = std::abs(d_atual - d_anterior);

        // Critérios de parada
        if ((erro_atual < precisao) || (std::abs(funcao(a, d_atual)) < precisao)) {
            return {d_atual, passos, erro_atual, true, ""};
        }

        // Prepara para a próxima iteração
        d_anterior = d_atual;
    }

    // Se saiu do loop, estourou o limite
    return {d_atual, passos, erro_atual, false, "Excedeu o maximo de iteracoes"};
}

ResultadoMetodo newtonModificado(double a, double d_inicial, double precisao, double fder_d, int maxIter) {
    int passos = 0;
    double d_anterior = d_inicial;
    double d_atual = d_inicial;
    double erro_atual = 0.0;

    // Verifica a derivada apenas uma vez
    if (std::abs(fder_d) < 1e-10) {
        return {-999.0, 0, 0.0, false, "Derivada proxima de zero"};
    }

    //Loop principal
    while (passos < maxIter) {
        passos++;
        double f = funcao(a, d_anterior);

        d_atual = d_anterior - (f / fder_d);

        // Verifica indeterminações
        if (std::isnan(d_atual) || std::isinf(d_atual)) {
            return {d_atual, passos, 0.0, false, "Divergiu (NaN/Inf)"};
        }
        
        erro_atual = std::abs(d_atual - d_anterior);

        // Critérios de parada
        if ((erro_atual < precisao) || (std::abs(funcao(a, d_atual)) < precisao)) {
            return {d_atual, passos, erro_atual, true, ""};
        }
        d_anterior = d_atual;
    }
    return {d_atual, passos, erro_atual, false, "Excedeu iteracoes"};
}

ResultadoMetodo secante(double a, double d0, double d1, double precisao, int maxIter) {
    int passos = 0;
    double dNmenosUm = d0;
    double dN = d1;
    double dNmaisUm;
    double erro_atual = 0.0;

    //Loop principal
    while (passos < maxIter) {
        passos++;
        double f_dN = funcao(a, dN);
        double f_dNmenosUm = funcao(a, dNmenosUm);

        // Verfica se é proximo de zero
        if (std::abs(f_dN - f_dNmenosUm) < 1e-20) {
            return {-999.0, passos, 0.0, false, "Denominador prooximo de zero"};
        }

        dNmaisUm = ((f_dN * dNmenosUm - f_dNmenosUm * dN) / (f_dN - f_dNmenosUm));

        // Verifica indeterminações
        if (std::isnan(dNmaisUm) || std::isinf(dNmaisUm)) {
            return {dNmaisUm, passos, 0.0, false, "Divergiu (NaN/Inf)"};
        }

        erro_atual = std::abs(dNmaisUm - dN);

        if ((erro_atual < precisao) || (std::abs(funcao(a, dNmaisUm)) < precisao)) {
            return {dNmaisUm, passos, erro_atual, true, ""};
        }

        // Prepara para a próxima iteração
        dNmenosUm = dN;
        dN = dNmaisUm;
    }
    return {dNmaisUm, passos, erro_atual, false, "Excedeu iteracoes"};
}

// Começa o processo de exibição do resultado
extern "C" {
    // Função que o JS vai chamar
    EMSCRIPTEN_KEEPALIVE
    const char* calcularNewtonRaphson(double a, double d0, double precisao, int maxIter) {
        // Chama sua função
        ResultadoMetodo res = newtonRaphson(a, d0, precisao, maxIter);

        // Constrói um JSON com o resultado
        stringstream ss;
        ss << "{";
        ss << "\"sucesso\": " << (res.sucesso ? "true" : "false") << ",";
        ss << "\"raiz\": " << res.raiz << ",";
        ss << "\"iteracoes\": " << res.iteracoes << ",";
        ss << "\"erro_final\": " << res.erro_final << ",";
        ss << "\"mensagem_erro\": \"" << res.mensagem_erro << "\"";
        ss << "}";

        // Converte a string para um formato Emscripten
        string json_str = ss.str();
        char* resultado_wasm = new char[json_str.length() + 1];
        strcpy(resultado_wasm, json_str.c_str());
        
        //Retorna JSON
        return resultado_wasm;
    }

    // Função que o JS vai chamar
    EMSCRIPTEN_KEEPALIVE
    const char* calcularNewtonModificado(double a, double d0, double precisao, int maxIter) {   
        // Primeiramente calcula a derivada
        double fder_d = funcao_derivada(a, d0);

        // Chama a função
        ResultadoMetodo res = newtonModificado(a, d0, precisao, fder_d, maxIter);

        // Constrói um JSON com o resultado
        stringstream ss;
        ss << "{";
        ss << "\"sucesso\": " << (res.sucesso ? "true" : "false") << ",";
        ss << "\"raiz\": " << res.raiz << ",";
        ss << "\"iteracoes\": " << res.iteracoes << ",";
        ss << "\"erro_final\": " << res.erro_final << ",";
        ss << "\"mensagem_erro\": \"" << res.mensagem_erro << "\"";
        ss << "}";
        
        // Converte a string para um formato Emscripten
        string json_str = ss.str();
        char* resultado_wasm = new char[json_str.length() + 1];
        strcpy(resultado_wasm, json_str.c_str());

        //Retorna JSON
        return resultado_wasm;
    }

    // Função que o JS vai chamar
    EMSCRIPTEN_KEEPALIVE
    const char* calcularSecante(double a, double d0, double precisao, int maxIter) {
        //Primeiramente define d1:
        double d1 = d0 + 0.1;
        
        // Chama a função
        ResultadoMetodo res = secante(a, d0, d1, precisao, maxIter);

        // Constrói um JSON com o resultado
        stringstream ss;
        ss << "{";
        ss << "\"sucesso\": " << (res.sucesso ? "true" : "false") << ",";
        ss << "\"raiz\": " << res.raiz << ",";
        ss << "\"iteracoes\": " << res.iteracoes << ",";
        ss << "\"erro_final\": " << res.erro_final << ",";
        ss << "\"mensagem_erro\": \"" << res.mensagem_erro << "\"";
        ss << "}";
        
        // Converte a string para um formato Emscripten
        string json_str = ss.str();
        char* resultado_wasm = new char[json_str.length() + 1];
        strcpy(resultado_wasm, json_str.c_str());

        //Retorna JSON
        return resultado_wasm;
    }
} // Fim do 'extern "C"'