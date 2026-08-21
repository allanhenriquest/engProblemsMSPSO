#ifndef PRESSURE_VESSEL_PROBLEM_HPP
#define PRESSURE_VESSEL_PROBLEM_HPP

#include "problem.hpp"

class PressureVesselProblem : public Problem {
public:
    std::string getName() const override;
    double evaluate(const std::vector<double>& pos, std::vector<double>& violations) const override;
    std::vector<std::pair<double, double>> getBounds() const override;
    void applyMixedVariableConstraints(std::vector<double>& position) const override;
};

#endif // PRESSURE_VESSEL_PROBLEM_HPP