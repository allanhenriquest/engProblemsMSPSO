#ifndef MSPSO_HPP
#define MSPSO_HPP

#include "swarm.hpp"
#include "constraintHandler.hpp"
#include "../problems/problem.hpp"
#include <memory>
#include <random>
#include <vector>

class MSPSO {
private:
    std::shared_ptr<Problem> problem;
    std::shared_ptr<ConstraintHandler> penaltyMethod;
    std::vector<Swarm> swarms;
    
    int numSwarms;
    int swarmSize;
    int maxEvaluations;
    int currentEvaluations;
    int dimensions;
    
    // Rastreador guiado pelo APM (Usado para movimentar o enxame, pode conter infactíveis)
    std::vector<double> globalBestPosition;
    double globalBestFitness;
    bool globalBestFeasible;

    // Rastreador Estrito (Apenas soluções perfeitamente factíveis)
    std::vector<double> strictGlobalBestPosition;
    double strictGlobalBestFitness;

    // Histórico da taxa de factibilidade por avaliação
    std::vector<double> feasibilityHistory;

    std::mt19937 rng;

public:
    MSPSO(std::shared_ptr<Problem> p, std::shared_ptr<ConstraintHandler> ch, 
          int nSwarms, int sSize, int maxEvals);

    void optimize();
    
    // Retorna os dados do Rastreador Estrito para garantir validade física
    double getGlobalBestFitness() const;
    std::vector<double> getGlobalBestPosition() const;
    
    // Retorna o log de factibilidade
    const std::vector<double>& getFeasibilityHistory() const;

private:
    void initialize();
    void updateParticles();
    void evaluateAndPenalize();
    void updateBests();
    void shareInformation();
};

#endif // MSPSO_HPP