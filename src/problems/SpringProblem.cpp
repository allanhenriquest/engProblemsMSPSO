#include "SpringProblem.hpp"
#include <cmath>

std::string SpringProblem::getName() const { 
    return "Mola sob Tracao/Compressao"; 
}

double SpringProblem::evaluate(const std::vector<double>& pos, std::vector<double>& violations) const {
    double x1 = pos[0]; // N (espirais ativos)
    double x2 = pos[1]; // D (diâmetro da volta)
    double x3 = pos[2]; // d (diâmetro do arame)

    // Função Objetivo: Volume
    double volume = (x1 + 2.0) * x2 * (x3 * x3);

    // Restrições (g <= 0)
    violations.clear();
    violations.push_back(1.0 - (std::pow(x2, 3) * x1) / (71785.0 * std::pow(x3, 4)));
    
    double term1 = (4.0 * std::pow(x2, 2) - x3 * x2) / (12566.0 * (x2 * std::pow(x3, 3) - std::pow(x3, 4)));
    double term2 = 1.0 / (5108.0 * std::pow(x3, 2));
    violations.push_back(term1 + term2 - 1.0);
    
    violations.push_back(1.0 - (140.45 * x3) / (std::pow(x2, 2) * x1));
    violations.push_back((x2 + x3) / 1.5 - 1.0);

    return volume;
}

std::vector<std::pair<double, double>> SpringProblem::getBounds() const {
    return {{2.0, 15.0}, {0.25, 1.3}, {0.05, 2.0}}; //
}

void SpringProblem::applyMixedVariableConstraints(std::vector<double>& /*position*/) const {
    // Variáveis contínuas
}