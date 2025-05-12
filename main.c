#include <stdlib.h> // for EXIT_SUCCESS

// model
// model : options
#include "./model/options.h"

// utils
#include "./util/main.h"

// test
#include "./test/main.h"

// simulation
#include "./model/simulation.h"
#include "./controller/simulation.h"
#include "./view/simulation.h"

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
    utilSetOptionsDefault(&options);

    // flags
    utilReadFlags(argc, argv, &options);

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
