#ifndef SWARM_HPP
#define SWARM_HPP

#include <vector>
#include "particle.hpp"

class Swarm {
public:
    std::vector<Particle> particles;
    std::vector<double> local_best_position;
    double local_best_fitness;
    bool local_best_feasible;

    Swarm(int num_particles, int dimensions) {
        for(int i = 0; i < num_particles; ++i) {
            particles.emplace_back(dimensions);
        }
        local_best_position.resize(dimensions, 0.0);
        local_best_fitness = std::numeric_limits<double>::infinity();
        local_best_feasible = false;
    }
};

#endif // SWARM_HPP