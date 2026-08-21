#ifndef STATS_HPP
#define STATS_HPP

#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

class Stats {
public:
    static void saveReport(const std::string& problemName, std::vector<double>& results, const std::string& filename = "resultados_estatisticos.csv") {
        if (results.empty()) return;

        int total_runs = (int)results.size();

        // Segue a mesma convenção da dissertação (coluna "nesf"): as
        // estatísticas de Melhor/Mediana/Média/dp/Pior são calculadas apenas
        // sobre as execuções que efetivamente encontraram uma solução
        // estritamente factível (fitness finito). Execuções sem solução
        // factível dentro do orçamento de avaliações contam apenas no
        // denominador do nesf (k/N), sem contaminar a média/dp com "inf".
        std::vector<double> feasible_results;
        for (double v : results) {
            if (std::isfinite(v)) feasible_results.push_back(v);
        }

        std::sort(feasible_results.begin(), feasible_results.end());
        int nesf = (int)feasible_results.size();

        std::ofstream file;
        file.open(filename, std::ios::out | std::ios::app);

        if (!file.is_open()) {
            std::cerr << "[Erro] Nao foi possivel abrir o arquivo: " << filename << "\n";
            return;
        }

        file.seekp(0, std::ios::end);
        if (file.tellp() == 0) {
            file << "Problema,Execucoes,Melhor,Mediana,Media,Desvio_Padrao,Pior,nesf\n";
        }

        if (nesf == 0) {
            file << problemName << "," << total_runs << ",NA,NA,NA,NA,NA,0/" << total_runs << "\n";
            file.close();
            std::cout << "[Stats][Aviso] Nenhuma execucao factivel para '" << problemName << "'.\n";
            return;
        }

        double best = feasible_results.front();
        double worst = feasible_results.back();

        double sum = std::accumulate(feasible_results.begin(), feasible_results.end(), 0.0);
        double mean = sum / nesf;

        double median = 0.0;
        if (nesf % 2 == 0) {
            median = (feasible_results[nesf / 2 - 1] + feasible_results[nesf / 2]) / 2.0;
        } else {
            median = feasible_results[nesf / 2];
        }

        double sq_sum = 0.0;
        for (double val : feasible_results) {
            sq_sum += (val - mean) * (val - mean);
        }
        double std_dev = std::sqrt(sq_sum / nesf);

        file << problemName << ","
             << total_runs << ","
             << std::fixed << std::setprecision(5) << best << ","
             << median << ","
             << mean << ","
             << std::scientific << std::setprecision(5) << std_dev << ","
             << std::fixed << std::setprecision(5) << worst << ","
             << nesf << "/" << total_runs << "\n";

        file.close();
        std::cout << "[Stats] Resultados do problema '" << problemName << "' salvos em " << filename << " (nesf=" << nesf << "/" << total_runs << ").\n";
    }

    // Grava o fitness bruto de cada uma das execucoes independentes (incluindo
    // "inf" quando nenhuma solucao estritamente factivel foi encontrada dentro
    // do orcamento), para permitir auditoria/analise externa (ex.: histograma
    // de sucesso, nesf) sem reprocessar o binario.
    static void saveRawResults(const std::string& problemName, const std::vector<double>& results, const std::string& filename = "resultados_brutos.csv") {
        std::ofstream file;
        file.open(filename, std::ios::out | std::ios::app);
        if (!file.is_open()) {
            std::cerr << "[Erro] Nao foi possivel abrir o arquivo: " << filename << "\n";
            return;
        }
        file.seekp(0, std::ios::end);
        if (file.tellp() == 0) {
            file << "Problema,Execucao,Fitness,Factivel\n";
        }
        for (size_t i = 0; i < results.size(); ++i) {
            bool feasible = std::isfinite(results[i]);
            file << problemName << "," << (i + 1) << ",";
            if (feasible) {
                file << std::fixed << std::setprecision(6) << results[i];
            } else {
                file << "inf";
            }
            file << "," << (feasible ? 1 : 0) << "\n";
        }
        file.close();
    }

    // NOVO MÉTODO: Grava o histórico de factibilidade (evolução temporal)
    static void saveFeasibilityHistory(const std::string& problemName, 
                                       const std::vector<std::vector<double>>& allHistories, 
                                       const std::string& filename = "historico_factibilidade.csv") {
        if (allHistories.empty()) return;

        std::ofstream file;
        file.open(filename, std::ios::out | std::ios::app);

        if (!file.is_open()) {
            std::cerr << "[Erro] Nao foi possivel abrir o arquivo de historico: " << filename << "\n";
            return;
        }

        file.seekp(0, std::ios::end);
        if (file.tellp() == 0) {
            // Formato 'Tidy Data' ideal para processamento de gráficos
            file << "Problema,Execucao,Iteracao,TaxaFactibilidade\n";
        }

        for (size_t run = 0; run < allHistories.size(); ++run) {
            for (size_t iter = 0; iter < allHistories[run].size(); ++iter) {
                file << problemName << "," 
                     << (run + 1) << "," 
                     << (iter + 1) << "," 
                     << std::fixed << std::setprecision(4) << allHistories[run][iter] << "\n";
            }
        }

        file.close();
        std::cout << "[Stats] Log de factibilidade para '" << problemName << "' exportado com sucesso.\n";
    }

    // Grava o vetor de variaveis de projeto da melhor execucao (menor fitness
    // estritamente factivel) entre as N execucoes independentes de um problema.
    // Apenas relatorio/exportacao: nao interfere na logica de otimizacao.
    static void saveBestDesignVariables(const std::string& problemName,
                                         double bestFitness,
                                         const std::vector<double>& bestPosition,
                                         const std::string& filename = "melhores_variaveis.csv") {
        std::ofstream file;
        file.open(filename, std::ios::out | std::ios::app);

        if (!file.is_open()) {
            std::cerr << "[Erro] Nao foi possivel abrir o arquivo: " << filename << "\n";
            return;
        }

        file.seekp(0, std::ios::end);
        if (file.tellp() == 0) {
            file << "Problema,Fitness";
            for (size_t d = 0; d < bestPosition.size(); ++d) {
                file << ",x" << (d + 1);
            }
            file << "\n";
        }

        file << problemName << "," << std::fixed << std::setprecision(6) << bestFitness;
        for (double v : bestPosition) {
            file << "," << std::fixed << std::setprecision(6) << v;
        }
        file << "\n";

        file.close();
        std::cout << "[Stats] Melhores variaveis de projeto de '" << problemName << "' salvas em " << filename << ".\n";
    }
};

#endif // STATS_HPP