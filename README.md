# MSPSO-APM: Otimização por Enxames de Múltiplas Ilhas com Método de Penalidade Adaptativa

Este repositório contém a implementação em C++ do algoritmo **MSPSO** (Multi-Swarm Particle Swarm Optimization) acoplado ao **APM** (Adaptive Penalty Method). O projeto tem como objetivo resolver problemas complexos de otimização restrita na engenharia, garantindo resultados físicos factíveis e precisos através da Regra de Deb e de um rigoroso rastreamento estrito de limites.

## 🚀 Funcionalidades
- **Arquitetura Multi-Swarm (MSPSO):** Enxame dividido em 5 ilhas isoladas para evitar convergência prematura, com migrações periódicas.
- **Adaptive Penalty Method (APM):** Lida com restrições altamente não-lineares sem a necessidade de calibração manual de parâmetros de punição (livre de *fine-tuning*).
- **Paralelismo (OpenMP):** Execução assíncrona e segura de múltiplas rodadas independentes para análise estatística robusta.
- **Rastreador Estrito:** Garante que a solução ótima global reportada seja sempre 100% fabricável e factível.
- **Geração de Logs (Tidy Data):** Exportação automática de logs de factibilidade, resultados brutos, estatísticas finais e vetores de variáveis de projeto em `.csv`.

## 📂 Problemas de Engenharia Implementados
O código testa e soluciona 4 *benchmarks* clássicos de engenharia mecânica:
1. **Mola sob Tração/Compressão** (Minimização de volume - Variáveis contínuas)
2. **Redutor de Velocidade** (Minimização de peso - Variáveis mistas/discretas)
3. **Viga Soldada** (Minimização de custo - Variáveis contínuas)
4. **Vaso de Pressão** (Minimização de peso - Variáveis mistas com degraus restritos a 0.0625)

## 📁 Estrutura do Projeto
.
├── CMakeLists.txt              # Script de configuração do CMake
└── src/
    ├── main.cpp                # Ponto de entrada, define orçamento (budget) e orquestra OpenMP
    ├── core/                   # Núcleo da meta-heurística
    │   ├── apm.cpp / .hpp      # Implementação do Adaptive Penalty Method
    │   ├── mspso.cpp / .hpp    # Implementação do Multi-Swarm PSO
    │   ├── particle.hpp        # Classe base das partículas do enxame
    │   ├── swarm.hpp           # Definição das Ilhas (Sub-enxames)
    │   └── constraintHandler.hpp # Interface para manipuladores de restrição
    ├── problems/               # Modelagem Físico-Matemática dos benchmarks
    │   ├── problem.hpp         # Classe abstrata base para os problemas
    │   ├── SpringProblem...    # Problema da Mola
    │   ├── ReducerProblem...   # Problema do Redutor
    │   ├── WeldedBeamProblem...# Problema da Viga Soldada
    │   └── PressureVesselProblem... # Problema do Vaso de Pressão
    └── utils/
        └── Stats.hpp           # Motor de extração e salvamento de logs em CSV

## 🛠️ Como Rodar o Programa (Passo a Passo)

O projeto foi configurado para ser compilado facilmente via terminal em ambientes Linux ou via WSL (*Windows Subsystem for Linux*). Siga os passos abaixo:

### Passo 1: Preparar as Dependências
Antes de compilar, você precisa garantir que o compilador C++, o CMake e a biblioteca de paralelismo (OpenMP) estejam instalados no seu sistema. 

Abra o seu terminal e rode o seguinte comando:
```bash
sudo apt update
sudo apt install build-essential cmake libomp-dev


# Se baixou o arquivo compactado:
unzip src.zip -d mspso_apm
cd mspso_apm

# 1. Crie uma pasta oculta para os arquivos de construção
mkdir build
cd build

# 2. Leia as configurações do CMakeLists.txt
cmake ..

# 3. Compile o executável
make

./mspso_apm