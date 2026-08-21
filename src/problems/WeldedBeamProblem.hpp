#ifndef WELDED_BEAM_PROBLEM_HPP
#define WELDED_BEAM_PROBLEM_HPP

#include "problem.hpp"

class WeldedBeamProblem : public Problem {
public:
    std::string getName() const override;
    double evaluate(const std::vector<double>& pos, std::vector<double>& violations) const override;
    std::vector<std::pair<double, double>> getBounds() const override;
    void applyMixedVariableConstraints(std::vector<double>& position) const override;
};

#endif // WELDED_BEAM_PROBLEM_HPP