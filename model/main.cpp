#include <vector>
#include <ctime>
#include <cstdlib> // for std::rand

class Model {
public :
    class StateResult {
    public:
        int winsCount;
        int lossesCount;

        /*** * * ***/

        StateResult(
            int winsVal = 0,
            int lossesVal = 0
        ):
            winsCount(winsVal),
            lossesCount(lossesVal)
        {

        }
    };

    class State {
    public:
        bool doHostCurtainReveal;
        bool doPlayerCurtainChange;

        StateResult result;

        /*** * * ***/

        State(
            bool doHostCurtainRevealVal,
            bool doPlayerCurtainChangeVal
        ):
            doHostCurtainReveal(doHostCurtainRevealVal),
            doPlayerCurtainChange(doPlayerCurtainChangeVal),

            result(0,0)
        {

        }
    };

    class Session {
    public:
        std::vector<State> States;
    };

    class Curtain {
    public:
        bool isWin;

        bool isPlayerChosen;
        bool isHostChosen;

        /*** * * ***/

        Curtain(
            bool isWinVal = false
        ):
            isWin(isWinVal),

            isPlayerChosen(false),
            isHostChosen(false)
        {

        }
    };

    class Game {
    public:
        std::vector<Curtain> curtains;
        int curtainsCount;

        int winCurtainIndex;

        int hostCurtainIdx;
        int playerCurtainIdx;

        /*** * * ***/

        Game(int curtainsCountVal = 3) {
            curtainsCount = curtainsCountVal;

            /*** * * ***/

            for (int i = 0; i < curtainsCount; i++) {
                curtains.emplace_back();
            }
        }

        /*** * * ***/

        void playerChangeCurtain(int oldCurtainIdx, int newCurtainIdx) {
            curtains[oldCurtainIdx].isPlayerChosen = false;
            curtains[newCurtainIdx].isPlayerChosen = true;
        }

        bool isPlayerWin() {
            return curtains[playerCurtainIdx].isWin;
        }
    };
};
