#include "apm.hpp"
#include <cmath>
#include <algorithm>

void APM::applyPenalty(std::vector<Particle>& all_particles) {
    if (all_particles.empty()) return;

    int num_particles = all_particles.size();
    int num_constraints = all_particles[0].violations.size();

    double sum_fitness = 0.0;
    std::vector<double> sum_violations(num_constraints, 0.0);

    // 1. Calcula o somatório da função objetivo e das violações maiores que 0
    for (const auto& p : all_particles) {
        sum_fitness += p.fitness;
        for (int j = 0; j < num_constraints; ++j) {
            sum_violations[j] += std::max(0.0, p.violations[j]);
        }
    }

    double avg_fitness = sum_fitness / num_particles;
    std::vector<double> avg_violations(num_constraints, 0.0);
    double sum_avg_viol_sq = 0.0;

    // 2. Calcula as médias e o denominador da fórmula do coeficiente de penalização (k_j)
    for (int j = 0; j < num_constraints; ++j) {
        avg_violations[j] = sum_violations[j] / num_particles;
        sum_avg_viol_sq += (avg_violations[j] * avg_violations[j]);
    }

    std::vector<double> k_j(num_constraints, 0.0);
    if (sum_avg_viol_sq > 1e-10) {
        for (int j = 0; j < num_constraints; ++j) {
            k_j[j] = std::abs(avg_fitness) * (avg_violations[j] / sum_avg_viol_sq);
        }
    }

    // 3. Aplica a penalização em cada partícula
    for (auto& p : all_particles) {
        bool feasible = true;
        double penalty = 0.0;
        
        for (int j = 0; j < num_constraints; ++j) {
            if (p.violations[j] > 0) {
                feasible = false;
                penalty += k_j[j] * p.violations[j];
            }
        }

        if (feasible) {
            p.penalized_fitness = p.fitness;
        } else {
            // Utiliza o valor da função ou a média como referência para indivíduos infactíveis
            double f_ref = (p.fitness > avg_fitness) ? p.fitness : avg_fitness;
            p.penalized_fitness = f_ref + penalty;
        }
    }
}