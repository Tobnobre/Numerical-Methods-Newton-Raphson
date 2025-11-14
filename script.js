// Espera o 'trabalho1.js' (nosso C++ compilado) terminar de carregar
Module.onRuntimeInitialized = () => {

    // 1. "Importa" as funções C++ que exportamos.
    // Module.cwrap('nome_js', 'tipo_retorno', ['tipo_arg1', 'tipo_arg2', ...])
    const calcNR = Module.cwrap('calcularNewtonRaphson', 'string', ['number', 'number', 'number']);
    const calcNM = Module.cwrap('calcularNewtonModificado', 'string', ['number', 'number', 'number']);
    const calcSec = Module.cwrap('calcularSecante', 'string', ['number', 'number', 'number', 'number']);

    // 2. Pega uma referência ao botão de calcular
    const calcButton = document.getElementById('btn_calcular');

    // 3. Adiciona um "ouvinte" de clique ao botão
    calcButton.addEventListener('click', () => {
        // 4. Pega os valores dos campos de input do HTML
        const a = parseFloat(document.getElementById('input_a').value);
        const d0 = parseFloat(document.getElementById('input_d0').value);
        const e = parseFloat(document.getElementById('input_e').value);
        const d1 = 0.6; // Valor fixo para o Secante, como no seu 'main'
        
        // --- Processa Newton-Raphson ---
        // 5. CHAMA A FUNÇÃO C++!
        const json_nr = calcNR(a, d0, e);
        // 6. O C++ retornou uma string JSON. Nós a convertemos para um objeto JS.
        const res_nr = JSON.parse(json_nr);
        // 7. Coloca os resultados na tabela HTML
        atualizarTabela('nr', res_nr);
        
        // --- Processa Newton Modificado ---
        const json_nm = calcNM(a, d0, e);
        const res_nm = JSON.parse(json_nm);
        atualizarTabela('nm', res_nm);

        // --- Processa Secante ---
        const json_sec = calcSec(a, d0, d1, e);
        const res_sec = JSON.parse(json_sec);
        atualizarTabela('sec', res_sec);
    });

    // Função auxiliar para preencher a tabela
    function atualizarTabela(prefixo, resultado) {
        // Pega as células da tabela (ex: 'nr_raiz', 'nr_iter', etc.)
        const raizEl = document.getElementById(prefixo + '_raiz');
        const iterEl = document.getElementById(prefixo + '_iter');
        const erroEl = document.getElementById(prefixo + '_erro');
        const statusEl = document.getElementById(prefixo + '_status');

        if (resultado.sucesso) {
            // Se deu certo, preenche os valores
            // .toFixed(8) formata o número para 8 casas decimais
            raizEl.textContent = resultado.raiz.toFixed(8);
            iterEl.textContent = resultado.iteracoes;
            // Notação para o formato: 0,m x 10^e
            // 1. Converte para string exponencial (ex: "3.17e-4") e quebra no "e"
            const parts = resultado.erro_final.toExponential(2).split('e');
            // 2. Monta o HTML com as tags <sup> (sobrescrito) e &times; (sinal de vezes)
            erroEl.innerHTML = parts[0] + ' &times; 10<sup>' + parts[1] + '</sup>';

            statusEl.textContent = "Sucesso";
            statusEl.className = ""; // Remove classe de erro
        } else {
            // Se deu erro, exibe a mensagem de erro
            raizEl.textContent = "-";
            iterEl.textContent = "-";
            erroEl.textContent = "-";
            statusEl.textContent = resultado.mensagem_erro;
            statusEl.className = "erro"; // Adiciona a classe CSS 'erro' (vermelha)
        }
    }
};