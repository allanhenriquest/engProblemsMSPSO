#ifndef CONSTRAINT_HANDLER_HPP
#define CONSTRAINT_HANDLER_HPP

#include <vector>
#include "particle.hpp"

class ConstraintHandler {
public:
    virtual ~ConstraintHandler() = default;

    // Recebe todas as partículas de todos os sub-enxames
    // para calcular médias globais e aplicar a penalização
    virtual void applyPenalty(std::vector<Particle>& all_particles) = 0;
};

#endif // CONSTRAINT_HANDLER_HPP