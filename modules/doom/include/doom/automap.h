#pragma once

#define DOOM_AUTOMAP_MODES_X                                                                                           \
    X(active, 0x01)                                                                                                    \
    X(overlay, 0x02)                                                                                                   \
    X(rotate, 0x04)                                                                                                    \
    X(follow, 0x08)                                                                                                    \
    X(grid, 0x10)

///
/// \brief Automap mode bitflags.
///
typedef enum
{
#define X(x, y) doom_automap_mode_##x = y,
    DOOM_AUTOMAP_MODES_X
#undef X
} doom_automap_mode_t;

#define DOOM_AUTOMAP_THINGS_APPEARANCES_X                                                                              \
    X(classic)                                                                                                         \
    X(scaled)                                                                                                          \
    X(icon)                                                                                                            \
    X(count)

///
/// \brief The way things look on the automap.
///
typedef enum
{
#define X(x) doom_automap_things_appearance_##x,
    DOOM_AUTOMAP_THINGS_APPEARANCES_X
#undef X
} doom_automap_things_appearance_t;
