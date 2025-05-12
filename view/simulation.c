#include "simulation.h"

/*** * * ***/

#include <stdio.h>

#include "../model/constants.h"
#include "../model/options.h"
#include "../model/simulation.h"

/*** * * ***/

void logSimulation(Simulation* _simulation, Options *_options) {
    // header
    if (_options->logHeader) {
        printf("Host Reveal,Player Change,Wins,Curtains,Games\n");
    }

    // data
    for (int s = 0; s < CONST_STATES_SIZE; s++) {
        printf(
            "%d,%d,%d,%d,%d\n",

            _simulation->states[s].doHostReveal,
            _simulation->states[s].doPlayerChange,
            _simulation->states[s].playerWinsCount,
            _options->curtainsSize,
            _simulation->states[s].gamesCount
        );
    }
}
