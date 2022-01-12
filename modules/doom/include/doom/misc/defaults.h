#pragma once

#include "doom/dsda/input.h"
#include "phyto/string/string.h"

#include <phyto/collections/dynamic_array.h>
#include <stdbool.h>
#include <stdint.h>

#define DOOM_MISC_DEFAULT_TYPES_X                                                                                      \
    X(none, 0)                                                                                                         \
    X(string, 1)                                                                                                       \
    X(integer, 2)                                                                                                      \
    X(hex_integer, 3)                                                                                                  \
    X(array, 4)                                                                                                        \
    X(input, 5)                                                                                                        \
    X(boolean, 2)                                                                                                      \
    X(color, 3)

typedef enum
{
#define X(x, y) doom_misc_default_type_##x = y,
    DOOM_MISC_DEFAULT_TYPES_X
#undef X
} doom_misc_default_type_t;

typedef struct {
    const char* name;

    struct {
        int32_t* pi;
        bool* pb;
        phyto_string_span_t* ps;
        struct {
            int32_t* size;
            char*** data;
            int32_t index;
        } array;
    } location;

    struct {
        int32_t i;
        bool b;
        phyto_string_span_t s;
        struct {
            int32_t size;
            phyto_string_span_t* data;
        } array;
    } default_value;

    int32_t min_value;
    int32_t max_value;

    doom_misc_default_type_t type;
    int32_t setup_screen;
    int32_t* current;

    // This must be pre-declared due to a cyclic dependency.
    struct doom_misc_setup_menu_s* setup_menu;

    int32_t identifier;
    doom_dsda_input_default_t input;
} doom_misc_default_t;

PHYTO_COLLECTIONS_DYNAMIC_ARRAY_DECL(doom_misc_default_dyarray, doom_misc_default_t);

doom_misc_default_dyarray_t doom_misc_default_dyarray_new(void);

#define DOOM_MISC_SETUP_SCREENS_X                                                                                      \
    X(none)                                                                                                            \
    X(keys)                                                                                                            \
    X(weapons)                                                                                                         \
    X(status_bar)                                                                                                      \
    X(auto)                                                                                                            \
    X(messages)                                                                                                        \
    X(chat)                                                                                                            \
    X(general)                                                                                                         \
    X(max)

typedef enum
{
#define X(x) doom_misc_setup_screen_##x,
    DOOM_MISC_SETUP_SCREENS_X
#undef X
} doom_misc_setup_screen_t;

///
/// \brief Load default values from the config file.
///
void doom_misc_load_defaults(void);

///
/// \brief Save default values from the config file.
///
void doom_misc_save_defaults(void);
