// #include <iostream>
// #include <string>
// #include <iostream>  // Para std::cin e std::cout (entrada/saída)
// #include <cmath>     // Para funções matemáticas: exp(), pow(), abs()
// #include <vector>    // Para armazenar sua lista de 'n' valores de 'a'
// #include <iomanip>   // Para formatar sua tabela de saída (definir casas decimais, etc.)
// #include <emscripten/emscripten.h> // Necessário para "exportar"
// #include <sstream> // Usado para construir a string JSON
// #include <cstring>

// using namespace std; // Para não precisar digitar std::

// struct ResultadoMetodo {
//     double raiz;
//     int iteracoes;
//     double erro_final;
//     bool sucesso;
//     string mensagem_erro;
// };


// double funcao (double a, double d){
//     // Retorno da função f(d) = a*(e^d) – 4d²
//     return (a*exp(d) - 4*d*d); // exp(d) é 'e elevado a d'
// }

// double funcao_derivada (double a, double d){
//     // Retorno da função f'(d) = a*e^d - 8d
//     return (a*exp(d) - 8*d);
// }

// ResultadoMetodo newtonRaphson (double a, double d_anterior, int num_passos_nRaphson, double precisao){
//     double f = funcao(a, d_anterior);
//     double fder = funcao_derivada(a, d_anterior);

//     num_passos_nRaphson += 1;

//     // Evitando divisão por zero: 
//     if (abs(fder) < 1e-10) { 
//         // Retorna um resultado "inválido"
//         return { -999.0, num_passos_nRaphson, 0.0, false, "Derivada próxima de zero" }; 
//     }

//     double d_atual = d_anterior - (f/fder);
//     double erro_atual = abs(d_atual - d_anterior);

//     // Apresentamos as condições de parada: |dn - dn-1| < E; |f(dn) < E; número máximo de iteraçẽs = 1000
//     if ((erro_atual < precisao) || (abs(funcao (a, d_atual)) < precisao) || (num_passos_nRaphson > 1000)) {
//         return {d_atual, num_passos_nRaphson, erro_atual, true, ""};
//     }
//     else {
//         return newtonRaphson(a, d_atual, num_passos_nRaphson, precisao);
//     }
// }

// ResultadoMetodo newtonModificado (double a, double d_anterior, int num_passos_nModificado, double precisao, double derivadaX0){
//     double f = funcao(a, d_anterior);
//     num_passos_nModificado += 1;

//     // Evitando divisão por zero: 
//     if (abs(derivadaX0) < 1e-10) { 
//         // Retorna um resultado "inválido"
//         return { -999.0, num_passos_nModificado, 0.0, false, "Erro: Derivada proxima de zero!"}; 
//     }

//     double d_atual = d_anterior - (f/derivadaX0);
//     double erro_atual = abs(d_atual - d_anterior);

//     // Apresentamos as condições de parada: |dn - dn-1| < E; |f(dn) < E; número máximo de iteraçẽs = 1000
//     if ((erro_atual < precisao) || (abs(funcao (a, d_atual)) < precisao) || (num_passos_nModificado > 1000)) {
//         return {d_atual, num_passos_nModificado, erro_atual, true, ""};
//     }
//     else {
//         return newtonModificado(a, d_atual, num_passos_nModificado, precisao, derivadaX0);
//     }
// }

// ResultadoMetodo secante (double a, double dNmenosUm, double dN, int num_passos_secante, double precisao){
//     num_passos_secante += 1;

//     double f_dN = funcao (a, dN);
//     double f_dNmenosUm = funcao (a, dNmenosUm);

//     // Evitando divisão por zero (diminuímos o parâmetro por se tratar de uma subtração de doubles): 
//     if (abs(f_dN - f_dNmenosUm) < 1e-20) { 
//         // Retorna um resultado "inválido"
//         return { -999.0, num_passos_secante, 0.0, false, "Erro: Denominador próximo de zero!"}; 
//     }

//     double dNmaisUm = ((f_dN*dNmenosUm - f_dNmenosUm*dN)/ (f_dN - f_dNmenosUm));
//     double erro_atual = abs(dNmaisUm - dN);

//     // Apresentamos as condições de parada: |dn - dn-1| < E; |f(dn) < E; número máximo de iteraçẽs = 1000
//     if ((erro_atual < precisao) || (abs(funcao (a, dNmaisUm)) < precisao) || (num_passos_secante > 1000)) {        
//         return {dNmaisUm, num_passos_secante, erro_atual, true, ""};
//     }
//     else {
//         return secante(a, dN, dNmaisUm, num_passos_secante, precisao);
//     }
// }

// int main () {
//     // Declarando variáveis que iremos usar:
//     int n; // Quantos valores 'a' pode assumir
//     double a; // Valores da constante que será substituída na fórmula
//     double precisao; // Valor de precisão que será comparado para a execução dos métodos
//     double d0 = 0.5;

//     cout << "Insira aqui o número inteiro de valores (n) que 'a' poderá assumir:" << endl;
//     cin >> n;

//     cout << "Insira aqui o valor 'a':" << endl;
//     cin >> a;

//     cout << "Insira a precisao (ex: 0.0001):" << endl;
//     cin >> precisao; // <-- CORREÇÃO: Faltava ler a precisão

//     ResultadoMetodo res_nr = newtonRaphson(a, d0, 0, precisao);

//     if(res_nr.sucesso){
//         // Imprimindo os valores de Newton-Raphson
//         cout << "--- Newton-Raphson ---" << endl;
//         cout << "Raiz (d): " << res_nr.raiz << endl;
//         cout << "Iteracoes: " << res_nr.iteracoes << endl;
//         cout << "Erro final: " << res_nr.erro_final << endl;
//         cout << "------------------------" << endl;
//     } else {
//         cout << "--- ERRO no método *Newton-Raphson* ---" << endl;
//         cout << "Falha: " << res_nr.mensagem_erro << endl;
//     }

// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// // P/Método de Newton modificado, usamos sempre um mesmo denominador, então salvamos na main para otimização
//     double derivadaX0 = funcao_derivada(a, d0);

//     ResultadoMetodo res_nm = newtonModificado(a, d0, 0, precisao, derivadaX0);

//     if (res_nm.sucesso) {
//         // Imprimindo os valores de Newton Modificado
//         cout << "--- Newton Modificado ---" << endl;
//         cout << "Raiz (d): " << res_nm.raiz << endl;
//         cout << "Iteracoes: " << res_nm.iteracoes << endl;
//         cout << "Erro final: " << res_nm.erro_final << endl;
//         cout << "------------------------" << endl;
//     }
//     else {
//         cout << "ERRO no método Newton Modificado" << endl;
//         cout << "Falha: " << res_nm.mensagem_erro << endl;
//     }

// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//     // Para o método da Secante, vamos assumir um d1 para que haja um intervalo possível para a análise
//     float d1 = 0.6;
//     ResultadoMetodo res_sec = secante(a, d0, d1, 0, precisao);

//     if(res_sec.sucesso){
//         // Imprimindo os valores de Newton Modificado
//         cout << "--- Secante ---" << endl;
//         cout << "Raiz (d): " << res_sec.raiz << endl;
//         cout << "Iteracoes: " << res_sec.iteracoes << endl;
//         cout << "Erro final: " << res_sec.erro_final << endl;
//         cout << "------------------------" << endl;
//     } else {
//         cout << "--- ERRO no método *Secante* ---" << endl;
//         cout << "Falha: " << res_sec.mensagem_erro << endl;
//     }

//     return 0;
// }

// /* ====================================================================
//    COLE ISSO NO FINAL DO SEU ARQUIVO, DEPOIS DA SUA FUNÇÃO 'main'
//    ====================================================================
// */

// // O 'main' que você escreveu só roda no console.
// // Para a GUI, não precisamos dele. O JS será o nosso "novo main".

// // Inicia o bloco que será "exportado" para o JavaScript
// extern "C" {

// // Função "tradutora" que o JS vai chamar
// // EMSCRIPTEN_KEEPALIVE garante que o compilador não otimize e remova esta função
// EMSCRIPTEN_KEEPALIVE
// const char* calcularNewtonRaphson(double a, double d0, double precisao) {
//     // 1. Chama sua função C++ original
//     ResultadoMetodo res = newtonRaphson(a, d0, 0, precisao);

//     // 2. Constrói uma string JSON com o resultado
//     stringstream ss;
//     ss << "{";
//     ss << "\"sucesso\": " << (res.sucesso ? "true" : "false") << ",";
//     ss << "\"raiz\": " << res.raiz << ",";
//     ss << "\"iteracoes\": " << res.iteracoes << ",";
//     ss << "\"erro_final\": " << res.erro_final << ",";
//     ss << "\"mensagem_erro\": \"" << res.mensagem_erro << "\"";
//     ss << "}";

//     // 3. Converte a string para um formato que o Emscripten entende
//     // (Isso é um pouco complexo, mas é o padrão)
//     string json_str = ss.str();
//     char* resultado_wasm = new char[json_str.length() + 1];
//     strcpy(resultado_wasm, json_str.c_str());
    
//     // 4. Retorna a string JSON para o JavaScript
//     return resultado_wasm;
// }

// EMSCRIPTEN_KEEPALIVE
// const char* calcularNewtonModificado(double a, double d0, double precisao) {
//     // Calcula o f'(d0)
//     double derivadaX0 = funcao_derivada(a, d0);
    
//     // Chama sua função C++ original
//     ResultadoMetodo res = newtonModificado(a, d0, 0, precisao, derivadaX0);

//     // Constrói e retorna o JSON (mesma lógica acima)
//     stringstream ss;
//     ss << "{";
//     ss << "\"sucesso\": " << (res.sucesso ? "true" : "false") << ",";
//     ss << "\"raiz\": " << res.raiz << ",";
//     ss << "\"iteracoes\": " << res.iteracoes << ",";
//     ss << "\"erro_final\": " << res.erro_final << ",";
//     ss << "\"mensagem_erro\": \"" << res.mensagem_erro << "\"";
//     ss << "}";
    
//     string json_str = ss.str();
//     char* resultado_wasm = new char[json_str.length() + 1];
//     strcpy(resultado_wasm, json_str.c_str());
//     return resultado_wasm;
// }

// EMSCRIPTEN_KEEPALIVE
// const char* calcularSecante(double a, double d0, double d1, double precisao) {
//     // Chama sua função C++ original
//     ResultadoMetodo res = secante(a, d0, d1, 0, precisao);

//     // Constrói e retorna o JSON (mesma lógica acima)
//     stringstream ss;
//     ss << "{";
//     ss << "\"sucesso\": " << (res.sucesso ? "true" : "false") << ",";
//     ss << "\"raiz\": " << res.raiz << ",";
//     ss << "\"iteracoes\": " << res.iteracoes << ",";
//     ss << "\"erro_final\": " << res.erro_final << ",";
//     ss << "\"mensagem_erro\": \"" << res.mensagem_erro << "\"";
//     ss << "}";
    
//     string json_str = ss.str();
//     char* resultado_wasm = new char[json_str.length() + 1];
//     strcpy(resultado_wasm, json_str.c_str());
//     return resultado_wasm;
// }

// } // Fim do 'extern "C"'