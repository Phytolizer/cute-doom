#pragma once

#include "doom/misc/argv.h"
#include "doom/misc/defaults.h"
#include "doom/sys/system.h"

#include <phyto/string/string.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

///
/// \brief The entire state of the engine and game.
///
typedef struct {
    ///
    /// \brief The command-line parameters.
    ///
    doom_misc_parameters_t params;

    ///
    /// \brief A linked list of hooks to run at exit.
    ///
    doom_sys_atexit_list_entry_t* exit_funcs[doom_sys_exit_priority_max];

    ///
    /// \brief The default values for many configuration options.
    ///
    doom_misc_default_dyarray_t defaults;

    ///
    /// \brief The runtime configuration options.
    ///
    doom_misc_default_storage_t defaults_storage;
} doom_state_t;

///
/// \brief Initialize the Doom state.
///
/// NOTE: The defaults field is NOT initialized, as it is dependent on the runtime storage location (i.e. the pointer
/// value). It should be initialized with `doom_misc_default_dyarray_new()` after calling this function.
///
doom_state_t* doom_state_new(int argc, char** argv);

///
/// \brief Free the Doom state.
///
void doom_state_free(doom_state_t** p_state);
