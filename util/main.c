#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>
#include <string.h>

#include "../lib/ratatoi/ratatoi.h"

#include "../model/constants.h"
#include "../model/constantsDefaults.h"
#include "../model/options.h"
#include "../model/enumFlagType.h"

/*** * * ***/

void utilSetOptionsDefault(Options *_options) {
    _options->stateGamesSize = CONST_DEF_OPTIONS_STATE_GAMES_SIZE;
    _options->curtainsSize = CONST_DEF_OPTIONS_CURTAINS_SIZE;
    _options->logHeader = CONST_DEF_OPTIONS_LOG_HEADER;
}

enum EnumFlagType utilGetFlagType(const char* flag) {
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

void utilReadFlags(int argc, char* argv[], Options *_options) {
    for (int pos = 1; pos < argc; pos++) { // start from 1 to skip program name
        enum EnumFlagType flagType;

        /*** * * ***/

        flagType = utilGetFlagType(argv[pos]);

        /*** * * ***/

        switch (flagType) {
            case FLAG_TYPE_NOT_FLAG: {
                break;
            }

            case FLAG_TYPE_UNKNOWN: {
                fprintf(stderr, "Error: Unknown flag %s\n", argv[pos]);

                /*** * * ***/

                exit(EXIT_FAILURE);

                /*** * * ***/

                break;
            }

            case FLAG_TYPE_CURTAINS: {
                int newCurtainsSize;

                /*** * * ***/

                if (pos+1 >= argc) {
                    fprintf(stderr, "Error: Missing argument for %s flag\n", argv[pos]);

                    /*** * * ***/

                    exit(EXIT_FAILURE);

                    /*** * * ***/

                    break;
                }

                /*** * * ***/

                // newCurtainsSize
                newCurtainsSize = ratatoi(argv[pos + 1]);

                /*** * * ***/

                // newCurtainsSize : checks

                // < 0 (error, exit)
                if (newCurtainsSize < 0) {
                    fprintf(stderr, "Error: Invalid argument for %s flag: Negative.\n", argv[pos]);

                    /*** * * ***/

                    exit(EXIT_FAILURE);

                    /*** * * ***/

                    break;
                }

                // == 0 (error, exit)
                if (newCurtainsSize == 0) {
                    fprintf(stderr, "Error: Invalid argument for %s flag: Zero.\n", argv[pos]);

                    /*** * * ***/

                    exit(EXIT_FAILURE);

                    /*** * * ***/

                    break;
                }

                // < CONST_OPTIONS_CURTAINS_SIZE_MIN (error, exit)
                if (newCurtainsSize < CONST_OPTIONS_CURTAINS_SIZE_MIN) {
                    fprintf(stderr, "Error: Invalid argument for %s flag: Less than 3 (the player won’t have any curtain to change to).\n", argv[pos]);

                    /*** * * ***/

                    exit(EXIT_FAILURE);

                    /*** * * ***/

                    break;
                }

                /*** * * ***/

                _options->curtainsSize = newCurtainsSize;

                /*** * * ***/

                pos++;

                /*** * * ***/

                break;
            }

            case FLAG_TYPE_STATE_GAMES: {
                int newStateGamesSize;

                /*** * * ***/

                if (pos+1 >= argc) {
                    fprintf(stderr, "Error: Missing argument for %s flag.\n", argv[pos]);

                    /*** * * ***/

                    exit(EXIT_FAILURE);

                    /*** * * ***/

                    break;
                }

                /*** * * ***/

                newStateGamesSize = ratatoi(argv[pos + 1]);

                /*** * * ***/

                // < 0 (error, exit)
                if (newStateGamesSize < 0) {
                    fprintf(stderr, "Error: Invalid argument for %s flag: Negative.\n", argv[pos]);

                    /*** * * ***/

                    exit(EXIT_FAILURE);

                    /*** * * ***/

                    break;
                }

                // == 0 (error, exit)
                if (newStateGamesSize == 0) {
                    fprintf(stderr, "Error: Invalid argument for %s flag : Zero.\n", argv[pos]);

                    /*** * * ***/

                    exit(EXIT_FAILURE);

                    /*** * * ***/

                    break;
                }

                /*** * * ***/

                _options->stateGamesSize = newStateGamesSize;

                /*** * * ***/

                pos++;

                /*** * * ***/

                break;
            }

            case FLAG_TYPE_LOG_NO_HEADER: {
                _options->logHeader = false;

                /*** * * ***/

                break;
            }
        }
    }
}
