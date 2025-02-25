#include <stdbool.h>

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
    State* states;
} Simulation;
