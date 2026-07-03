# cryptKX

cryptKX é um pequeno gerador de senhas desktop criado com C++20, Qt 6/QML e libsodium.

O [README.md](../../README.md) em inglês é a documentação principal do projeto. Este arquivo é a tradução em PT-BR.

## Funcionalidades

- Gera senhas locais com tamanho configurável de 8 a 64 caracteres;
- Usa 20 caracteres por padrão;
- Permite habilitar letras maiúsculas, letras minúsculas, dígitos e símbolos;
- Usa a inicialização da libsodium e `randombytes_uniform()` para seleção aleatória adequada para uso criptográfico;
- Expõe um controller Qt para o QML e permite copiar a senha gerada para a área de transferência do sistema;
- Mantém testes do core registrados pelo CTest.

## Requisitos

O ambiente principal de desenvolvimento no Windows é **MSYS2 MinGW64** com Ninja, Qt 6 e libsodium.

Instale os pacotes necessários em um terminal **MSYS2 MinGW64**:

```bash
pacman -S --needed base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja mingw-w64-x86_64-pkgconf mingw-w64-x86_64-libsodium mingw-w64-x86_64-qt6-base mingw-w64-x86_64-qt6-declarative mingw-w64-x86_64-qt6-tools
```

O projeto exige:

- CMake 3.21 ou mais recente;
- Um compilador MinGW64 com suporte a C++20;
- Ninja;
- Qt 6.5 ou mais recente com Core, Gui, Quick, Qml e QuickControls2;
- libsodium.

Se você compilar, testar ou executar pelo PowerShell em vez do terminal MSYS2 MinGW64, coloque o diretório de binários do **MinGW64** no início do `PATH system` da sessão atual do PowerShell no qual você esteja trabalhando:

```powershell
$env:PATH = "C:\msys64\mingw64\bin;$env:PATH"
```

## Build

A partir de um terminal MSYS2 MinGW64, ou do PowerShell depois de configurar o `PATH` mostrado acima:

```bash
cmake -B build -S . -G Ninja -DCRYPTKX_BUILD_APP=ON
cmake --build build
```

`CRYPTKX_BUILD_APP=ON` compila o executável desktop Qt/QML. O padrão também é `ON` quando `src/main.cpp` existe.

## Execução

Execute a aplicação compilada a partir do mesmo ambiente usado no build, ou pelo PowerShell com `C:\msys64\mingw64\bin` no `PATH`:

```bash
.\build\cryptkx.exe
```

Se você executar o binário fora de um terminal MSYS2 MinGW64 ou de uma sessão PowerShell com o `PATH` correto, as DLLs necessárias do Qt, QML, runtime do MinGW e libsodium precisam estar disponíveis no `PATH` ou empacotadas manualmente ao lado do executável. Isso inclui as DLLs do Qt, diretórios de QML/plugins, DLLs do runtime MinGW e `libsodium-*.dll`.

## Teste do Aplicativo Completo

O repositório atualmente registra testes do core pelo CTest. Para uma verificação do aplicativo completo, compile o app, rode os testes e depois abra o executável Qt/QML:

```bash
cmake -B build -S . -G Ninja -DCRYPTKX_BUILD_APP=ON -DBUILD_TESTING=ON
cmake --build build
ctest --test-dir build --output-on-failure
.\build\cryptkx.exe
```

Use esta lista de verificação manual para o app desktop:

- A janela do cryptKX abre sem erros de importação QML.
- O tamanho padrão da senha é 20.
- O controle de tamanho aceita valores de 8 a 64.
- As opções de maiúsculas, minúsculas, dígitos e símbolos podem ser alternadas.
- A geração de senha atualiza a senha visível.
- Copiar a senha coloca o valor na área de transferência do sistema.

## Teste Somente do Core

Use um build somente do core quando quiser testar a lógica de geração de senhas sem exigir Qt:

```bash
cmake -B build-core -S . -G Ninja -DCRYPTKX_BUILD_APP=OFF -DBUILD_TESTING=ON
cmake --build build-core
ctest --test-dir build-core --output-on-failure
```

Isso compila `cryptkx_core` e `cryptkx_core_tests`, depois executa a suíte pelo CTest.

## Estrutura do Projeto

Essa é a estrutura do projeto atual:
- `src/core`: lógica pura de geração de senhas, independente de Qt e de acesso ao clipboard;
- `src/app`: controller Qt que adapta chamadas do QML para o gerador do core e para o clipboard;
- `src/qml`: interface de usuário Qt Quick/QML;
- `tests`: testes do core registrados com CTest;
- `cmake`: módulos auxiliares do CMake, incluindo descoberta da libsodium;
- `docs/`: documentação do projeto em inglês e português.

## Notas de Segurança

Antes de usar o cryptKX, leia as notas de segurança:
- As senhas são geradas localmente pela aplicação;
- O cryptKX não armazena senhas geradas em disco;
- O gerador valida tamanho e grupos selecionados antes de gerar;
- A libsodium é inicializada antes da geração aleatória e `randombytes_uniform()` é usado para evitar viés de módulo ao selecionar caracteres;
- Evite substituir a geração baseada em libsodium por geradores previsíveis como `rand()` ou `std::mt19937`;
- Senhas copiadas são colocadas na área de transferência do sistema operacional. Ferramentas de histórico de clipboard ou outros aplicativos podem conseguir lê-las.

Para mais detalhes, veja [Notas de segurança](docs/pt-br/security-notes.md).