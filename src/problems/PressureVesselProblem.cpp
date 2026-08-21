#include "PressureVesselProblem.hpp"
#include <cmath>

std::string PressureVesselProblem::getName() const { 
    return "Vaso de Pressao"; 
}

double PressureVesselProblem::evaluate(const std::vector<double>& pos, std::vector<double>& violations) const {
    double Ts = pos[0], Th = pos[1], R = pos[2], L = pos[3];

    // Função Objetivo: Peso
    double W = 0.6224 * Ts * R * L + 1.7781 * Th * (R * R) + 3.1661 * (Ts * Ts) * L
               + 19.84 * (Ts * Ts) * R;

    // Constante Pi compatível com diferentes compiladores
    const double pi = std::acos(-1.0);

    // Restrições (g <= 0)
    violations.clear();
    violations.push_back(0.0193 * R - Ts);
    violations.push_back(0.00954 * R - Th);
    violations.push_back(1296000.0 - (pi * (R * R) * L + (4.0 / 3.0) * pi * std::pow(R, 3)));
    violations.push_back(L - 240.0);

    return W;
}

std::vector<std::pair<double, double>> PressureVesselProblem::getBounds() const {
    return {{0.0625, 5.0}, {0.0625, 5.0}, {10.0, 200.0}, {10.0, 200.0}}; //
}

void PressureVesselProblem::applyMixedVariableConstraints(std::vector<double>& pos) const {
    // Ts e Th possuem degraus constantes de 0.0625.
    // Usa arredondamento para o degrau mais próximo (round), não ceil:
    // ceil viesava sistematicamente para paredes mais grossas (mais peso),
    // afastando a busca da região mais fina onde está o ótimo real.
    pos[0] = std::round(pos[0] / 0.0625) * 0.0625;
    pos[1] = std::round(pos[1] / 0.0625) * 0.0625;
}