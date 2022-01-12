#pragma once

#define DOOM_COMPATIBILITY_LEVELS_X                                                                                    \
    X(doom_12)                                                                                                         \
    X(doom_1666)                                                                                                       \
    X(doom2_19)                                                                                                        \
    X(ultimate_doom)                                                                                                   \
    X(final_doom)                                                                                                      \
    X(dos_doom)                                                                                                        \
    X(tas_doom)                                                                                                        \
    X(boom_compatibility_mode)                                                                                         \
    X(boom_201)                                                                                                        \
    X(boom_202)                                                                                                        \
    X(lxdoom_1)                                                                                                        \
    X(mbf)                                                                                                             \
    X(prboom_1)                                                                                                        \
    X(prboom_2)                                                                                                        \
    X(prboom_3)                                                                                                        \
    X(prboom_4)                                                                                                        \
    X(prboom_5)                                                                                                        \
    X(prboom_6)                                                                                                        \
    X(placeholder_18)                                                                                                  \
    X(placeholder_19)                                                                                                  \
    X(placeholder_20)                                                                                                  \
    X(mbf21)                                                                                                           \
    X(max)

typedef enum
{
#define X(x) doom_compatibility_level_##x,
    DOOM_COMPATIBILITY_LEVELS_X
#undef X
        doom_compatibility_level_boom = doom_compatibility_level_boom_201,
    doom_compatibility_level_best = doom_compatibility_level_mbf21,
} doom_compatibility_level_t;
