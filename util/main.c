#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>
#include <string.h>

#include "../model/constantsDefaults.h"
#include "../model/options.h"
#include "../model/enumFlagType.h"

/*** * * ***/

void utilSetOptionsDefault(Options *_options) {
    _options->stateGames = CONST_DEF_OPTIONS_STATE_GAMES;
    _options->curtains = CONST_DEF_OPTIONS_CURTAINS;
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
    for (int i = 1; i < argc; i++) { // start from 1 to skip program name
        enum EnumFlagType flagType;

        flagType = utilGetFlagType(argv[i]);

        switch (flagType) {
            case FLAG_TYPE_NOT_FLAG:
                break;

            case FLAG_TYPE_UNKNOWN:
                fprintf(stderr, "Error: Unknown flag %s\n", argv[i]);

                /*** * * ***/

                break;

            case FLAG_TYPE_CURTAINS: {
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

                _options->curtains = arg;

                /*** * * ***/

                break;
            }

            case FLAG_TYPE_STATE_GAMES: {
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

                _options->stateGames = arg;

                /*** * * ***/

                break;
            }

            case FLAG_TYPE_LOG_NO_HEADER:
                _options->logHeader = false;

                /*** * * ***/

                break;
        }
    }
}
