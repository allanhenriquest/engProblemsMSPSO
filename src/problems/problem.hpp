#ifndef PROBLEM_HPP
#define PROBLEM_HPP

#include <vector>
#include <string>

class Problem{
public:
    virtual ~Problem() = default;

    // Return problem s name for logging
    virtual std::string getName() const = 0;
    
    //Evaluates objective function and fills restraint array (g_j)
    virtual double evaluate(const std::vector<double>& position, std::vector<double>& violations) const = 0;

    //Returns boudaries [min,max] for each variable
    virtual std::vector<std::pair<double, double>> getBounds() const = 0;
    
    //Fix mix variables (discrete/integer) before evaluation
    virtual void applyMixedVariableConstraints(std::vector<double>& position) const = 0;
};

#endif