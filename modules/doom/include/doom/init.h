#pragma once

#include "doom/state.h"

#include <stdint.h>
#include <stdnoreturn.h>

///
/// \brief The state of Doom.
///
/// Contains most of the game state. This structure should be allocated on the heap with doom_init(), and freed with
/// doom_quit().
///
/// \see doom_init
/// \see doom_quit
///
extern doom_state_t* doom_state;

///
/// \brief Initialize the global Doom state.
///
/// This is meant to be called directly by main().
///
void doom_init(int argc, char** argv);

///
/// \brief Free the global Doom state and exit normally.
///
/// This is meant to be called directly by main().
///
noreturn void doom_quit(int32_t exit_code);
