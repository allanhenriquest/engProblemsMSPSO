#ifndef APM_HPP
#define APM_HPP

#include "constraintHandler.hpp"
#include <cmath>
#include <algorithm>

class APM : public ConstraintHandler {
public:
    void applyPenalty(std::vector<Particle>& all_particles);
};

#endif // APM_HPP