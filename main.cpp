#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib> // for std::rand
#include <sstream>

#include "model/main.cpp"

/*** * * ***/

const int DEFAULT_STATE_GAMES_COUNT = 1000000;
const int DEFAULT_CURTAINS_COUNT = 3;

/*** * * ***/

Model model;
Model::Session session;

int curtainsCount = DEFAULT_CURTAINS_COUNT;
int stateGamesCount = DEFAULT_STATE_GAMES_COUNT;

/*** * * ***/

void handleFlags(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) {
        if (std::string(argv[i]) == "--c") {
            if (i+1 > argc) {
                return;
            }

            /*** * * ***/

            curtainsCount = std::stoi(argv[i+1]);

            /*** * * ***/

            i++;
        } else if (std::string(argv[i]) == "--g") {
            if (i+1 > argc) {
                return;
            }

            /*** * * ***/

            stateGamesCount = std::stoi(argv[i+1]);

            /*** * * ***/

            i++;
        }
    }
}

void initRand() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void initSession(){
    session.States.emplace_back(true, true);
    session.States.emplace_back(true, false);
    session.States.emplace_back(false, true);
    session.States.emplace_back(false, false);
}

void runSession() {
    for (int i = 0; i < stateGamesCount; ++i) {
        for (Model::State& state : session.States) {
            Model::Game game(curtainsCount);

            /*** * * ***/

            game.playerCurtainIdx = std::rand() % curtainsCount;

            game.curtains[(std::rand() % curtainsCount)].isWin = true;

            game.curtains[game.playerCurtainIdx].isPlayerChosen = true;

            /*** * * ***/

            if (state.doHostCurtainReveal) {
                do {
                    game.hostCurtainIdx = std::rand() % curtainsCount;
                } while (
                    game.curtains[game.hostCurtainIdx].isWin
                    ||
                    game.curtains[game.hostCurtainIdx].isPlayerChosen
                );

                /*** * * ***/

                game.curtains[game.hostCurtainIdx].isHostChosen = true;
            }

            if (state.doPlayerCurtainChange) {
                int oldPlayerCurtainIdx;
                int newPlayerCurtainIdx;

                /*** * * ***/

                oldPlayerCurtainIdx = game.playerCurtainIdx;

                do {
                    newPlayerCurtainIdx = std::rand() % curtainsCount;
                } while (
                    game.curtains[newPlayerCurtainIdx].isPlayerChosen
                    ||
                    game.curtains[newPlayerCurtainIdx].isHostChosen
                );

                /*** * * ***/

                game.playerChangeCurtain(oldPlayerCurtainIdx, newPlayerCurtainIdx);

                /*** * * ***/

                game.playerCurtainIdx = newPlayerCurtainIdx;
            }

            /*** * * ***/

            if (game.isPlayerWin()) {
                state.result.winsCount++;
            } else {
                state.result.lossesCount++;
            }
        }
    }
}

void logResultState(const Model::State& state) {
    std::ostringstream oss;
    oss << "For state "
        << "<"
        << "doHostCurtainReveal=" << state.doHostCurtainReveal
        << ", " << "doPlayerCurtainChange=" << state.doPlayerCurtainChange
        << ">"
        << ": "
        << "Wins=" << state.result.winsCount
        << ", " << "Losses=" << state.result.lossesCount;
        // << ", Loops=" << state.result.iterations;
    std::cout << oss.str() << std::endl;
}

void logResult() {
    for (const Model::State& state : session.States) {
        logResultState(state);
    }

    std::cout << std::endl;
    std::cout << "Curtains: " << curtainsCount << std::endl;
    std::cout << "Games per state: " << stateGamesCount << std::endl;
}

/*** * * ***/

int main(int argc, char* argv[]) {
    handleFlags(argc, argv);

    initRand();
    initSession();

    /*** * * ***/

    runSession();

    /*** * * ***/

    logResult();

    /*** * * ***/

    return 0;
}
