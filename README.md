# Trabalho de Métodos Numéricos (WASM)

Este projeto implementa métodos numéricos em C++ e os executa na web com WebAssembly.

## Como Compilar

1.  **Instale o Emscripten SDK:**
    Siga o guia oficial: [https://emscripten.org/docs/getting_started/downloads.html](https://emscripten.org/docs/getting_started/downloads.html)

2.  **Ative o SDK:**
    ```bash
    cd /caminho/para/o/emsdk
    source ./emsdk_env.sh
    ```

3.  **Compile o C++:**
    (Navegue para a pasta do projeto)
    ```bash
    emcc src/main.cpp src/math_funcs.cpp src/newton_raphson.cpp src/newton_modificado.cpp src/secante.cpp -I include -o public/trabalho1.js -s EXPORTED_RUNTIME_METHODS=['cwrap'] -s EXPORTED_FUNCTIONS=['_calcularTodosMetodos']
    ```

4.  **Inicie o Servidor Local:**
    (Navegue até a pasta *src/* dentro do projeto)
    ```bash
    python -m http.server
    ```

5.  **Acesse:** `http://localhost:8000`.