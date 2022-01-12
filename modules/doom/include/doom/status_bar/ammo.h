#pragma once

#define DOOM_STATUS_BAR_AMMO_COLOR_BEHAVIORS_X                                                                         \
    X(no)                                                                                                              \
    X(full_only)                                                                                                       \
    X(yes)                                                                                                             \
    X(count)

typedef enum
{
#define X(x) doom_status_bar_ammo_color_behavior_##x,
    DOOM_STATUS_BAR_AMMO_COLOR_BEHAVIORS_X
#undef X
} doom_status_bar_ammo_color_behavior_t;
