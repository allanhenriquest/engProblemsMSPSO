#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <vector>
#include <limits>

class Particle {
public:
    std::vector<double> position;
    std::vector<double> velocity;
    std::vector<double> pbest_position;
    std::vector<double> violations;

    double fitness;
    double penalized_fitness;
    double pbest_fitness;      // guarda fitness real (se feasible) ou penalizado (se não)
    bool feasible;
    bool pbest_feasible;

    Particle(int dimensions) {
        position.resize(dimensions, 0.0);
        velocity.resize(dimensions, 0.0);
        pbest_position.resize(dimensions, 0.0);
        
        fitness = std::numeric_limits<double>::infinity();
        penalized_fitness = std::numeric_limits<double>::infinity();
        pbest_fitness = std::numeric_limits<double>::infinity();
        feasible = false;
        pbest_feasible = false;
    }
};

#endif // PARTICLE_HPP