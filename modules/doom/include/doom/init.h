#pragma once

#include "doom/state.h"

#include <stdint.h>

extern doom_state_t* doom_state;

void doom_init(int argc, char** argv);
void doom_quit(int32_t exit_code) __attribute__((noreturn));
