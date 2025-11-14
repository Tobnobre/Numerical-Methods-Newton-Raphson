#include <iostream>
#include <string>
#include <iostream>  // Para std::cin e std::cout (entrada/saída)
#include <cmath>     // Para funções matemáticas: exp(), pow(), abs()
#include <vector>    // Para armazenar sua lista de 'n' valores de 'a'
#include <iomanip>   // Para formatar sua tabela de saída (definir casas decimais, etc.)
#include <emscripten/emscripten.h> // Necessário para "exportar"
#include <sstream> // Usado para construir a string JSON
#include <cstring>

using namespace std; // Para não precisar digitar std::

struct ResultadoMetodo {
    double raiz;
    int iteracoes;
    double erro_final;
    bool sucesso;
    string mensagem_erro;
};


double funcao (double a, double d){
    // Retorno da função f(d) = a*(e^d) – 4d²
    return (a*exp(d) - 4*d*d); // exp(d) é 'e elevado a d'
}

double funcao_derivada (double a, double d){
    // Retorno da função f'(d) = a*e^d - 8d
    return (a*exp(d) - 8*d);
}

ResultadoMetodo newtonRaphson_iterativo(double a, double d_inicial, double precisao) {
    int passos = 0;
    double d_anterior = d_inicial;
    double d_atual = d_inicial;
    double erro_atual = 0.0;

    // Loop com limite de segurança de 1000 iterações
    while (passos < 1000) {
        passos++;
        double f = funcao(a, d_anterior);
        double fder = funcao_derivada(a, d_anterior);

        if (abs(fder) < 1e-10) {
            return {-999.0, passos, 0.0, false, "Derivada próxima de zero"};
        }

        d_atual = d_anterior - (f / fder);
        erro_atual = abs(d_atual - d_anterior);

        // Critérios de parada
        if ((erro_atual < precisao) || (abs(funcao(a, d_atual)) < precisao)) {
            return {d_atual, passos, erro_atual, true, ""};
        }

        d_anterior = d_atual; // Prepara para a próxima iteração
    }

    // Se saiu do loop, estourou o limite
    return {d_atual, passos, erro_atual, false, "Excedeu 1000 iteracoes"};
}

ResultadoMetodo newtonModificado_iterativo(double a, double d_inicial, double precisao, double derivadaX0) {
    int passos = 0;
    double d_anterior = d_inicial;
    double d_atual = d_inicial;
    double erro_atual = 0.0;

    // Checa a derivada fixa apenas uma vez
    if (abs(derivadaX0) < 1e-10) {
        return {-999.0, 0, 0.0, false, "Erro: f'(d0) proxima de zero!"};
    }

    while (passos < 1000) {
        passos++;
        double f = funcao(a, d_anterior);

        d_atual = d_anterior - (f / derivadaX0);
        erro_atual = abs(d_atual - d_anterior);

        if ((erro_atual < precisao) || (abs(funcao(a, d_atual)) < precisao)) {
            return {d_atual, passos, erro_atual, true, ""};
        }
        d_anterior = d_atual;
    }
    return {d_atual, passos, erro_atual, false, "Excedeu 1000 iteracoes"};
}

ResultadoMetodo secante_iterativo(double a, double d0, double d1, double precisao) {
    int passos = 0;
    double dNmenosUm = d0;
    double dN = d1;
    double dNmaisUm = d1; // Inicialização
    double erro_atual = 0.0;

    while (passos < 1000) {
        passos++;
        double f_dN = funcao(a, dN);
        double f_dNmenosUm = funcao(a, dNmenosUm);

        if (abs(f_dN - f_dNmenosUm) < 1e-20) {
            return {-999.0, passos, 0.0, false, "Erro: Denominador próximo de zero!"};
        }

        dNmaisUm = ((f_dN * dNmenosUm - f_dNmenosUm * dN) / (f_dN - f_dNmenosUm));
        erro_atual = abs(dNmaisUm - dN);

        if ((erro_atual < precisao) || (abs(funcao(a, dNmaisUm)) < precisao)) {
            return {dNmaisUm, passos, erro_atual, true, ""};
        }

        // Prepara para a próxima iteração
        dNmenosUm = dN;
        dN = dNmaisUm;
    }
    return {dNmaisUm, passos, erro_atual, false, "Excedeu 1000 iteracoes"};
}
int main () {
    // Declarando variáveis que iremos usar:
    int n; // Quantos valores 'a' pode assumir
    double a; // Valores da constante que será substituída na fórmula
    double precisao; // Valor de precisão que será comparado para a execução dos métodos
    double d0 = 0.5;

    cout << "Insira aqui o número inteiro de valores (n) que 'a' poderá assumir:" << endl;
    cin >> n;

    cout << "Insira aqui o valor 'a':" << endl;
    cin >> a;

    cout << "Insira a precisao (ex: 0.0001):" << endl;
    cin >> precisao; // <-- CORREÇÃO: Faltava ler a precisão

    ResultadoMetodo res_nr = newtonRaphson_iterativo(a, d0, precisao);

    if(res_nr.sucesso){
        // Imprimindo os valores de Newton-Raphson
        cout << "--- Newton-Raphson ---" << endl;
        cout << "Raiz (d): " << res_nr.raiz << endl;
        cout << "Iteracoes: " << res_nr.iteracoes << endl;
        cout << "Erro final: " << res_nr.erro_final << endl;
        cout << "------------------------" << endl;
    } else {
        cout << "--- ERRO no método *Newton-Raphson* ---" << endl;
        cout << "Falha: " << res_nr.mensagem_erro << endl;
    }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// P/Método de Newton modificado, usamos sempre um mesmo denominador, então salvamos na main para otimização
    double derivadaX0 = funcao_derivada(a, d0);

    ResultadoMetodo res_nm = newtonModificado_iterativo(a, d0, precisao, derivadaX0);

    if (res_nm.sucesso) {
        // Imprimindo os valores de Newton Modificado
        cout << "--- Newton Modificado ---" << endl;
        cout << "Raiz (d): " << res_nm.raiz << endl;
        cout << "Iteracoes: " << res_nm.iteracoes << endl;
        cout << "Erro final: " << res_nm.erro_final << endl;
        cout << "------------------------" << endl;
    }
    else {
        cout << "ERRO no método Newton Modificado" << endl;
        cout << "Falha: " << res_nm.mensagem_erro << endl;
    }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Para o método da Secante, vamos assumir um d1 para que haja um intervalo possível para a análise
    float d1 = 0.6;
    ResultadoMetodo res_sec = secante_iterativo(a, d0, d1, precisao);

    if(res_sec.sucesso){
        // Imprimindo os valores de Newton Modificado
        cout << "--- Secante ---" << endl;
        cout << "Raiz (d): " << res_sec.raiz << endl;
        cout << "Iteracoes: " << res_sec.iteracoes << endl;
        cout << "Erro final: " << res_sec.erro_final << endl;
        cout << "------------------------" << endl;
    } else {
        cout << "--- ERRO no método *Secante* ---" << endl;
        cout << "Falha: " << res_sec.mensagem_erro << endl;
    }

    return 0;
}

/* ====================================================================
   COLE ISSO NO FINAL DO SEU ARQUIVO, DEPOIS DA SUA FUNÇÃO 'main'
   ====================================================================
*/

// O 'main' que você escreveu só roda no console.
// Para a GUI, não precisamos dele. O JS será o nosso "novo main".

// Inicia o bloco que será "exportado" para o JavaScript
extern "C" {

// Função "tradutora" que o JS vai chamar
// EMSCRIPTEN_KEEPALIVE garante que o compilador não otimize e remova esta função
EMSCRIPTEN_KEEPALIVE
const char* calcularNewtonRaphson(double a, double d0, double precisao) {
    // 1. Chama sua função C++ original
    ResultadoMetodo res = newtonRaphson_iterativo(a, d0, precisao);

    // 2. Constrói uma string JSON com o resultado
    stringstream ss;
    ss << "{";
    ss << "\"sucesso\": " << (res.sucesso ? "true" : "false") << ",";
    ss << "\"raiz\": " << res.raiz << ",";
    ss << "\"iteracoes\": " << res.iteracoes << ",";
    ss << "\"erro_final\": " << res.erro_final << ",";
    ss << "\"mensagem_erro\": \"" << res.mensagem_erro << "\"";
    ss << "}";

    // 3. Converte a string para um formato que o Emscripten entende
    // (Isso é um pouco complexo, mas é o padrão)
    string json_str = ss.str();
    char* resultado_wasm = new char[json_str.length() + 1];
    strcpy(resultado_wasm, json_str.c_str());
    
    // 4. Retorna a string JSON para o JavaScript
    return resultado_wasm;
}

EMSCRIPTEN_KEEPALIVE
const char* calcularNewtonModificado(double a, double d0, double precisao) {
    // Calcula o f'(d0)
    double derivadaX0 = funcao_derivada(a, d0);
    
    // Chama sua função C++ original
    ResultadoMetodo res = newtonModificado_iterativo(a, d0, precisao, derivadaX0);

    // Constrói e retorna o JSON (mesma lógica acima)
    stringstream ss;
    ss << "{";
    ss << "\"sucesso\": " << (res.sucesso ? "true" : "false") << ",";
    ss << "\"raiz\": " << res.raiz << ",";
    ss << "\"iteracoes\": " << res.iteracoes << ",";
    ss << "\"erro_final\": " << res.erro_final << ",";
    ss << "\"mensagem_erro\": \"" << res.mensagem_erro << "\"";
    ss << "}";
    
    string json_str = ss.str();
    char* resultado_wasm = new char[json_str.length() + 1];
    strcpy(resultado_wasm, json_str.c_str());
    return resultado_wasm;
}

EMSCRIPTEN_KEEPALIVE
const char* calcularSecante(double a, double d0, double d1, double precisao) {
    // Chama sua função C++ original
    ResultadoMetodo res = secante_iterativo(a, d0, d1, precisao);

    // Constrói e retorna o JSON (mesma lógica acima)
    stringstream ss;
    ss << "{";
    ss << "\"sucesso\": " << (res.sucesso ? "true" : "false") << ",";
    ss << "\"raiz\": " << res.raiz << ",";
    ss << "\"iteracoes\": " << res.iteracoes << ",";
    ss << "\"erro_final\": " << res.erro_final << ",";
    ss << "\"mensagem_erro\": \"" << res.mensagem_erro << "\"";
    ss << "}";
    
    string json_str = ss.str();
    char* resultado_wasm = new char[json_str.length() + 1];
    strcpy(resultado_wasm, json_str.c_str());
    return resultado_wasm;
}

} // Fim do 'extern "C"'