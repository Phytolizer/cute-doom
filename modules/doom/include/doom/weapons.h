#pragma once

#define DOOM_WEAPON_TYPES_X                                                                                            \
    X(fist, 0)                                                                                                         \
    X(pistol, 1)                                                                                                       \
    X(shotgun, 2)                                                                                                      \
    X(chaingun, 3)                                                                                                     \
    X(missile, 4)                                                                                                      \
    X(plasma, 5)                                                                                                       \
    X(bfg, 6)                                                                                                          \
    X(chainsaw, 7)                                                                                                     \
    X(supershotgun, 8)                                                                                                 \
    X(staff, 0)                                                                                                        \
    X(goldwand, 1)                                                                                                     \
    X(crossbow, 2)                                                                                                     \
    X(blaster, 3)                                                                                                      \
    X(skullrod, 4)                                                                                                     \
    X(phoenixrod, 5)                                                                                                   \
    X(mace, 6)                                                                                                         \
    X(gauntlets, 7)                                                                                                    \
    X(beak, 8)                                                                                                         \
    X(count, 9)                                                                                                        \
    X(nochange, 10)                                                                                                    \
    X(first, 0)                                                                                                        \
    X(second, 1)                                                                                                       \
    X(third, 2)                                                                                                        \
    X(fourth, 3)                                                                                                       \
    X(hexen_count, 4)

///
/// \brief The defined weapons, including a marker indicating user has not changed weapon.
///
typedef enum
{
#define X(x, y) doom_weapon_type_##x = y,
    DOOM_WEAPON_TYPES_X
#undef X
} doom_weapon_type_t;
