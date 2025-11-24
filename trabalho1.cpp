#include <iostream>
#include <string>
#include <cmath>     
#include <vector>    
#include <iomanip>   
#include <emscripten/emscripten.h> 
#include <sstream> 
#include <cstring>
#include <algorithm>

using namespace std;

// Estrutura para retornar os resultados
struct ResultadoMetodo {
    double raiz;
    int iteracoes;
    double erro_final;
    bool sucesso;
    string mensagem_erro;
};

// Funções Matemáticas
double funcao (double a, double d){
    return (a*exp(d) - 4*d*d);
}

double funcao_derivada (double a, double d){
    return (a*exp(d) - 8*d);
}

// Métodos Numéricos
ResultadoMetodo newtonRaphson(double a, double d_inicial, double precisao, int maxIter) {
    int passos = 0;
    double d_anterior = d_inicial;
    double d_atual = d_inicial;
    double erro_atual = 0.0;

    while (passos < maxIter) {
        passos++;
        double f = funcao(a, d_anterior);
        double fder = funcao_derivada(a, d_anterior);

        if (std::abs(fder) < 1e-10) return {-999.0, passos, 0.0, false, "Derivada proxima de zero"};

        d_atual = d_anterior - (f / fder);

        // Verifica imediatamente se deu Infinito
        if (std::isnan(d_atual) || std::isinf(d_atual)) return {d_atual, passos, 0.0, false, "Divergiu (NaN/Inf)"};

        erro_atual = std::abs(d_atual - d_anterior);

        if ((erro_atual < precisao) || (std::abs(funcao(a, d_atual)) < precisao)) {
            return {d_atual, passos, erro_atual, true, ""};
        }
        d_anterior = d_atual;
    }
    return {d_atual, passos, erro_atual, false, "Excedeu iteracoes"};
}

ResultadoMetodo newtonModificado(double a, double d_inicial, double precisao, double fder_d, int maxIter) {
    int passos = 0;
    double d_anterior = d_inicial;
    double d_atual = d_inicial;
    double erro_atual = 0.0;

    if (std::abs(fder_d) < 1e-10) return {-999.0, 0, 0.0, false, "Derivada proxima de zero"};

    while (passos < maxIter) {
        passos++;
        double f = funcao(a, d_anterior);
        d_atual = d_anterior - (f / fder_d);

        if (std::isnan(d_atual) || std::isinf(d_atual)) return {d_atual, passos, 0.0, false, "Divergiu (NaN/Inf)"};
        
        erro_atual = std::abs(d_atual - d_anterior);

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

    while (passos < maxIter) {
        passos++;
        double f_dN = funcao(a, dN);
        double f_dNmenosUm = funcao(a, dNmenosUm);

        if (std::abs(f_dN - f_dNmenosUm) < 1e-20) return {-999.0, passos, 0.0, false, "Denom. prox. zero"};

        dNmaisUm = ((f_dN * dNmenosUm - f_dNmenosUm * dN) / (f_dN - f_dNmenosUm));

        if (std::isnan(dNmaisUm) || std::isinf(dNmaisUm)) return {dNmaisUm, passos, 0.0, false, "Divergiu (NaN/Inf)"};

        erro_atual = std::abs(dNmaisUm - dN);

        if ((erro_atual < precisao) || (std::abs(funcao(a, dNmaisUm)) < precisao)) {
            return {dNmaisUm, passos, erro_atual, true, ""};
        }
        dNmenosUm = dN;
        dN = dNmaisUm;
    }
    return {dNmaisUm, passos, erro_atual, false, "Excedeu iteracoes"};
}

// Função auxiliar da entrada de "a"
vector<double> parsearValoresA(char* entrada) {
    vector<double> valores;
    string s(entrada);
    string delimitador = ";";
    size_t pos = 0;
    
    for(char &c : s) { if(c == ',') c = '.'; }

    while ((pos = s.find(delimitador)) != string::npos) {
        string token = s.substr(0, pos);
        if (!token.empty()) {
            try { valores.push_back(stod(token)); } catch(...) {}
        }
        s.erase(0, pos + delimitador.length());
    }
    if (!s.empty()) {
        try { valores.push_back(stod(s)); } catch(...) {}
    }
    return valores;
}

// Se o valor for infinito ou NaN, retorna null, se for normal, retorna o número.
string safeVal(double val) {
    if (std::isnan(val) || std::isinf(val)) {
        return "null"; 
    }
    return to_string(val);
}

extern "C" {
    EMSCRIPTEN_KEEPALIVE
    const char* calcularTodosMetodos(char* stringValoresA, double d0, double precisao, int maxIter) {
        vector<double> listaA = parsearValoresA(stringValoresA);
        
        stringstream ss;
        ss << "["; 

        for (size_t i = 0; i < listaA.size(); i++) {
            double a = listaA[i];
            
            ResultadoMetodo resNR = newtonRaphson(a, d0, precisao, maxIter);
            
            double fder_d = funcao_derivada(a, d0);
            ResultadoMetodo resNM = newtonModificado(a, d0, precisao, fder_d, maxIter);
            
            double d1 = d0 + 0.1;
            ResultadoMetodo resSec = secante(a, d0, d1, precisao, maxIter);

            if (i > 0) ss << ",";

            ss << "{ \"valor_a\": " << a << ",";
            
            ss << "\"nr\": {" 
               << "\"raiz\": " << safeVal(resNR.raiz) << ", \"iter\": " << resNR.iteracoes 
               << ", \"erro\": " << safeVal(resNR.erro_final) << ", \"sucesso\": " << (resNR.sucesso?"true":"false") 
               << ", \"msg\": \"" << resNR.mensagem_erro << "\"},";

            ss << "\"nm\": {" 
               << "\"raiz\": " << safeVal(resNM.raiz) << ", \"iter\": " << resNM.iteracoes 
               << ", \"erro\": " << safeVal(resNM.erro_final) << ", \"sucesso\": " << (resNM.sucesso?"true":"false") 
               << ", \"msg\": \"" << resNM.mensagem_erro << "\"},";

            ss << "\"sec\": {" 
               << "\"raiz\": " << safeVal(resSec.raiz) << ", \"iter\": " << resSec.iteracoes 
               << ", \"erro\": " << safeVal(resSec.erro_final) << ", \"sucesso\": " << (resSec.sucesso?"true":"false") 
               << ", \"msg\": \"" << resSec.mensagem_erro << "\"}";
               
            ss << "}";
        }
        ss << "]";

        string json_str = ss.str();
        char* resultado_wasm = new char[json_str.length() + 1];
        strcpy(resultado_wasm, json_str.c_str());
        
        return resultado_wasm;
    }
}