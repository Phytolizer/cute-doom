#pragma once

#include "doom/misc/argv.h"
#include "doom/misc/defaults.h"
#include "doom/sys/system.h"

#include <phyto/string/string.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    doom_misc_parameters_t params;
    doom_sys_atexit_list_entry_t* exit_funcs[doom_sys_exit_priority_max];
    doom_misc_default_dyarray_t defaults;

    // ========
    // Defaults
    // ========

    doom_misc_default_storage_t defaults_storage;
} doom_state_t;

doom_state_t* doom_state_new(int argc, char** argv);
void doom_state_free(doom_state_t** p_state);
