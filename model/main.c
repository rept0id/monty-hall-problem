#include <stdbool.h>

#include "./constants.c"

typedef struct {
    int winCurtainIdx;

    int playerCurtainIdx;
    int hostCurtainIdx;
} Game;

typedef struct {
    bool doPlayerChange;
    bool doHostReveal;

    int playerWinsCount;

    int gamesCount;
} State;

typedef struct {
    State states[CONST_STATES_MAX_IDX+1];
} Simulation;
