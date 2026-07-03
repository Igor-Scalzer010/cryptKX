# Notas de segurança

## Geradores inadequados

`rand()` e `std::mt19937` não devem ser usados para senhas. Eles foram desenhados para simulações, jogos, testes ou sorteios reproduzíveis, não para segredo. Mesmo quando parecem aleatórios, seus estados internos podem ser previstos ou reproduzidos com informação suficiente.

## CSPRNG

Senhas precisam de um CSPRNG (*Cryptographically Secure Pseudorandom Number Generator*), um gerador pseudoaleatório criptograficamente seguro. No cryptKX, essa responsabilidade fica com a libsodium. A família `randombytes_*` usa fontes seguras do sistema operacional e é adequada para gerar índices de caracteres.

Para escolher um caractere dentro de um alfabeto, prefira `randombytes_uniform(tamanhoDoAlfabeto)`. Essa função evita o viés de módulo que aparece quando um número aleatório grande é reduzido com `% tamanhoDoAlfabeto`.

## Clipboard

Quando o usuário copia a senha, ela passa para a área de transferência do sistema operacional. Outros aplicativos ou históricos de clipboard podem acessar esse conteúdo dependendo das configurações da máquina. Por isso, o clipboard deve ser tratado como uma conveniência, não como armazenamento seguro.

## Sem histórico

O aplicativo não deve manter histórico de senhas geradas, nem gravar senhas em disco, logs, arquivos temporários ou telemetria. A senha atual vive apenas na memória da aplicação e, quando copiada, no clipboard do sistema.

## Regras aplicadas no core

Algumas escolhas de design do core são aplicadas para manter a segurança da geração de senhas:
- Tamanho permitido: 8 a 64 caracteres;
- Valor padrão: 20 caracteres;
- Ao menos um grupo de caracteres precisa estar selecionado;
- A geração usa `randombytes_uniform()` para cada índice de caractere.
