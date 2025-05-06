#include <stdio.h>
#include <stdlib.h> // malloc, free
#include <assert.h>
#include <stdbool.h>

#include <string.h>

#include <omp.h>

#include <stdint.h>

#include "./model/constants.h"
#include "./model/constantsDefaults.h"
#include "./model/options.h"
#include "./model/simulation.h"
#include "./model/enumFlagType.h"
#include "./model/enumStateType.h"

#include "./controller/simulation.h"

/*** * * ***/

void setOptionsDefault(Options *_options) {
    _options->stateGames = DEF_OPTIONS_STATE_GAMES;
    _options->curtains = DEF_OPTIONS_CURTAINS;
    _options->logHeader = DEF_OPTIONS_LOG_HEADER;
}

enum EnumFlagType getFlagType(const char* flag) {
    if (flag[0] != '-') {
        return FLAG_TYPE_NOT_FLAG;
    }

    /*** * * ***/

    if (
        strcmp(flag, "--c") == 0
        ||
        strcmp(flag, "--curtains") == 0
    ) {
        return FLAG_TYPE_CURTAINS;
    }

    if (
        strcmp(flag, "--sg") == 0
        ||
        strcmp(flag, "--state-games") == 0
    ) {
        return FLAG_TYPE_STATE_GAMES;
    }

    if (strcmp(flag, "--log-no-header") == 0) {
        return FLAG_TYPE_LOG_NO_HEADER;
    }

    /*** * * ***/

    return FLAG_TYPE_UNKNOWN;
}

void readFlags(int argc, char* argv[], Options *_options) {
    for (int i = 1; i < argc; i++) { // start from 1 to skip program name
        enum EnumFlagType flagType;

        flagType = getFlagType(argv[i]);

        switch (flagType) {
            case FLAG_TYPE_NOT_FLAG:
                break;

            case FLAG_TYPE_UNKNOWN:
                fprintf(stderr, "Error: Unknown flag %s\n", argv[i]);

                /*** * * ***/

                break;

            case FLAG_TYPE_CURTAINS: {
                int arg;

                /*** * * ***/

                if (i+1 >= argc) {
                    fprintf(stderr, "Error: Missing argument for %s flag\n", argv[i]);

                    /*** * * ***/

                    break;
                }

                /*** * * ***/

                arg = atoi(argv[i + 1]);
                i++;

                /*** * * ***/

                if (arg == 0) {
                    fprintf(stderr, "Error: Invalid argument for %s flag\n", argv[i]);

                    /*** * * ***/

                    break;
                }

                if (arg < 0) {
                    fprintf(stderr, "Error: Invalid argument for %s flag: Negative\n", argv[i]);

                    /*** * * ***/

                    break;
                }

                /*** * * ***/

                _options->curtains = arg;

                /*** * * ***/

                break;
            }

            case FLAG_TYPE_STATE_GAMES: {
                int arg;

                /*** * * ***/

                if (i+1 >= argc) {
                    fprintf(stderr, "Error: Missing argument for %s flag\n", argv[i]);

                    /*** * * ***/

                    break;
                }

                /*** * * ***/

                arg = atoi(argv[i + 1]);
                i++;

                /*** * * ***/

                if (arg == 0) {
                    fprintf(stderr, "Error: Invalid argument for %s flag\n", argv[i]);

                    /*** * * ***/

                    break;
                }

                if (arg < 0) {
                    fprintf(stderr, "Error: Invalid argument for %s flag: Negative\n", argv[i]);

                    /*** * * ***/

                    break;
                }

                /*** * * ***/

                _options->stateGames = arg;

                /*** * * ***/

                break;
            }

            case FLAG_TYPE_LOG_NO_HEADER:
                _options->logHeader = false;

                /*** * * ***/

                break;
        }
    }
}

void test() {
    // options
    Options options = {0};

    // simulation
    Simulation simulation = {0};

    /*** * * ***/

    // readFlags
    readFlags(
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
    assert(options.curtains == 5);
    // readFlags : assert : stateGames
    assert(options.stateGames == 100);
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
    for (int s=0;s<=CONST_STATES_MAX_IDX;s++) {
        // simulation : run : assert : gamesCount
        assert(simulation.states[s].gamesCount == options.stateGames);
        // simulation : run : assert : playerWinsCount
        assert(simulation.states[s].playerWinsCount > 0);
        assert(simulation.states[s].playerWinsCount < options.stateGames);
    }
}

/*** * * ***/

int main(int argc, char* argv[]) {
    // test
    test();

    /*** * * ***/

    // options
    Options options = {0};

    // simulation
    Simulation simulation = {0};

    /*** * * ***/

    // options
    // options : default
    setOptionsDefault(&options);

    // flags
    readFlags(argc, argv, &options);

    // simulation
    // simulation : populate
    populateSimulation(&simulation);
    // simulation : run
    runSimulation(&simulation, &options);
    // simulation : log
    logSimulation(&simulation, &options);

    /*** * * ***/

    return EXIT_SUCCESS;
}
