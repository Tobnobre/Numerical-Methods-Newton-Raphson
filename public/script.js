Module.onRuntimeInitialized = () => {

    const calcularTudo = Module.cwrap('calcularTodosMetodos', 'string', ['string', 'number', 'number', 'number']);

    const calcButton = document.getElementById('btn_calcular');
    const inputQtdA = document.getElementById('input_qtd_a');
    const containerInputs = document.getElementById('container_inputs_a');

    // Função para gerar os inputs dinamicamente
    function gerarCamposDeA() {
        containerInputs.innerHTML = ''; // Limpa os campos anteriores
        const qtd = parseInt(inputQtdA.value) || 0;

        for (let i = 0; i < qtd; i++) {
            const input = document.createElement('input');
            input.type = 'number';
            input.step = 'any';
            input.placeholder = `a${i+1}`;
            input.className = 'valor-a-individual';
            input.style.width = '100%';
            containerInputs.appendChild(input);

            if (i === 0) input.value = "1";
        }
    }

    // Gera os campos assim que mudar o número
    inputQtdA.addEventListener('input', gerarCamposDeA);
    
    // Gera os campos iniciais ao carregar a página
    gerarCamposDeA();

    calcButton.addEventListener('click', () => {
        // Coleta todos os inputs criados dinamicamente
        const inputsIndividuais = document.querySelectorAll('.valor-a-individual');
        
        // Cria um array com os valores e junta com ";"
        // Exemplo: se digitou 1, 2 e 5.5 -> vira "1;2;5.5"
        const valoresArray = Array.from(inputsIndividuais)
            .map(input => input.value)
            .filter(val => val.trim() !== ""); // Remove vazios
            
        const inputTextoA = valoresArray.join(';');

        const d0 = parseFloat(document.getElementById('input_d0').value);
        const e = parseFloat(document.getElementById('input_e').value);
        const maxIterInput = parseFloat(document.getElementById('input_maxiter').value);
        const maxIter = (!isNaN(maxIterInput) && maxIterInput >= 1) ? maxIterInput : 1000;

        // Validação
        if (valoresArray.length === 0 || isNaN(d0) || isNaN(e)) {
            alert("Preencha os campos corretamente.");
            return;
        }

        // O resto do código continua igual, pois o inputTextoA agora tem o formato que o C++ gosta
        const jsonResposta = calcularTudo(inputTextoA, d0, e, maxIter);
        const resultadosArray = JSON.parse(jsonResposta);

        const corpoTabela = document.getElementById('corpo_tabela');
        corpoTabela.innerHTML = '';

        resultadosArray.forEach(item => {
            adicionarLinha(item.valor_a, "Newton-Raphson", item.nr);
            adicionarLinha(item.valor_a, "Newton Modificado", item.nm);
            adicionarLinha(item.valor_a, "Secante", item.sec);
        });
    });

    function adicionarLinha(valorA, metodo, dados) {
        const corpoTabela = document.getElementById('corpo_tabela');
        const tr = document.createElement('tr');
        
        let displayRaiz = dados.sucesso ? dados.raiz.toFixed(8) : "-";
        let displayIter = dados.sucesso ? dados.iter : "-";
        let displayErro = "-";
        let classe = dados.sucesso ? "" : "erro";
        let msg = dados.sucesso ? "Sucesso" : dados.msg;

        if (dados.sucesso) {
            let parts = dados.erro.toExponential(2).split('e');
            displayErro = parts[0] + ' &times; 10<sup>' + parts[1] + '</sup>';
        }

        tr.innerHTML = `
            <td style="font-weight:bold; color: #a78bfa">${valorA}</td>
            <td>${metodo}</td>
            <td style="font-family: monospace">${displayRaiz}</td>
            <td>${displayIter}</td>
            <td>${displayErro}</td>
            <td class="${classe}">${msg}</td>
        `;
        corpoTabela.appendChild(tr);
    }
};