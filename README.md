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

---

## Precisão numérica e IEEE 754 (ponto flutuante)

As funções desta calculadora operam com `double`. Em C, esse tipo normalmente segue o padrão **IEEE 754**, o que implica que:
- Nem todo decimal é representável exatamente (ex.: `0.1`), então pequenas diferenças por arredondamento são esperadas.
- Comparações diretas com `==` podem falhar mesmo quando o resultado está correto do ponto de vista numérico.

Por esse motivo, **todos os testes que verificam resultados em ponto flutuante foram escritos usando as macros de comparação IEEE do Criterion**, em vez de igualdade exata. Em especial:

- **Comparação por ULP** (`ieee_ulp_eq(...)`): compara valores considerando a distância em *Units in the Last Place* (passos representáveis do IEEE 754).  
  Essa abordagem é adequada para resultados de operações aritméticas que podem sofrer variações mínimas de arredondamento.
- **Comparação por epsilon** (`epsilon_eq(...)`): compara valores com base em tolerância absoluta (`epsilon`).  
  É particularmente útil para resultados próximos de zero ou quando se deseja estabelecer explicitamente um erro máximo aceitável.

Com isso, a suíte de testes permanece **robusta** e **estável**, mesmo em cenários típicos de ponto flutuante onde o arredondamento é inevitável.

---

## Testes realizados

Os testes estão implementados no arquivo de testes do projeto (ex.: `tests/test_calculator_n.c`) e cobrem as operações da calculadora, **sempre validando resultados com asserts compatíveis com IEEE 754**:

### Casos cobertos
- **Adição (`addition`)**
  - Casos com valores simples e casos com casas decimais, validados com `ieee_ulp_eq(...)` e/ou `epsilon_eq(...)`.
- **Subtração (`subtraction`)**
  - Casos gerais e casos com resultado próximo de zero, validados com `epsilon_eq(...)` e/ou `ieee_ulp_eq(...)`.
- **Multiplicação (`multiplication`)** 
  - Casos gerais com validação IEEE (`ieee_ulp_eq(...)`).
  - Caso com parametrização dado um grupo de valores para validação IEEE (`ieee_ulp_eq(...)`).
- **Divisão (`division`)** 
  - Casos gerais com validação IEEE (`ieee_ulp_eq(...)`).
  - Casos em que a divisão é por zero (`isinf(...)`)
  - Casos em que a divisão gera um valor indeterminado NaN (`isnan(...)`)

### Observação sobre resultados “exatos”
Mesmo quando um resultado poderia ser comparado por igualdade exata (ex.: `2.0 + 2.0 = 4.0`), a suíte mantém o padrão e utiliza as comparações IEEE do Criterion de forma consistente, garantindo uniformidade e evitando dependência de detalhes de representação.

---


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
