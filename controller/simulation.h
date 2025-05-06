#ifndef CONTROLLER_SIMULATION_H
#define CONTROLLER_SIMULATION_H

#include "../model/simulation.h"
#include "../model/options.h"

void populateSimulation(Simulation* _simulation);
void runSimulation(Simulation* _simulation, Options* _options);
void logSimulation(Simulation* _simulation, Options* _options);

#endif
