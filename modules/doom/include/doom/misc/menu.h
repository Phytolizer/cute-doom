#pragma once

#include "doom/misc/defaults.h"

#include <stdint.h>

#define DOOM_MISC_SETUP_GROUPS_X                                                                                       \
    X(null)                                                                                                            \
    X(screen)                                                                                                          \
    X(map)                                                                                                             \
    X(menu)                                                                                                            \
    X(dsda)

typedef enum
{
#define X(x) doom_misc_setup_group_##x,
    DOOM_MISC_SETUP_GROUPS_X
#undef X
} doom_misc_setup_group_t;

typedef struct doom_misc_setup_menu_s {
    const char* text;
    int32_t flags;
    doom_misc_setup_group_t group;
    int16_t x;
    int16_t y;

    union {
        const void* var;
        int32_t* key;
        const char* name;
        doom_misc_default_t* default_entry;
        struct doom_misc_setup_menu_s* menu;
    } var;

    int32_t input;
    void (*action)(void);
    const char** selection_strings;
} doom_misc_setup_menu_t;
