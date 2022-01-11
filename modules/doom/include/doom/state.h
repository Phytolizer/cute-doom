#pragma once

#include "doom/sys/system.h"

#include <stdint.h>

typedef struct {
    int32_t argc;
    char** argv;
    doom_sys_atexit_list_entry_t* exit_funcs[doom_sys_exit_priority_max];
} doom_state_t;

doom_state_t* doom_state_new(int argc, char** argv);
void doom_state_free(doom_state_t** p_state);
