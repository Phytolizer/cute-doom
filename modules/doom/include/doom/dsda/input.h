#pragma once

#include <stdint.h>

///
/// \brief A configured input.
///
typedef struct {
    int32_t key;
    int32_t mouse_button;
    int32_t joystick_button;
} doom_dsda_input_default_t;

enum
{
    ///
    /// \brief The number of input profiles.
    ///
    doom_dsda_input_profile_count = 3,
};
