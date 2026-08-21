#include "mspso.hpp"
#include <iostream>

MSPSO::MSPSO(std::shared_ptr<Problem> p, std::shared_ptr<ConstraintHandler> ch, 
             int nSwarms, int sSize, int maxEvals)
    : problem(p), penaltyMethod(ch), numSwarms(nSwarms), swarmSize(sSize), 
      maxEvaluations(maxEvals), currentEvaluations(0) {
    
    dimensions = problem->getBounds().size();
    
    // Inicializa o Rastreador do Enxame (APM)
    globalBestFitness = std::numeric_limits<double>::infinity();
    globalBestPosition.resize(dimensions, 0.0);
    globalBestFeasible = false;

    // Inicializa o Rastreador Estrito (Realidade Física)
    strictGlobalBestFitness = std::numeric_limits<double>::infinity();
    strictGlobalBestPosition.resize(dimensions, 0.0);
    
    std::random_device rd;
    rng.seed(rd());
    
    for(int i = 0; i < numSwarms; ++i) {
        swarms.emplace_back(swarmSize, dimensions);
    }
}

void MSPSO::initialize() {
    auto bounds = problem->getBounds();
    for (auto& swarm : swarms) {
        for (auto& p : swarm.particles) {
            for (int d = 0; d < dimensions; ++d) {
                std::uniform_real_distribution<double> dist(bounds[d].first, bounds[d].second);
                p.position[d] = dist(rng);
                
                double vMax = (bounds[d].second - bounds[d].first) * 0.1;
                std::uniform_real_distribution<double> vDist(-vMax, vMax);
                p.velocity[d] = vDist(rng);
            }
            problem->applyMixedVariableConstraints(p.position);
            p.pbest_position = p.position;
        }
    }
}

void MSPSO::evaluateAndPenalize() {
    std::vector<Particle*> all_particles_ptrs;
    int feasible_count = 0;
    int total_particles = 0;

    for (auto& swarm : swarms) {
        for (auto& p : swarm.particles) {
            if (currentEvaluations >= maxEvaluations) break;
            
            p.fitness = problem->evaluate(p.position, p.violations);
            currentEvaluations++;
            all_particles_ptrs.push_back(&p);

            // Verificação de factibilidade para o Log
            bool is_feasible = true;
            for(double v : p.violations) {
                if (v > 0) {
                    is_feasible = false;
                    break;
                }
            }
            p.feasible = is_feasible;
            if (is_feasible) feasible_count++;
            total_particles++;
        }
    }

    // Armazena a porcentagem de factibilidade desta iteração
    if (total_particles > 0) {
        feasibilityHistory.push_back((double)feasible_count / total_particles);
    }

    std::vector<Particle> all_particles_copy;
    for(auto ptr : all_particles_ptrs) {
        all_particles_copy.push_back(*ptr);
    }

    penaltyMethod->applyPenalty(all_particles_copy);

    for(size_t i = 0; i < all_particles_ptrs.size(); ++i) {
        all_particles_ptrs[i]->penalized_fitness = all_particles_copy[i].penalized_fitness;
    }
}

void MSPSO::updateBests() {
    // Regra de Deb: um indivíduo factível é sempre melhor que um infactível,
    // independente do fitness penalizado. Entre dois factíveis, compara-se o
    // fitness real f(x). Entre dois infactíveis, compara-se o fitness
    // penalizado (APM). Sem essa regra, o pbest/gbest pode ser "capturado"
    // por um ótimo infactível com baixa penalização, e o enxame nunca é
    // puxado de volta para a região factível (é exatamente o que acontecia:
    // taxa de factibilidade da população ficava achatada em ~1% a vida toda).
    auto is_better_than = [](bool feas_a, double val_a, bool feas_b, double val_b) {
        if (feas_a && !feas_b) return true;
        if (!feas_a && feas_b) return false;
        return val_a < val_b; // ambos factíveis (compara f) ou ambos infactíveis (compara F)
    };

    for (auto& swarm : swarms) {
        for (auto& p : swarm.particles) {
            double p_val = p.feasible ? p.fitness : p.penalized_fitness;
            double pbest_val = p.pbest_feasible ? p.pbest_fitness : p.pbest_fitness; // já armazenado corretamente

            // 1. Rastreador Exploratório (agora respeita a regra de Deb)
            if (is_better_than(p.feasible, p_val, p.pbest_feasible, pbest_val)) {
                p.pbest_fitness = p_val;
                p.pbest_position = p.position;
                p.pbest_feasible = p.feasible;
            }
            if (is_better_than(p.feasible, p_val, swarm.local_best_feasible, swarm.local_best_fitness)) {
                swarm.local_best_fitness = p_val;
                swarm.local_best_position = p.position;
                swarm.local_best_feasible = p.feasible;
            }

            // 2. Rastreador Estrito (Apenas soluções perfeitamente seguras)
            if (p.feasible && p.fitness < strictGlobalBestFitness) {
                strictGlobalBestFitness = p.fitness;
                strictGlobalBestPosition = p.position;
            }
        }
        
        // Atualiza a Mente de Colmeia exploratória
        if (is_better_than(swarm.local_best_feasible, swarm.local_best_fitness,
                            globalBestFeasible, globalBestFitness)) {
            globalBestFitness = swarm.local_best_fitness;
            globalBestPosition = swarm.local_best_position;
            globalBestFeasible = swarm.local_best_feasible;
        }
    }
}

void MSPSO::updateParticles() {
    double w = 0.9 - 0.5 * ((double)currentEvaluations / maxEvaluations);
    double c1 = 2.05, c2 = 2.05;
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    auto bounds = problem->getBounds();

    for (auto& swarm : swarms) {
        for (auto& p : swarm.particles) {
            for (int d = 0; d < dimensions; ++d) {
                double r1 = dist(rng);
                double r2 = dist(rng);
                
                p.velocity[d] = w * p.velocity[d] + 
                                c1 * r1 * (p.pbest_position[d] - p.position[d]) + 
                                c2 * r2 * (swarm.local_best_position[d] - p.position[d]);

                // Clamping de velocidade (essencial pois c1+c2=4.1>4, sem fator
                // de constrição — sem isso a velocidade diverge estruturalmente)
                double vMax = (bounds[d].second - bounds[d].first) * 0.2;
                if (p.velocity[d] > vMax) p.velocity[d] = vMax;
                if (p.velocity[d] < -vMax) p.velocity[d] = -vMax;

                p.position[d] += p.velocity[d];

                if (p.position[d] < bounds[d].first) p.position[d] = bounds[d].first;
                if (p.position[d] > bounds[d].second) p.position[d] = bounds[d].second;
            }
            problem->applyMixedVariableConstraints(p.position);
        }
    }
}

void MSPSO::shareInformation() {
    for(auto& swarm : swarms) {
        for (int d = 0; d < dimensions; ++d) {
            swarm.local_best_position[d] = 0.8 * swarm.local_best_position[d] + 0.2 * globalBestPosition[d];
        }
        problem->applyMixedVariableConstraints(swarm.local_best_position);
    }
}

void MSPSO::optimize() {
    initialize();
    
    int epochs = 0;
    while (currentEvaluations < maxEvaluations) {
        evaluateAndPenalize();
        updateBests();
        updateParticles();
        
        epochs++;
        if (epochs % 10 == 0) { 
            shareInformation();
        }
    }
}

double MSPSO::getGlobalBestFitness() const { 
    // Garante que o relatório final sempre imprima apenas um projeto válido
    return strictGlobalBestFitness; 
}

std::vector<double> MSPSO::getGlobalBestPosition() const { 
    return strictGlobalBestPosition; 
}

const std::vector<double>& MSPSO::getFeasibilityHistory() const {
    return feasibilityHistory;
}