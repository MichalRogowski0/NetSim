#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "types.hpp"
#include "factory.hpp"

void simulate(Factory& f, TimeOffset d, std::function<void(Factory&, Time)> rf);

#endif //SIMULATION_HPP