Module.onRuntimeInitialized = () => {

    const calcularTudo = Module.cwrap('calcularTodosMetodos', 'string', ['string', 'number', 'number', 'number']);

    const calcButton = document.getElementById('btn_calcular');
    const inputN = document.getElementById('input_n');
    const aValuesContainer = document.getElementById('a_values_container');

    console.log('Elementos encontrados:', { calcButton, inputN, aValuesContainer });

    function gerarInputsA() {
        const n = parseInt(inputN.value, 10);
        console.log('Gerando inputs para n =', n);
        aValuesContainer.innerHTML = '';

        if (n > 0) { 
            for (let i = 0; i < n; i++) {
                const input = document.createElement('input');
                input.type = 'number';
                input.step = 'any';
                input.className = 'input_a_dinamico';
                input.placeholder = `a[${i+1}]`;
                
                input.value = 1 + i * 1; 
                
                aValuesContainer.appendChild(input);
            }
        }
    }

    inputN.addEventListener('input', gerarInputsA);
    gerarInputsA();

    calcButton.addEventListener('click', () => {
        const aInputs = document.querySelectorAll('.input_a_dinamico');
        const aValues = Array.from(aInputs).map(input => input.value);
        const inputTextoA = aValues.join(';');

        const d0 = parseFloat(document.getElementById('input_d0').value);
        const e = parseFloat(document.getElementById('input_e').value);
        const maxIterInput = parseFloat(document.getElementById('input_maxiter').value);
        const maxIter = (!isNaN(maxIterInput) && maxIterInput >= 1) ? maxIterInput : 1000;

        if (!inputTextoA || isNaN(d0) || isNaN(e)) {
            alert("Preencha os campos corretamente.");
            return;
        }

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
        let classe = dados.sucesso ? "sucesso" : "erro";
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