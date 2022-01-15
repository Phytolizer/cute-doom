#include "doom/state.h"

#include "doom/misc/argv.h"
#include "doom/misc/defaults.h"
#include "doom/sys/system.h"
#include "phyto/string/string.h"

#include <nonstd/strdup.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

doom_state_t* doom_state_new(int argc, char** argv) {
    doom_state_t* state = calloc(1, sizeof(doom_state_t));
    state->params = doom_misc_parameters_new();
    doom_misc_parameters_resize(&state->params, argc);
    for (int i = 0; i < argc; i++) {
        doom_misc_parameters_append(&state->params, phyto_string_from_c(argv[i]));
    }
    return state;
}

void doom_state_free(doom_state_t** p_state) {
    doom_state_t* state = *p_state;
    if (state == NULL) {
        // avoid double free
        return;
    }
    doom_misc_parameters_free(&state->params);
    for (doom_sys_exit_priority_t ep = doom_sys_exit_priority_first; ep < doom_sys_exit_priority_max; ep++) {
        doom_sys_atexit_list_entry_t* entry = state->exit_funcs[ep];
        while (entry != NULL) {
            doom_sys_atexit_list_entry_t* next = entry->next;
            free(entry);
            entry = next;
        }
        state->exit_funcs[ep] = NULL;
    }
    doom_misc_default_dyarray_free(&state->defaults);
    free(state);
    *p_state = NULL;
}
