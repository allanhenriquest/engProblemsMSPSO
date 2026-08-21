#include "WeldedBeamProblem.hpp"
#include <cmath>

std::string WeldedBeamProblem::getName() const { 
    return "Viga Soldada"; 
}

double WeldedBeamProblem::evaluate(const std::vector<double>& pos, std::vector<double>& violations) const {
    double h = pos[0], l = pos[1], t = pos[2], b = pos[3];

    // Função Objetivo: Custo
    double C = 1.10471 * (h * h) * l + 0.04811 * t * b * (14.0 + l);

    // Termos para tensão
    double alpha = std::sqrt(0.25 * (l * l + std::pow(h + t, 2)));
    double tau_prime = 6000.0 / (std::sqrt(2.0) * h * l);
    double J = 2.0 * (0.707 * h * l * (l * l / 12.0 + 0.25 * std::pow(h + t, 2)));
    double tau_double_prime = (6000.0 * (14.0 + 0.5 * l) * alpha) / J;
    
    double tau = std::sqrt(std::pow(tau_prime, 2) + std::pow(tau_double_prime, 2) 
                 + (l * tau_prime * tau_double_prime) / alpha);
    
    double sigma = 504000.0 / (t * t * b);
    double Pc = 64746.022 * (1.0 - 0.0282346 * t) * t * std::pow(b, 3);
    double delta = 2.1952 / (std::pow(t, 3) * b);

    // Restrições (g <= 0)
    violations.clear();
    violations.push_back(tau - 13600.0);
    violations.push_back(sigma - 30000.0);
    violations.push_back(h - b);
    violations.push_back(6000.0 - Pc);
    violations.push_back(delta - 0.25);

    return C;
}

std::vector<std::pair<double, double>> WeldedBeamProblem::getBounds() const {
    return {{0.125, 10.0}, {0.1, 10.0}, {0.1, 10.0}, {0.1, 10.0}};
}

void WeldedBeamProblem::applyMixedVariableConstraints(std::vector<double>& /*pos*/) const {
    // Variáveis contínuas
}