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
    (Navegue para a pasta raiz do projeto)
    ```bash
    emcc trabalho1.cpp -o trabalho1.js -s WASM=1 -s "EXPORTED_RUNTIME_METHODS=[\"cwrap\"]" -s INVOKE_RUN=0
    ```

4.  **Inicie o Servidor Local:**
    ```bash
    python -m http.server
    ```

5.  Acesse `http://localhost:8000`.