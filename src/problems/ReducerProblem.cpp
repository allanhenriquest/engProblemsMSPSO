#include "ReducerProblem.hpp"
#include <cmath>

std::string ReducerProblem::getName() const { 
    return "Redutor de Velocidade"; 
}

double ReducerProblem::evaluate(const std::vector<double>& pos, std::vector<double>& violations) const {
    double b = pos[0], m = pos[1], n = pos[2], l1 = pos[3], l2 = pos[4], d1 = pos[5], d2 = pos[6];

    // Função Objetivo: Peso
    double W = 0.7854 * b * (m * m) * (3.3333 * (n * n) + 14.9334 * n - 43.0934) 
               - 1.5079 * b * (d1 * d1 + d2 * d2) + 7.4777 * (std::pow(d1, 3) + std::pow(d2, 3))
               + 0.7854 * (l1 * d1 * d1 + l2 * d2 * d2);

    // Restrições (g <= 0)
    violations.clear();
    violations.push_back(27.0 / (b * (m * m) * n) - 1.0);
    violations.push_back(397.5 / (b * (m * m) * (n * n)) - 1.0);
    violations.push_back((1.93 * std::pow(l1, 3)) / (m * n * std::pow(d1, 4)) - 1.0);
    violations.push_back((1.93 * std::pow(l2, 3)) / (m * n * std::pow(d2, 4)) - 1.0);
    violations.push_back(std::sqrt(std::pow(745.0 * l1 / (m * n), 2) + 16.9e6) / (0.1 * std::pow(d1, 3)) - 1100.0);
    violations.push_back(std::sqrt(std::pow(745.0 * l2 / (m * n), 2) + 157.5e6) / (0.1 * std::pow(d2, 3)) - 850.0);
    violations.push_back((m * n) / 40.0 - 1.0);
    violations.push_back(5.0 - b / m);
    violations.push_back(b / m - 12.0);
    violations.push_back((1.5 * d1 + 1.9) / l1 - 1.0);
    violations.push_back((1.1 * d2 + 1.9) / l2 - 1.0);

    return W;
}

std::vector<std::pair<double, double>> ReducerProblem::getBounds() const {
    return {
        {2.6, 3.6}, {0.7, 0.8}, {17.0, 28.0}, 
        {7.3, 8.3}, {7.8, 8.3}, {2.9, 3.9}, {5.0, 5.9}
    }; //
}

void ReducerProblem::applyMixedVariableConstraints(std::vector<double>& pos) const {
    // x3 (n) deve ser inteiro
    pos[2] = std::round(pos[2]);
}