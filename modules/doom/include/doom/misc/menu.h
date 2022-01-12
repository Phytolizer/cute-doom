#pragma once

#include "doom/misc/defaults.h"

#include <phyto/string/string.h>
#include <stdint.h>

#define DOOM_MISC_SETUP_GROUPS_X                                                                                       \
    X(null)                                                                                                            \
    X(screen)                                                                                                          \
    X(map)                                                                                                             \
    X(menu)                                                                                                            \
    X(dsda)

///
/// \brief Setup groups.
///
typedef enum
{
#define X(x) doom_misc_setup_group_##x,
    DOOM_MISC_SETUP_GROUPS_X
#undef X
} doom_misc_setup_group_t;

#define DOOM_MISC_SETUP_MENU_FLAGS_X                                                                                   \
    X(none, 0x0000000)                                                                                                 \
    X(highlighted, 0x0000001)                                                                                          \
    X(selected, 0x0000002)                                                                                             \
    X(title, 0x0000004)                                                                                                \
    X(yes_or_no, 0x0000008)                                                                                            \
    X(use_message_color, 0x0000010)                                                                                    \
    X(use_automap_color, 0x0000020)                                                                                    \
    X(chat_string, 0x0000040)                                                                                          \
    X(reset_to_defaults, 0x0000080)                                                                                    \
    X(has_previous, 0x0000100)                                                                                         \
    X(has_next, 0x0000200)                                                                                             \
    X(input, 0x0000400)                                                                                                \
    X(weapon_number, 0x0000800)                                                                                        \
    X(numeric, 0x0001000)                                                                                              \
    X(skip_me, 0x0002000)                                                                                              \
    X(keep, 0x0004000)                                                                                                 \
    X(last_item, 0x0008000)                                                                                            \
    X(always_warn, 0x0010000)                                                                                          \
    X(warn_on_change, 0x0020000)                                                                                       \
    X(warn_when_bad, 0x0040000)                                                                                        \
    X(filename, 0x0080000)                                                                                             \
    X(left_justify, 0x0100000)                                                                                         \
    X(credit, 0x0200000)                                                                                               \
    X(video_mode_change_error, 0x0400000)                                                                              \
    X(choice, 0x0800000)                                                                                               \
    X(disable, 0x1000000)                                                                                              \
    X(name, 0x2000000)                                                                                                 \
                                                                                                                       \
    X(show_description, doom_misc_setup_menu_flag_title | doom_misc_setup_menu_flag_yes_or_no |                        \
                            doom_misc_setup_menu_flag_use_message_color |                                              \
                            doom_misc_setup_menu_flag_use_automap_color | doom_misc_setup_menu_flag_chat_string |      \
                            doom_misc_setup_menu_flag_reset_to_defaults | doom_misc_setup_menu_flag_has_previous |     \
                            doom_misc_setup_menu_flag_has_next | doom_misc_setup_menu_flag_input |                     \
                            doom_misc_setup_menu_flag_weapon_number | doom_misc_setup_menu_flag_numeric |              \
                            doom_misc_setup_menu_flag_filename | doom_misc_setup_menu_flag_credit |                    \
                            doom_misc_setup_menu_flag_choice | doom_misc_setup_menu_flag_name)                         \
    X(show_set, doom_misc_setup_menu_flag_yes_or_no | doom_misc_setup_menu_flag_use_message_color |                    \
                    doom_misc_setup_menu_flag_use_automap_color | doom_misc_setup_menu_flag_chat_string |              \
                    doom_misc_setup_menu_flag_input | doom_misc_setup_menu_flag_weapon_number |                        \
                    doom_misc_setup_menu_flag_numeric | doom_misc_setup_menu_flag_filename |                           \
                    doom_misc_setup_menu_flag_choice | doom_misc_setup_menu_flag_name)                                 \
    X(string,                                                                                                          \
      doom_misc_setup_menu_flag_chat_string | doom_misc_setup_menu_flag_yes_or_no | doom_misc_setup_menu_flag_name)    \
    X(has_default_ptr, doom_misc_setup_menu_flag_string | doom_misc_setup_menu_flag_yes_or_no |                        \
                           doom_misc_setup_menu_flag_numeric | doom_misc_setup_menu_flag_weapon_number |               \
                           doom_misc_setup_menu_flag_use_message_color | doom_misc_setup_menu_flag_use_automap_color | \
                           doom_misc_setup_menu_flag_choice)

///
/// \brief Setup menu states.
///
typedef enum
{
#define X(x, y) doom_misc_setup_menu_flag_##x = y,
    DOOM_MISC_SETUP_MENU_FLAGS_X
#undef X
} doom_misc_setup_menu_flag_t;

///
/// \brief Setup menu definition.
///
typedef struct doom_misc_setup_menu_s {
    ///
    /// \brief The title of the menu.
    ///
    const char* text;

    ///
    /// \brief The current state.
    ///
    doom_misc_setup_menu_flag_t flags;

    ///
    /// \brief The setup group to which this belongs.
    ///
    doom_misc_setup_group_t group;

    ///
    /// \brief The x position of the menu.
    ///
    int16_t x;

    ///
    /// \brief The y position of the menu.
    ///
    int16_t y;

    ///
    /// \brief The first field of the menu.
    ///
    union {
        ///
        /// \brief A generic variable.
        ///
        const void* var;

        ///
        /// \brief A key value, or NULL if not shown.
        ///
        int32_t* key;

        ///
        /// \brief A name.
        ///
        const char* name;

        ///
        /// \brief An entry in doom_state->defaults.
        ///
        doom_misc_default_t* default_entry;

        ///
        /// \brief The next (or previous) menu.
        ///
        struct doom_misc_setup_menu_s* menu;
    } var;

    ///
    /// \brief An identifier for the input.
    ///
    int32_t input;

    ///
    /// \brief A callback upon changing.
    ///
    void (*action)(void);

    ///
    /// \brief For a choice value, the choices.
    ///
    phyto_string_span_t* selection_strings;
} doom_misc_setup_menu_t;
