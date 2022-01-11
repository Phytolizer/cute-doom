#include "doom/state.h"

#include <nonstd/strdup.h>
#include <stdlib.h>
#include <string.h>

doom_state_t* doom_state_new(int argc, char** argv) {
    doom_state_t* state = calloc(1, sizeof(doom_state_t));
    state->argc = argc;
    state->argv = malloc(sizeof(char*) * argc);
    for (int i = 0; i < argc; i++) {
        state->argv[i] = nonstd_strdup(argv[i]);
    }
    return state;
}

void doom_state_free(doom_state_t* state) {
    for (int32_t i = 0; i < state->argc; i++) {
        free(state->argv[i]);
    }
    free(state->argv);
    free(state);
}
