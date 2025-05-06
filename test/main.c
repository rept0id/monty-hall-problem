#include <assert.h>

// utils
#include "../util/main.h"

// model
// model : enumStateType
#include "../model/enumStateType.h"

// simulation
#include "../model/simulation.h"
#include "../controller/simulation.h"

/*** * * ***/

void test() {
    // options
    Options options = {0};

    // simulation
    Simulation simulation = {0};

    /*** * * ***/

    // readFlags
    utilReadFlags(
        6, // argc
        (char*[]) {
            "program",
            "--c", "5",
            "--sg", "100",
            "--log-no-header"
        }, // argv
        &options // _options
    );
    // readFlags : assert : curtains
    assert(options.curtainsMaxIdx+1 == 5);
    // readFlags : assert : stateGames
    assert(options.stateGamesMaxIdx+1 == 100);
    // readFlags : assert : logHeader
    assert(options.logHeader == false);

    // simulation

    // simulation : populate
    populateSimulation(&simulation);
    // simulation : populate : STATE_TYPE_NOTHING
    assert(simulation.states[STATE_TYPE_NOTHING].doHostReveal == false);
    assert(simulation.states[STATE_TYPE_NOTHING].doPlayerChange == false);
    // simulation : populate : STATE_TYPE_DO_HOST_REVEAL
    assert(simulation.states[STATE_TYPE_DO_HOST_REVEAL].doHostReveal == true);
    assert(simulation.states[STATE_TYPE_DO_HOST_REVEAL].doPlayerChange == false);
    // simulation : populate : STATE_TYPE_DO_HOST_REVEAL_DO_PLAYER_SWITCH
    assert(simulation.states[STATE_TYPE_DO_HOST_REVEAL_DO_PLAYER_SWITCH].doHostReveal == true);
    assert(simulation.states[STATE_TYPE_DO_HOST_REVEAL_DO_PLAYER_SWITCH].doPlayerChange == true);
    // simulation : populate : STATE_TYPE_DO_PLAYER_CHANGE
    assert(simulation.states[STATE_TYPE_DO_PLAYER_CHANGE].doHostReveal == false);
    assert(simulation.states[STATE_TYPE_DO_PLAYER_CHANGE].doPlayerChange == true);

    // simulation : run
    runSimulation(&simulation, &options);
    // simulation : run : assert
    for (int s = 0; s <= CONST_STATES_MAX_IDX; s++) {
        // simulation : run : assert : gamesCount
        assert(simulation.states[s].gamesCount == options.stateGamesMaxIdx+1);
        // simulation : run : assert : playerWinsCount
        assert(simulation.states[s].playerWinsCount > 0);
        assert(simulation.states[s].playerWinsCount < options.stateGamesMaxIdx+1);
    }
}
