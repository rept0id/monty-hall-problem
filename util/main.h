#ifndef UTIL_MAIN_H
#define UTIL_MAIN_H

#include "../model/options.h"
#include "../model/enumFlagType.h"

void utilSetOptionsDefault(Options* _options);

enum EnumFlagType utilGetFlagType(const char* flag);

void utilReadFlags(int argc, char* argv[], Options* _options);

#endif
