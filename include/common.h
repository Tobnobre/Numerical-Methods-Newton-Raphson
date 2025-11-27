#ifndef COMMON_H
#define COMMON_H

#include <string>

// Header do struct de resultados
struct ResultadoMetodo {
    double raiz;
    int iteracoes;
    double erro_final;
    bool sucesso;
    std::string mensagem_erro;
};

#endif