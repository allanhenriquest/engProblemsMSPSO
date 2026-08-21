# MSPSO-APM: Otimização por Enxames de Múltiplas Ilhas com Método de Penalidade Adaptativa

Este repositório contém a implementação em **C++** do algoritmo **MSPSO (Multi-Swarm Particle Swarm Optimization)** integrado ao **APM (Adaptive Penalty Method)**.

O projeto tem como objetivo resolver problemas complexos de **otimização restrita em engenharia**, buscando soluções factíveis e de boa qualidade por meio da **Regra de Deb**, do tratamento adaptativo de penalidades e do controle rigoroso dos limites das variáveis de projeto.

## 🚀 Funcionalidades

* **Arquitetura Multi-Swarm (MSPSO):** o enxame é dividido em 5 ilhas independentes, reduzindo o risco de convergência prematura e permitindo migrações periódicas entre as populações.
* **Adaptive Penalty Method (APM):** tratamento de restrições não lineares por meio de penalização adaptativa, reduzindo a necessidade de calibração manual de parâmetros.
* **Paralelismo com OpenMP:** execução paralela de múltiplas rodadas independentes, permitindo uma análise estatística mais robusta dos resultados.
* **Rastreador Estrito de Factibilidade:** garante que a melhor solução reportada respeite as restrições do problema e os limites das variáveis de projeto.
* **Geração de Logs:** exportação automática dos resultados para arquivos `.csv`, incluindo informações de factibilidade, resultados brutos, estatísticas e vetores das variáveis de projeto.

## 📂 Problemas de Engenharia Implementados

O algoritmo é testado em quatro benchmarks clássicos de otimização em engenharia:

1. **Mola sob Tração/Compressão**

   * Objetivo: minimização do volume.
   * Variáveis: contínuas.

2. **Redutor de Velocidade**

   * Objetivo: minimização do peso.
   * Variáveis: mistas/discretas.

3. **Viga Soldada**

   * Objetivo: minimização do custo.
   * Variáveis: contínuas.

4. **Vaso de Pressão**

   * Objetivo: minimização do peso.
   * Variáveis: mistas.
   * Algumas variáveis possuem valores discretizados em incrementos de `0.0625`.

## 📁 Estrutura do Projeto

```text
.
├── CMakeLists.txt
└── src/
    ├── main.cpp
    ├── core/
    │   ├── apm.cpp
    │   ├── apm.hpp
    │   ├── mspso.cpp
    │   ├── mspso.hpp
    │   ├── particle.hpp
    │   ├── swarm.hpp
    │   └── constraintHandler.hpp
    │
    ├── problems/
    │   ├── problem.hpp
    │   ├── SpringProblem.*
    │   ├── ReducerProblem.*
    │   ├── WeldedBeamProblem.*
    │   └── PressureVesselProblem.*
    │
    └── utils/
        └── Stats.hpp
```

> Os arquivos indicados com `*` representam os arquivos `.cpp` e `.hpp` correspondentes.

## 🛠️ Como Compilar e Executar

O projeto pode ser compilado utilizando **CMake** e **OpenMP** em ambientes Linux ou **WSL (Windows Subsystem for Linux)**.

### 1. Instalar as dependências

Em distribuições baseadas em Ubuntu, execute:

```bash
sudo apt update
sudo apt install build-essential cmake libomp-dev
```

### 2. Obter o projeto

Caso o projeto tenha sido disponibilizado como um arquivo compactado:

```bash
unzip src.zip -d mspso_apm
cd mspso_apm
```

Caso esteja utilizando um repositório Git:

```bash
git clone <URL_DO_REPOSITORIO>
cd <DIRETORIO_DO_REPOSITORIO>
```

### 3. Criar o diretório de compilação

Na raiz do projeto, execute:

```bash
mkdir -p build
cd build
```

### 4. Configurar o CMake

Execute:

```bash
cmake ..
```

O CMake irá verificar as dependências e gerar os arquivos necessários para a compilação.

### 5. Compilar

Execute:

```bash
make -j$(nproc)
```

A opção `-j$(nproc)` permite utilizar os núcleos disponíveis do processador para acelerar a compilação.

### 6. Executar

Após a compilação, execute:

```bash
./mspso_apm
```

## 📊 Resultados

Durante a execução, o programa realiza múltiplas rodadas independentes do algoritmo. Os resultados podem ser utilizados para avaliar:

* melhor solução encontrada;
* pior solução encontrada;
* média das soluções;
* desvio padrão;
* factibilidade das soluções;
* número de avaliações da função objetivo;
* comportamento da convergência;
* variáveis de projeto da melhor solução.

Os arquivos `.csv` gerados pelo programa podem ser posteriormente utilizados para análise estatística e geração de gráficos.

## ⚙️ Paralelismo

O projeto utiliza **OpenMP** para executar rodadas independentes em paralelo.

A quantidade de threads pode ser controlada pela variável de ambiente:

```bash
export OMP_NUM_THREADS=8
```

Por exemplo:

```bash
export OMP_NUM_THREADS=8
./mspso_apm
```

Caso essa variável não seja definida, o comportamento dependerá da configuração padrão do OpenMP e da implementação utilizada pelo compilador.

## 🧠 Estratégia de Otimização

O MSPSO-APM combina três mecanismos principais:

1. **Múltiplos enxames:** diferentes populações exploram regiões distintas do espaço de busca.
2. **Migração:** informações entre as ilhas são compartilhadas periodicamente para favorecer a exploração e a exploração conjunta do espaço de soluções.
3. **Penalidade adaptativa:** soluções que violam restrições recebem um tratamento adaptativo, permitindo que o algoritmo considere simultaneamente a qualidade da função objetivo e a factibilidade.

A avaliação das soluções também considera a **Regra de Deb**, priorizando soluções factíveis em relação às inviáveis e, entre soluções factíveis, aquela com melhor valor da função objetivo.

## 📌 Requisitos

* **C++** com suporte ao padrão utilizado pelo projeto;
* **CMake**;
* **OpenMP**;
* Sistema operacional Linux ou WSL recomendado.

## 📄 Licença

Este projeto é destinado a fins acadêmicos e de pesquisa.
