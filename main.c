#include <stdio.h>
#include <stdlib.h> // malloc, free
#include <assert.h>
#include <stdbool.h>
#include <time.h>

#include <omp.h>

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

    /*** * * ***/

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

    /*** * * ***/
    
    return FLAG_UNKNOWN;
}

void readFlags(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) { // start from 1 to skip program name
        enum Flag flag = getFlagType(argv[i]);

        switch (flag) {
            case NOT_FLAG:
                break;

            case FLAG_UNKNOWN:
                fprintf(stderr, "Warning: Unknown flag %s\n", argv[i]);

                /*** * * ***/
                
                break;

            case FLAG_CURTAINS: {
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

            case FLAG_STATE_GAMES: {
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

            case FLAG_LOG_NO_HEADER:
                logHeader = false;

                /*** * * ***/
                
                break;
        }
    }
}

void initRand() {
    srand(time(NULL));
}

void populateSimulation(Simulation* _simulation) {
    for (int i = 0; i < STATES; i++) {
        switch (i) {
            case 0:
                _simulation->states[i].doHostReveal = true;
                _simulation->states[i].doPlayerChange = false;

                /*** * * ***/
                
                break;
            
            case 1:
                _simulation->states[i].doHostReveal = true;
                _simulation->states[i].doPlayerChange = true;

                /*** * * ***/
                
                break;
            
            case 2:
                _simulation->states[i].doHostReveal = false;
                _simulation->states[i].doPlayerChange = false;

                /*** * * ***/
                
                break;
            
            case 3:
                _simulation->states[i].doHostReveal = false;
                _simulation->states[i].doPlayerChange = true;

                /*** * * ***/
                
                break;
        }
    }
}

void runSimulation(Simulation* _simulation) {
    #pragma omp parallel for
    for (int g=0;g<stateGames;g++) {
        for (int s=0;s<STATES;s++) {
            Game game;

            /*** * * ***/

            game.winCurtainIdx = rand() % curtains;
            game.playerCurtainIdx = rand() % curtains;

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

                        n = rand() % curtains;

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

                        n = rand() % curtains;

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

            if (game.playerCurtainIdx == game.winCurtainIdx) {
                #pragma omp atomic
                _simulation->states[s].playerWinsCount++;
            }

            #pragma omp atomic
            _simulation->states[s].gamesCount++;
        }
    }
}

char* logSimulation(Simulation* _simulation) {
    size_t bufferSize;

    char *buffer;

    size_t bufferOffset;

    /*** * * ***/

    bufferSize = 1024;
    buffer = (char *)malloc(bufferSize);
    if (!buffer) {
        fprintf(stderr, "Error: logSimulation : buffer : Memory allocation failed\n");

        /*** * * ***/
        
        return NULL;
    }

    /*** * * ***/

    bufferOffset = 0;

    /*** * * ***/

    if (logHeader) {
        bufferOffset += snprintf(
            buffer + bufferOffset,
            bufferSize - bufferOffset,
            "Host Reveal,Player Change,Wins,Curtains,Games\n"
        );
    }

    for (int s = 0; s < STATES; s++) {
        if (bufferOffset >= bufferSize/2) {
            int bufferSizeNew;

            /*** * * ***/

            bufferSizeNew = bufferSize * 2;

            /*** * * ***/

            buffer = (char *)realloc(buffer, bufferSizeNew);
            if (!buffer) {
                fprintf(stderr, "Error: logSimulation : buffer : Memory reallocation failed\n");

                /*** * * ***/
                
                return NULL;
            }

            /*** * * ***/

            bufferSize = bufferSizeNew;
        }

        /*** * * ***/

        bufferOffset += snprintf(
            buffer + bufferOffset,
            bufferSize - bufferOffset,
            "%d,%d,%d,%d,%d\n",
            _simulation->states[s].doHostReveal,
            _simulation->states[s].doPlayerChange,
            _simulation->states[s].playerWinsCount,
            curtains,
            _simulation->states[s].gamesCount
        );
    }

    /*** * * ***/

    return buffer;
}

/*** * * ***/

void test() {
    const int curtainsOriginal = curtains;
    const int stateGamesOriginal = stateGames;
    const bool logHeaderOriginal = logHeader;

    /*** * * ***/
    
    Simulation simulation;

    /*** * * ***/

    simulation.states = (State *)calloc(STATES, sizeof(State));
    assert(simulation.states != NULL);

    /*** * * ***/

    // readFlags
    readFlags(6, (char*[]) { "program", "--c", "5", "--sg", "100", "--log-no-header" });
    assert(curtains == 5); // Check if --c set curtains
    assert(stateGames == 100); // Check if --sg set stateGames
    assert(logHeader == false); // Check if --log-no-header set logHeader

    // initRand
    initRand();

    // simulation
    
    // simulation : populate
    populateSimulation(&simulation);
    assert(simulation.states[0].doHostReveal == true);
    assert(simulation.states[1].doHostReveal == true);
    assert(simulation.states[1].doPlayerChange == true);
    assert(simulation.states[3].doPlayerChange == true);

    // simulation : run
    runSimulation(&simulation);
    for (int s=0;s<STATES;s++) {
        assert(simulation.states[s].gamesCount == stateGames);
    }

    // simulation : log
    assert(strlen(logSimulation(&simulation)) > 0);

    /*** * * ***/

    curtains = curtainsOriginal;
    stateGames = stateGamesOriginal;
    logHeader = logHeaderOriginal;

    /*** * * ***/

    free(simulation.states);
}

/*** * * ***/

int main(int argc, char* argv[]) {
    test();

    /*** * * ***/

    Simulation simulation;

    /*** * * ***/

    simulation.states = (State *)calloc(STATES, sizeof(State));
    if (!simulation.states) {
        fprintf(stderr, "Error: main : simulation.states : Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    /*** * * ***/

    readFlags(argc, argv);

    initRand();

    populateSimulation(&simulation);

    runSimulation(&simulation);

    printf("%s",logSimulation(&simulation));

    /*** * * ***/

    free(simulation.states);

    /*** * * ***/

    return EXIT_SUCCESS;
}
