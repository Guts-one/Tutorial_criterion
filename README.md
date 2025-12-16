# Criterion (C) — Mini Tutorial (WSL/Ubuntu)

Este mini tutorial documenta como eu instalei o Criterion, organizei um projeto simples (Calculator) e compilei/executei os testes, incluindo geração de relatórios (TAP/XML/JSON) e uso de skip via variável de ambiente.

## Estrutura do Projeto

Criei um projeto com a seguinte estrutura de pastas/arquivos:

```
criterion_calculator/
├─ src/
│  ├─ calculator.c
│  └─ calculator.h
└─ tests/
   └─ test_calculator_n.c
```

O SUT (System Under Test) expõe as funções no header `calculator.h` (addition, subtraction, multiplication, power, division).  
A implementação fica em `calculator.c`.  
A suíte de testes fica em `tests/test_calculator_n.c` e usa a nova API de asserts e testes parametrizados.

## Instalação do Criterion

### Primeira Instalação (WSL)

```
sudo apt update
sudo apt install -y build-essential pkg-config libcriterion-dev
```

### Instalação (Equivalente)

Obs.: em geral você vai precisar de sudo.

```
sudo apt-get update
sudo apt-get install -y libcriterion-dev
```

## Criar Pastas e Abrir o Projeto

```
mkdir -p criterion_calculator/{src,tests}
cd criterion_calculator
code .
```

## Compilar e Executar os Testes

### Recompilar do Zero (Apagando Binário Antigo)

Este comando compila apenas o arquivo de teste (`tests/test_calculator_n.c`) junto com o SUT (`src/calculator.c`) e gera o executável `run_tests`.

```
rm -f run_tests
gcc -Wall -Wextra -std=c11 \
  -Isrc \
  tests/test_calculator_n.c src/calculator.c \
  $(pkg-config --cflags --libs criterion) \
  -lm \
  -o run_tests
```

### Notas Rápidas

- `-Isrc` permite incluir `calculator.h` no teste.
- `-lm` é necessário porque o SUT usa `pow()` (math).
- `pkg-config --cflags --libs criterion` injeta os flags corretos de include/link do Criterion.

### Rodar os Testes

```
./run_tests
```

## Opções úteis de execução

### Verbose

```
./run_tests --verbose
```

### Falhar ao primeiro teste que falhar (fail-fast)

```
./run_tests -f
```

### Help / opções rápidas

```
./run_tests -h
```

## Geração de Relatórios (Reports)

O executável de testes aceita opções para exportar resultados em formatos comuns (útil para CI).

1. **Gerar TAP**

   ```
   ./run_tests --tap=report.tap
   ```

2. **Gerar JUnit XML**

   ```
   ./run_tests --xml=report.xml
   ```

3. **Gerar JSON**

   ```
   ./run_tests --json=report.json
   ```

Obs.: no meu caso o binário chama `run_tests` (se o seu binário tiver outro nome, ajuste o comando).

## Skip com “Modo Sanity” (SANITY_ONLY)

No arquivo de teste, eu controlo uma flag `sanityOnly` via variável de ambiente `SANITY_ONLY` lida na inicialização da suíte (`TestSuite(... .init = load_flags)`).  

Para executar com `SANITY_ONLY=1`:

```
SANITY_ONLY=1 ./run_tests
```

Exemplo (no meu teste parametrizado) de como eu “pulo” quando `sanityOnly` está ativo.  
Também existe um exemplo de `cr_skip(...)` direto dentro de um teste (sem condição), útil para desabilitar temporariamente um caso.
