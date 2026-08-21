#include <iostream>
#include <vector>
#include <memory>
#include <omp.h>

// Inclusão dos problemas
#include "problems/SpringProblem.hpp"
#include "problems/ReducerProblem.hpp"
#include "problems/WeldedBeamProblem.hpp"
#include "problems/PressureVesselProblem.hpp"

// Inclusão do Core e Utils
#include "core/apm.hpp"
#include "core/mspso.hpp"
#include "utils/Stats.hpp"

void runOptimization(std::shared_ptr<Problem> problem, int budget, int num_runs) {
    auto apm = std::make_shared<APM>();

    // Pré-alocações seguras para evitar Race Conditions no OpenMP
    std::vector<double> results(num_runs);
    std::vector<std::vector<double>> histories(num_runs);
    std::vector<std::vector<double>> positions(num_runs);

    int numSwarms = 5;
    int swarmSize = 10;

    std::cout << "Iniciando otimizacao para: " << problem->getName() << " usando OpenMP...\n";

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < num_runs; ++i) {

        MSPSO optimizer(problem, apm, numSwarms, swarmSize, budget);
        optimizer.optimize();

        // Coleta o rastreador estrito final e a matriz do histórico temporal
        results[i] = optimizer.getGlobalBestFitness();
        histories[i] = optimizer.getFeasibilityHistory();
        positions[i] = optimizer.getGlobalBestPosition();

        #pragma omp critical
        {
            std::cout << "Thread " << omp_get_thread_num() << " concluiu a execucao " << (i + 1) << ".\n";
        }
    }

    // Exportação em lote após o encerramento do pool de threads
    Stats::saveReport(problem->getName(), results, "resultados_estatisticos.csv");
    Stats::saveFeasibilityHistory(problem->getName(), histories, "historico_factibilidade.csv");
    Stats::saveRawResults(problem->getName(), results, "resultados_brutos.csv");

    // Identifica a execucao com o melhor (menor) fitness estritamente factivel
    // e salva o vetor de variaveis de projeto correspondente (apenas relatorio,
    // nao altera a logica do algoritmo).
    int bestIdx = 0;
    for (int i = 1; i < num_runs; ++i) {
        if (results[i] < results[bestIdx]) bestIdx = i;
    }
    Stats::saveBestDesignVariables(problem->getName(), results[bestIdx], positions[bestIdx], "melhores_variaveis.csv");
}

int main() {
    int num_runs = 35;

    runOptimization(std::make_shared<SpringProblem>(), 36000, num_runs);
    runOptimization(std::make_shared<ReducerProblem>(), 36000, num_runs);
    runOptimization(std::make_shared<WeldedBeamProblem>(), 320000, num_runs);
    runOptimization(std::make_shared<PressureVesselProblem>(), 80000, num_runs);

    std::cout << "\nTodas as otimizacoes e extracoes de logs temporais foram concluidas.\n";

    return 0;
}