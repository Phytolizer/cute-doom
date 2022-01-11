#include "doom/misc/argv.h"

#include "doom/init.h"

#include <nonstd/stricmp.h>

int32_t doom_misc_check_parameter(const char* parameter) {
    return doom_misc_check_parameter_ex(parameter, doom_state->argv, doom_state->argc);
}

int32_t doom_misc_check_parameter_ex(const char* parameter, char** params, int32_t params_len) {
    for (int32_t i = 0; i < params_len; i++) {
        if (nonstd_stricmp(params[i], parameter) == 0) {
            return i;
        }
    }
    return -1;
}
