#include <stdio.h>
#include <stdlib.h> // malloc, free
#include <assert.h>
#include <stdbool.h>

#include <omp.h>

#include <stdint.h>

#include <string.h>

#include "./model/main.c"
#include "./model/enumFlagType.c"
#include "./model/enumStateType.c"

/*** * * ***/

#define DEF_STATE_GAMES 1000000
#define DEF_CURTAINS 3
#define DEF_LOG_HEADER true

/*** * * ***/

int stateGames = DEF_STATE_GAMES;
int curtains = DEF_CURTAINS;
bool logHeader = DEF_LOG_HEADER;

/*** * * ***/

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

void readFlags(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) { // start from 1 to skip program name
        enum EnumFlagType flag = getFlagType(argv[i]);

        switch (flag) {
            case FLAG_TYPE_NOT_FLAG:
                break;

            case FLAG_TYPE_UNKNOWN:
                fprintf(stderr, "Warning: Unknown flag %s\n", argv[i]);

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

                curtains = arg;

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

                stateGames = arg;

                /*** * * ***/

                break;
            }

            case FLAG_TYPE_LOG_NO_HEADER:
                logHeader = false;

                /*** * * ***/

                break;
        }
    }
}

void populateSimulation(Simulation* _simulation) {
    for (int i = 0; i <= CONST_STATES_MAX_IDX; i++) {
        switch (i) {
            case STATE_TYPE_NORMAL:
                _simulation->states[i].doHostReveal = false;
                _simulation->states[i].doPlayerChange = false;

                /*** * * ***/

                break;

            case STATE_TYPE_DO_HOST_REVEAL:
                _simulation->states[i].doHostReveal = true;
                _simulation->states[i].doPlayerChange = false;

                /*** * * ***/

                break;

            case STATE_TYPE_DO_HOST_REVEAL_DO_PLAYER_SWITCH:
                _simulation->states[i].doHostReveal = true;
                _simulation->states[i].doPlayerChange = true;

                /*** * * ***/

                break;

            case STATE_TYPE_DO_PLAYER_CHANGE:
                _simulation->states[i].doHostReveal = false;
                _simulation->states[i].doPlayerChange = true;

                /*** * * ***/

                break;

            default:
                _simulation->states[i].doHostReveal = false;
                _simulation->states[i].doPlayerChange = false;

                /*** * * ***/

                break;
        }
    }
}

void runSimulation(Simulation* _simulation) {
    for (int s=0;s<=CONST_STATES_MAX_IDX;s++) {
        int sPlayerWinsCount;
        int sGamesCount;

        /*** * * ***/

        sPlayerWinsCount = 0;
        sGamesCount = 0;

        /*** * * ***/

        #pragma omp parallel for reduction(+:sPlayerWinsCount, sGamesCount) // parallelizing the inner-most loop
        for (int g=0;g<stateGames;g++) {
            Game game;

            unsigned int gWinCurtainIdxSeed;
            unsigned int gPlayerCurtainIdxSeed;
            unsigned int gHostCurtainIdxSeed;

            /*** * * ***/

            gWinCurtainIdxSeed = (unsigned int)((s + g) * 7919);
            gPlayerCurtainIdxSeed = (unsigned int)((s + g) * 104729);
            gHostCurtainIdxSeed = (unsigned int)((s + g) * 1299709);

            game.winCurtainIdx = rand_r(&gWinCurtainIdxSeed) % curtains;
            game.playerCurtainIdx = rand_r(&gPlayerCurtainIdxSeed) % curtains;

            /*** * * ***/

            if (_simulation->states[s].doHostReveal) {
                // if curtains are 3, there is only one curtain to switch to,
                //  as the other is either the winning or the *PLAYER* choosen
                //  thus, we want a deterministic approach.
                // else, we want a purely random approach (and not one that just picks the next).
                if (curtains == 3) {
                    for (int i=0;i<3;i++) {
                        if (i == game.winCurtainIdx) {
                            continue;
                        }

                        if (i == game.playerCurtainIdx) {
                            continue;
                        }

                        /*** * * ***/

                        game.hostCurtainIdx = i;

                        /*** * * ***/

                        break;
                    }
                } else {
                    while(1) {
                        int n;

                        /*** * * ***/

                        n = rand_r(&gHostCurtainIdxSeed) % curtains;

                        /*** * * ***/

                        if (n == game.winCurtainIdx) {
                            continue;
                        }

                        if (n == game.playerCurtainIdx) {
                            continue;
                        }

                        /*** * * ***/

                        game.hostCurtainIdx = n;

                        /*** * * ***/

                        break;
                    }
                }
            }

            if (_simulation->states[s].doPlayerChange) {
                // (again, kind of same logic)
                // if curtains are 3, there is only one curtain to switch to,
                //  as the other is either the winning or the *HOST* choosen
                //  thus, we want a deterministic approach.
                // else, we want a purely random approach (and not one that just picks the next).
                if (curtains == 3) {
                    for (int i=0;i<3;i++) {
                        if (i == game.playerCurtainIdx) {
                            continue;
                        }

                        if (_simulation->states[s].doHostReveal) {
                            if (i == game.hostCurtainIdx) {
                                continue;
                            }
                        }

                        /*** * * ***/

                        game.playerCurtainIdx = i;

                        /*** * * ***/

                        break;
                    }
                } else {
                    while(1) {
                        int n;

                        /*** * * ***/

                        n = rand_r(&gPlayerCurtainIdxSeed) % curtains;

                        /*** * * ***/

                        if (n == game.playerCurtainIdx) {
                            continue;
                        }

                        if (_simulation->states[s].doHostReveal) {
                            if (n == game.hostCurtainIdx) {
                                continue;
                            }
                        }

                        /*** * * ***/

                        game.playerCurtainIdx = n;

                        /*** * * ***/

                        break;
                    }
                }
            }

            if (game.playerCurtainIdx == game.winCurtainIdx) {
                sPlayerWinsCount++;
            }

            /*** * * ***/

            sGamesCount++;
        }

        _simulation->states[s].playerWinsCount = sPlayerWinsCount;
        _simulation->states[s].gamesCount = sGamesCount;
    }
}

void logSimulation(Simulation* _simulation) {
    if (logHeader) {
        printf("Host Reveal,Player Change,Wins,Curtains,Games\n");
    }

    for (int s = 0; s <= CONST_STATES_MAX_IDX; s++) {
        printf(
            "%d,%d,%d,%d,%d\n",

            _simulation->states[s].doHostReveal,
            _simulation->states[s].doPlayerChange,
            _simulation->states[s].playerWinsCount,
            curtains,
            _simulation->states[s].gamesCount
        );
    }
}

/*** * * ***/

void test() {
    const int curtainsOriginal = curtains;
    const int stateGamesOriginal = stateGames;
    const bool logHeaderOriginal = logHeader;

    /*** * * ***/

    Simulation simulation;

    /*** * * ***/

    // readFlags

    readFlags(6, (char*[]) { "program", "--c", "5", "--sg", "100", "--log-no-header" });

    assert(curtains == 5); // Check if --c set curtains
    assert(stateGames == 100); // Check if --sg set stateGames
    assert(logHeader == false); // Check if --log-no-header set logHeader

    // simulation

    // simulation : populate

    populateSimulation(&simulation);

    assert(simulation.states[STATE_TYPE_NORMAL].doHostReveal == false);
    assert(simulation.states[STATE_TYPE_NORMAL].doPlayerChange == false);

    assert(simulation.states[STATE_TYPE_DO_HOST_REVEAL].doHostReveal == true);
    assert(simulation.states[STATE_TYPE_DO_HOST_REVEAL].doPlayerChange == false);

    assert(simulation.states[STATE_TYPE_DO_HOST_REVEAL_DO_PLAYER_SWITCH].doHostReveal == true);
    assert(simulation.states[STATE_TYPE_DO_HOST_REVEAL_DO_PLAYER_SWITCH].doPlayerChange == true);

    assert(simulation.states[STATE_TYPE_DO_PLAYER_CHANGE].doHostReveal == false);
    assert(simulation.states[STATE_TYPE_DO_PLAYER_CHANGE].doPlayerChange == true);

    // simulation : run

    runSimulation(&simulation);

    for (int s=0;s<=CONST_STATES_MAX_IDX;s++) {
        assert(simulation.states[s].gamesCount == stateGames);

        assert(simulation.states[s].playerWinsCount > 0);
        assert(simulation.states[s].playerWinsCount < stateGames);
    }

    /*** * * ***/

    curtains = curtainsOriginal;
    stateGames = stateGamesOriginal;
    logHeader = logHeaderOriginal;
}

/*** * * ***/

int main(int argc, char* argv[]) {
    test();

    /*** * * ***/

    Simulation simulation = {0};

    /*** * * ***/

    readFlags(argc, argv);

    populateSimulation(&simulation);

    runSimulation(&simulation);

    logSimulation(&simulation);

    /*** * * ***/

    return EXIT_SUCCESS;
}
