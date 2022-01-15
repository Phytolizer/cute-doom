#pragma once

#include <stdint.h>

#define DOOM_DSDA_INPUT_IDENTIFIERS_X                                                                                  \
    X(null)                                                                                                            \
    X(forward)                                                                                                         \
    X(backward)                                                                                                        \
    X(turn_left)                                                                                                       \
    X(turn_right)                                                                                                      \
    X(speed)                                                                                                           \
    X(strafe_left)                                                                                                     \
    X(strafe_right)                                                                                                    \
    X(strafe)                                                                                                          \
    X(autorun)                                                                                                         \
    X(reverse)                                                                                                         \
    X(use)                                                                                                             \
    X(fly_up)                                                                                                          \
    X(fly_down)                                                                                                        \
    X(fly_center)                                                                                                      \
    X(mouselook)                                                                                                       \
    X(novert)                                                                                                          \
    X(weapon1)                                                                                                         \
    X(weapon2)                                                                                                         \
    X(weapon3)                                                                                                         \
    X(weapon4)                                                                                                         \
    X(weapon5)                                                                                                         \
    X(weapon6)                                                                                                         \
    X(weapon7)                                                                                                         \
    X(weapon8)                                                                                                         \
    X(weapon9)                                                                                                         \
    X(next_weapon)                                                                                                     \
    X(prev_weapon)                                                                                                     \
    X(toggle_weapon)                                                                                                   \
    X(fire)                                                                                                            \
    X(look_up)                                                                                                         \
    X(look_down)                                                                                                       \
    X(look_center)                                                                                                     \
    X(use_artifact)                                                                                                    \
    X(arti_tome)                                                                                                       \
    X(arti_quartz)                                                                                                     \
    X(arti_urn)                                                                                                        \
    X(arti_bomb)                                                                                                       \
    X(arti_ring)                                                                                                       \
    X(arti_chaos_device)                                                                                               \
    X(arti_shadow_sphere)                                                                                              \
    X(arti_wings)                                                                                                      \
    X(arti_torch)                                                                                                      \
    X(arti_morph)                                                                                                      \
    X(inventory_left)                                                                                                  \
    X(inventory_right)                                                                                                 \
    X(spy)                                                                                                             \
    X(join_demo)                                                                                                       \
    X(pause)                                                                                                           \
    X(setup)                                                                                                           \
    X(map)                                                                                                             \
    X(sound_volume)                                                                                                    \
    X(hud)                                                                                                             \
    X(messages)                                                                                                        \
    X(gamma)                                                                                                           \
    X(zoom_in)                                                                                                         \
    X(zoom_out)                                                                                                        \
    X(screenshot)                                                                                                      \
    X(save_game)                                                                                                       \
    X(load_game)                                                                                                       \
    X(quick_save)                                                                                                      \
    X(quick_load)                                                                                                      \
    X(end_game)                                                                                                        \
    X(quit)                                                                                                            \
    X(store_quick_key_frame)                                                                                           \
    X(restore_quick_key_frame)                                                                                         \
    X(rewind)                                                                                                          \
    X(map_follow)                                                                                                      \
    X(map_zoom_in)                                                                                                     \
    X(map_zoom_out)                                                                                                    \
    X(map_up)                                                                                                          \
    X(map_down)                                                                                                        \
    X(map_left)                                                                                                        \
    X(map_right)                                                                                                       \
    X(map_mark)                                                                                                        \
    X(map_clear)                                                                                                       \
    X(map_go_big)                                                                                                      \
    X(map_grid)                                                                                                        \
    X(map_rotate)                                                                                                      \
    X(map_overlay)                                                                                                     \
    X(map_textured)                                                                                                    \
    X(chat)                                                                                                            \
    X(chat_dest0)                                                                                                      \
    X(chat_dest1)                                                                                                      \
    X(chat_dest2)                                                                                                      \
    X(chat_dest3)                                                                                                      \
    X(chat_backspace)                                                                                                  \
    X(chat_enter)                                                                                                      \
    X(speed_up)                                                                                                        \
    X(speed_down)                                                                                                      \
    X(speed_default)                                                                                                   \
    X(demo_skip)                                                                                                       \
    X(demo_end_level)                                                                                                  \
    X(walk_camera)                                                                                                     \
    X(restart)                                                                                                         \
    X(next_level)                                                                                                      \
    X(show_alive)                                                                                                      \
    X(menu_down)                                                                                                       \
    X(menu_up)                                                                                                         \
    X(menu_left)                                                                                                       \
    X(menu_right)                                                                                                      \
    X(menu_backspace)                                                                                                  \
    X(menu_enter)                                                                                                      \
    X(menu_escape)                                                                                                     \
    X(menu_clear)                                                                                                      \
    X(help)                                                                                                            \
    X(escape)                                                                                                          \
    X(cycle_profile)                                                                                                   \
    X(iddqd)                                                                                                           \
    X(idkfa)                                                                                                           \
    X(idfa)                                                                                                            \
    X(idclip)                                                                                                          \
    X(idbeholdh)                                                                                                       \
    X(idbeholdm)                                                                                                       \
    X(idbeholdv)                                                                                                       \
    X(idbeholds)                                                                                                       \
    X(idbeholdi)                                                                                                       \
    X(idbeholdr)                                                                                                       \
    X(idbeholda)                                                                                                       \
    X(idbeholdl)                                                                                                       \
    X(idmypos)                                                                                                         \
    X(idrate)                                                                                                          \
    X(iddt)                                                                                                            \
    X(cycle_palette)                                                                                                   \
    X(command_display)                                                                                                 \
    X(strict_mode)                                                                                                     \
    X(ponce)                                                                                                           \
    X(shazam)                                                                                                          \
    X(chicken)                                                                                                         \
    X(console)                                                                                                         \
    X(coordinate_display)                                                                                              \
    X(jump)                                                                                                            \
    X(hexen_arti_incant)                                                                                               \
    X(hexen_arti_summon)                                                                                               \
    X(hexen_arti_disk)                                                                                                 \
    X(hexen_arti_flechette)                                                                                            \
    X(hexen_arti_banishment)                                                                                           \
    X(hexen_arti_boots)                                                                                                \
    X(hexen_arti_krater)                                                                                               \
    X(hexen_arti_bracers)                                                                                              \
    X(avj)                                                                                                             \
    X(exhud)                                                                                                           \
    X(mute_sfx)                                                                                                        \
    X(mute_music)                                                                                                      \
    X(cheat_codes)                                                                                                     \
    X(notarget)                                                                                                        \
    X(count)

///
/// \brief The list of all possible inputs.
///
typedef enum
{
#define X(x) doom_dsda_input_##x,
    DOOM_DSDA_INPUT_IDENTIFIERS_X
#undef X
} doom_dsda_input_identifier_t;

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
