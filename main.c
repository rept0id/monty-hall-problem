#include <stdio.h>
#include <stdlib.h> // malloc, free
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "./model/main.c"

/*** * * ***/

#define DEF_STATE_GAMES 1000000
#define DEF_CURTAINS 3
#define DEF_STATES 4
#define DEF_LOG_HEADER true

/*** * * ***/

enum Flag {
    FLAG_C,
    FLAG_G,
    FLAG_LOG_NO_HEADER,
    FLAG_UNKNOWN
};

int stateGames = DEF_STATE_GAMES;
int curtains = DEF_CURTAINS;

bool logHeader = DEF_LOG_HEADER;

/*** * * ***/

enum Flag getFlagType(const char* flag) {
    if (strcmp(flag, "--c") == 0) return FLAG_C;
    if (strcmp(flag, "--g") == 0) return FLAG_G;
    if (strcmp(flag, "--log-no-header") == 0) return FLAG_LOG_NO_HEADER;
    return FLAG_UNKNOWN;
}

void handleFlags(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) {
        enum Flag flag = getFlagType(argv[i]);

        switch (flag) {
            case FLAG_C:
                if (i + 1 < argc) {
                    curtains = atoi(argv[i + 1]);
                    i++;
                } else {
                    return; // missing argument for flag
                }
                break;

            case FLAG_G:
                if (i + 1 < argc) {
                    stateGames = atoi(argv[i + 1]);
                    i++;
                } else {
                    return; // missing argument for flag
                }
                break;

            case FLAG_LOG_NO_HEADER:
                logHeader = false;
                break;

            case FLAG_UNKNOWN:
                break;
        }
    }
}

void initRand() {
    srand(time(NULL));
}

void initStatesSimulation(Simulation* _simulation) {
    _simulation->states = (State *)malloc(DEF_STATES * sizeof(State));

    /*** * * ***/

    for (int i = 0; i < DEF_STATES; i++) {
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
        for (int s=0;s<DEF_STATES;s++) {
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

    for (int s=0;s<DEF_STATES;s++) {
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
