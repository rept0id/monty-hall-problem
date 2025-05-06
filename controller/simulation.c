#include "simulation.h"

/*** * * ***/

#include <stdio.h>
#include <stdbool.h>

#include <omp.h>

#include <stdlib.h> // for rand_r

#include "../model/constants.h"
#include "../model/options.h"
#include "../model/simulation.h"
#include "../model/enumStateType.h"

/*** * * ***/

void populateSimulation(Simulation* _simulation) {
    for (int i = 0; i <= CONST_STATES_MAX_IDX; i++) {
        switch (i) {
            case STATE_TYPE_NOTHING:
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

void runSimulation(Simulation* _simulation, Options *_options) {
    for (int s=0;s<=CONST_STATES_MAX_IDX;s++) {
        // simulation
        int simulationPlayerWinsCount;
        int simulationGamesCount;

        /*** * * ***/

        // simulation
        simulationPlayerWinsCount = 0;
        simulationGamesCount = 0;

        /*** * * ***/

        #pragma omp parallel for reduction(+:simulationPlayerWinsCount, simulationGamesCount) // parallelizing the inner-most loop
        for (int g=0;g<_options->stateGames;g++) {
            // seeds
            unsigned int seedWin;
            unsigned int seedPlayer;
            unsigned int seedHost;

            // game
            Game game;

            /*** * * ***/

            // seeds
            seedWin = (unsigned int)((s + g) * 7919);
            seedPlayer = (unsigned int)((s + g) * 104729);
            seedHost = (unsigned int)((s + g) * 1299709);

            // game (based on : seeds)
            game.winCurtainIdx = rand_r(&seedWin) % _options->curtains;
            game.playerCurtainIdx = rand_r(&seedPlayer) % _options->curtains;

            /*** * * ***/

            // game
            // game : doHostReveal (based on : seeds)
            if (_simulation->states[s].doHostReveal) {
                // if curtains are 3, there is only one curtain to switch to,
                //  as the other is either the winning or the *PLAYER* choosen
                //  thus, we want a deterministic approach.
                // else, we want a purely random approach (and not one that just picks the next).
                if (_options->curtains == 3) {
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
                    while(true) {
                        int n;

                        /*** * * ***/

                        n = rand_r(&seedHost) % _options->curtains;

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
            // game : doPlayerChange (based on : seeds)
            if (_simulation->states[s].doPlayerChange) {
                // (again, kind of same logic)
                // if curtains are 3, there is only one curtain to switch to,
                //  as the other is either the winning or the *HOST* choosen
                //  thus, we want a deterministic approach.
                // else, we want a purely random approach (and not one that just picks the next).
                if (_options->curtains == 3) {
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
                    while(true) {
                        int n;

                        /*** * * ***/

                        n = rand_r(&seedPlayer) % _options->curtains;

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

            /*** * * ***/

            // state
            // state : sPlayerWinsCount (based on : game)
            if (game.playerCurtainIdx == game.winCurtainIdx) {
                simulationPlayerWinsCount++;
            }
            // state : sGamesCount
            simulationGamesCount++;
        }

        /*** * * ***/

        // _simulation
        _simulation->states[s].playerWinsCount = simulationPlayerWinsCount;
        _simulation->states[s].gamesCount = simulationGamesCount;
    }
}

void logSimulation(Simulation* _simulation, Options *_options) {
    // header
    if (_options->logHeader) {
        printf("Host Reveal,Player Change,Wins,Curtains,Games\n");
    }

    // data
    for (int s = 0; s <= CONST_STATES_MAX_IDX; s++) {
        printf(
            "%d,%d,%d,%d,%d\n",

            _simulation->states[s].doHostReveal,
            _simulation->states[s].doPlayerChange,
            _simulation->states[s].playerWinsCount,
            _options->curtains,
            _simulation->states[s].gamesCount
        );
    }
}
