#include <stdio.h>
#include <stdlib.h> // malloc, free
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "./model/main.c"

/*** * * ***/

#define STATES 4

#define DEF_STATE_GAMES 1000000
#define DEF_CURTAINS 3
#define DEF_LOG_HEADER true

/*** * * ***/

enum Flag {
    NOT_FLAG,
    FLAG_UNKNOWN,
    FLAG_CURTAINS,
    FLAG_STATE_GAMES,
    FLAG_LOG_NO_HEADER
};

int stateGames = DEF_STATE_GAMES;
int curtains = DEF_CURTAINS;
bool logHeader = DEF_LOG_HEADER;

/*** * * ***/

enum Flag getFlagType(const char* flag) {
    if (flag[0] != '-') {
        return NOT_FLAG;
    }

    if (
        strcmp(flag, "--c") == 0
        ||
        strcmp(flag, "--curtains") == 0
    ) {
        return FLAG_CURTAINS;
    }

    if (
        strcmp(flag, "--sg") == 0
        ||
        strcmp(flag, "--state-games") == 0
    ) {
        return FLAG_STATE_GAMES;
    }

    if (strcmp(flag, "--log-no-header") == 0) {
        return FLAG_LOG_NO_HEADER;
    }

    return FLAG_UNKNOWN;
}

void handleFlags(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) { // start from 1 to skip program name
        enum Flag flag = getFlagType(argv[i]);

        switch (flag) {
            case NOT_FLAG:
                break;

            case FLAG_UNKNOWN:
                fprintf(stderr, "Warning: Unknown flag %s\n", argv[i]);
                break;

            case FLAG_CURTAINS: {
                int arg;

                /*** * * ***/

                if (i+1 >= argc) {
                    fprintf(stderr, "Error: Missing argument for %s flag\n", argv[i]);

                    break;
                }

                /*** * * ***/

                arg = atoi(argv[i + 1]);
                i++;

                /*** * * ***/

                if (arg == 0) {
                    fprintf(stderr, "Error: Invalid argument for %s flag\n", argv[i]);

                    break;
                }

                if (arg < 0) {
                    fprintf(stderr, "Error: Invalid argument for %s flag: Negative\n", argv[i]);

                    break;
                }

                /*** * * ***/

                curtains = arg;

                /*** * * ***/

                break;
            }

            case FLAG_STATE_GAMES: {
                int arg;

                /*** * * ***/

                if (i+1 >= argc) {
                    fprintf(stderr, "Error: Missing argument for %s flag\n", argv[i]);

                    break;
                }

                /*** * * ***/

                arg = atoi(argv[i + 1]);
                i++;

                /*** * * ***/
                if (arg == 0) {
                    fprintf(stderr, "Error: Invalid argument for %s flag\n", argv[i]);

                    break;
                }

                if (arg < 0) {
                    fprintf(stderr, "Error: Invalid argument for %s flag: Negative\n", argv[i]);

                    break;
                }

                /*** * * ***/

                stateGames = arg;

                /*** * * ***/

                break;
            }

            case FLAG_LOG_NO_HEADER:
                logHeader = false;
                break;
        }
    }
}

void initRand() {
    srand(time(NULL));
}

void initStatesSimulation(Simulation* _simulation) {
    _simulation->states = (State *)malloc(STATES * sizeof(State));

    /*** * * ***/

    for (int i = 0; i < STATES; i++) {
        switch (i) {
            case 0:
                _simulation->states[i].doPlayerChange = true;
                _simulation->states[i].doHostReveal = true;
                break;
            case 1:
                _simulation->states[i].doPlayerChange = true;
                _simulation->states[i].doHostReveal = false;
                break;
            case 2:
                _simulation->states[i].doPlayerChange = false;
                _simulation->states[i].doHostReveal = true;
                break;
            case 3:
                _simulation->states[i].doPlayerChange = true;
                _simulation->states[i].doHostReveal = false;
                break;
        }
    }
}

void simulate(Simulation* _simulation) {
    for (int g=0;g<stateGames;g++) {
        for (int s=0;s<STATES;s++) {
            Game game;

            /*** * * ***/

            game.winCurtainIdx = rand() % curtains;
            game.playerCurtainIdx = rand() % curtains;

            if (_simulation->states[s].doHostReveal) {
                do {
                    game.hostCurtainIdx = rand() % curtains;
                } while(game.hostCurtainIdx == game.winCurtainIdx);
            }

            if (_simulation->states[s].doPlayerChange) {
                int newPlayerCurtainIdx;

                do {
                    newPlayerCurtainIdx = rand() % curtains;
                } while(
                    newPlayerCurtainIdx == game.playerCurtainIdx
                    &&
                    newPlayerCurtainIdx == game.hostCurtainIdx
                );

                game.playerCurtainIdx = newPlayerCurtainIdx;

            }

            /*** * * ***/

            if (game.playerCurtainIdx == game.winCurtainIdx) {
                _simulation->states[s].playerWinsCount++;
            }
        }
    }
}

void logSimulation(Simulation* _simulation) {
    if (logHeader) {
        printf("Host Reveal,Player Change,Wins,Curtains,Games\n");
    }

    /*** * * ***/

    for (int s=0;s<STATES;s++) {
        printf(
            "%d,%d,%d,%d,%d\n",

            _simulation->states[s].doHostReveal,
            _simulation->states[s].doPlayerChange,
            _simulation->states[s].playerWinsCount,
            curtains,
            stateGames
        );
    }
}

/*** * * ***/

int main(int argc, char* argv[]) {
    Simulation simulation;

    /*** * * ***/

    handleFlags(argc, argv);

    /*** * * ***/

    initRand();
    initStatesSimulation(&simulation);

    /*** * * ***/

    simulate(&simulation);

    /*** * * ***/

    logSimulation(&simulation);

    /*** * * ***/

    free(simulation.states);
}
