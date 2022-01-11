#pragma once

#include <stdint.h>

typedef struct {
    int32_t argc;
    char** argv;
} doom_state_t;

doom_state_t* doom_state_new(int argc, char** argv);
void doom_state_free(doom_state_t* state);
