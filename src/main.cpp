#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <cmath>
#include <emscripten/emscripten.h>

// Headers
#include "../include/common.h"
#include "../include/math_funcs.h"
#include "../include/newton_raphson.h"
#include "../include/newton_modificado.h"
#include "../include/secante.h"

using namespace std;

// Função utilitária que lê valores de "a" em string quebra e transforma em double
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

// Helper para converter doubles em string para JSON
string safeVal(double val) {
    if (std::isnan(val) || std::isinf(val)) {
        return "null"; 
    }
    return to_string(val);
}

// Função "main" do emscripten que chama os métodos e retorna os resultados
extern "C" {
    EMSCRIPTEN_KEEPALIVE
    const char* calcularTodosMetodos(char* stringValoresA, double d0, double precisao, int maxIter) {
        vector<double> listaA = parsearValoresA(stringValoresA);
        
        stringstream ss;
        ss << "["; 

        // Loop para calcular "n" valores de "a" para cada método
        for (size_t i = 0; i < listaA.size(); i++) {
            double a = listaA[i];
            
            // Método Newton
            ResultadoMetodo resNR = newtonRaphson(a, d0, precisao, maxIter);
            
            // Derivada antes da função
            double fder_d = funcao_derivada(a, d0);
            // Método Newton Modificado
            ResultadoMetodo resNM = newtonModificado(a, d0, precisao, fder_d, maxIter);
            
            
            // Iniciando d1
            double d1 = d0 + 0.1;
            // Método Secante
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