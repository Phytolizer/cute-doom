#include "doom/misc/defaults.h"

#include "doom/init.h"
#include "doom/render/demo.h"
#include "doom/state.h"
#include "phyto/string/string.h"

#include <config.h>

PHYTO_COLLECTIONS_DYNAMIC_ARRAY_IMPL(doom_misc_default_dyarray, doom_misc_default_t);

static const doom_misc_default_dyarray_callbacks_t sc_default_dyarray_callbacks = {
    .free_cb = NULL,
    .compare_cb = NULL,
    .copy_cb = NULL,
    .print_cb = NULL,
};

// These values specify that the min/max are unset.
enum
{
    min_unset = -0xDEAD,
    max_unset = -0xDEAD,
};

static doom_misc_default_t s_header_default(const char* name);

doom_misc_default_dyarray_t doom_misc_default_dyarray_new(void) {
    doom_misc_default_dyarray_t defaults = doom_misc_default_dyarray_init(&sc_default_dyarray_callbacks);

    doom_misc_default_dyarray_append(&defaults, s_header_default("System settings"));
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "process_priority",
                                                    .location = {.pi = &doom_state->defaults_storage.process_priority},
                                                    .default_value = {.i = 0},
                                                    .min_value = 0,
                                                    .max_value = 2,
                                                    .type = doom_misc_default_type_integer,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });

    doom_misc_default_dyarray_append(&defaults, s_header_default("Misc settings"));
    doom_misc_default_dyarray_append(
        &defaults, (doom_misc_default_t){
                       .name = "default_compatibility_level",
                       .location = {.pi = (int*)&doom_state->defaults_storage.default_compatibility_level},
                       .default_value = {.i = -1},
                       .min_value = -1,
                       .max_value = doom_compatibility_level_max - 1,
                       .type = doom_misc_default_type_integer,
                       .setup_screen = doom_misc_setup_screen_none,
                   });
    doom_misc_default_dyarray_append(&defaults,
                                     (doom_misc_default_t){
                                         .name = "realtic_clock_rate",
                                         .location = {.pi = &doom_state->defaults_storage.realtic_clock_rate},
                                         .default_value = {.i = 100},
                                         .min_value = 0,
                                         .max_value = max_unset,
                                         .type = doom_misc_default_type_integer,
                                         .setup_screen = doom_misc_setup_screen_none,
                                     });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "menu_background",
                                                    .location = {.pb = &doom_state->defaults_storage.menu_background},
                                                    .default_value = {.b = true},
                                                    .min_value = false,
                                                    .max_value = true,
                                                    .type = doom_misc_default_type_boolean,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "max_player_corpse",
                                                    .location = {.pi = &doom_state->defaults_storage.max_player_corpse},
                                                    .default_value = {.i = 32},
                                                    .min_value = -1,
                                                    .max_value = max_unset,
                                                    .type = doom_misc_default_type_integer,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "flashing_hom",
                                                    .location = {.pb = &doom_state->defaults_storage.flashing_hom},
                                                    .default_value = {.b = false},
                                                    .min_value = false,
                                                    .max_value = true,
                                                    .type = doom_misc_default_type_boolean,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "endoom_mode",
                                                    .location = {.pi = &doom_state->defaults_storage.endoom_mode},
                                                    .default_value = {.i = 5},
                                                    .min_value = 0,
                                                    .max_value = 7,
                                                    .type = doom_misc_default_type_hex_integer,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "level_precache",
                                                    .location = {.pb = &doom_state->defaults_storage.level_precache},
                                                    .default_value = {.b = true},
                                                    .min_value = false,
                                                    .max_value = true,
                                                    .type = doom_misc_default_type_boolean,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "demo_smoothturns",
                                                    .location = {.pb = &doom_state->defaults_storage.demo_smoothturns},
                                                    .default_value = {.b = false},
                                                    .min_value = false,
                                                    .max_value = true,
                                                    .type = doom_misc_default_type_boolean,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                });
    doom_misc_default_dyarray_append(&defaults,
                                     (doom_misc_default_t){
                                         .name = "demo_smoothturnsfactor",
                                         .location = {.pi = &doom_state->defaults_storage.demo_smoothturnsfactor},
                                         .default_value = {.i = 6},
                                         .min_value = 1,
                                         .max_value = doom_demo_smoothturnsfactor_max,
                                         .type = doom_misc_default_type_integer,
                                         .setup_screen = doom_misc_setup_screen_status_bar,
                                     });

    doom_misc_default_dyarray_append(&defaults, s_header_default("Files"));
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "wadfile_1",
                                                    .location = {.ps = &doom_state->defaults_storage.wad_files[1]},
                                                    .default_value = {.s = phyto_string_span_empty()},
                                                    .min_value = min_unset,
                                                    .max_value = max_unset,
                                                    .type = doom_misc_default_type_string,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "wadfile_2",
                                                    .location = {.ps = &doom_state->defaults_storage.wad_files[2]},
                                                    .default_value = {.s = phyto_string_span_empty()},
                                                    .min_value = min_unset,
                                                    .max_value = max_unset,
                                                    .type = doom_misc_default_type_string,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "dehfile_1",
                                                    .location = {.ps = &doom_state->defaults_storage.deh_files[0]},
                                                    .default_value = {.s = phyto_string_span_empty()},
                                                    .min_value = min_unset,
                                                    .max_value = max_unset,
                                                    .type = doom_misc_default_type_string,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "dehfile_2",
                                                    .location = {.ps = &doom_state->defaults_storage.deh_files[1]},
                                                    .default_value = {.s = phyto_string_span_empty()},
                                                    .min_value = min_unset,
                                                    .max_value = max_unset,
                                                    .type = doom_misc_default_type_string,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });

    doom_misc_default_dyarray_append(&defaults, s_header_default("Game settings"));
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "default_skill",
                                                    .location = {.pi = &doom_state->defaults_storage.default_skill},
                                                    .default_value = {.i = 4},
                                                    .min_value = 0,
                                                    .max_value = 5,
                                                    .type = doom_misc_default_type_integer,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });
    doom_misc_default_dyarray_append(
        &defaults, (doom_misc_default_t){
                       .name = "weapon_attack_alignment",
                       .location = {.pi = (int*)&doom_state->defaults_storage.weapon_attack_alignment},
                       .default_value = {.i = doom_centerweapon_off},
                       .min_value = doom_centerweapon_off,
                       .max_value = doom_centerweapon_bob,
                       .type = doom_misc_default_type_integer,
                       .setup_screen = doom_misc_setup_screen_weapons,
                   });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "sts_always_red",
                                                    .location = {.pb = &doom_state->defaults_storage.sts_always_red},
                                                    .default_value = {.b = true},
                                                    .min_value = false,
                                                    .max_value = true,
                                                    .type = doom_misc_default_type_boolean,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                });
    doom_misc_default_dyarray_append(&defaults,
                                     (doom_misc_default_t){
                                         .name = "sts_pct_always_gray",
                                         .location = {.pb = &doom_state->defaults_storage.sts_pct_always_gray},
                                         .default_value = {.b = false},
                                         .min_value = false,
                                         .max_value = true,
                                         .type = doom_misc_default_type_boolean,
                                         .setup_screen = doom_misc_setup_screen_status_bar,
                                     });
    doom_misc_default_dyarray_append(&defaults,
                                     (doom_misc_default_t){
                                         .name = "sts_traditional_keys",
                                         .location = {.pb = &doom_state->defaults_storage.sts_traditional_keys},
                                         .default_value = {.b = false},
                                         .min_value = false,
                                         .max_value = true,
                                         .type = doom_misc_default_type_boolean,
                                         .setup_screen = doom_misc_setup_screen_status_bar,
                                     });
    doom_misc_default_dyarray_append(&defaults,
                                     (doom_misc_default_t){
                                         .name = "sts_armorcolor_type",
                                         .location = {.pi = (int*)&doom_state->defaults_storage.sts_armorcolor_type},
                                         .default_value = {.i = doom_armor_color_type_strength},
                                         .min_value = doom_armor_color_type_strength,
                                         .max_value = doom_armor_color_type_amount,
                                         .type = doom_misc_default_type_integer,
                                         .setup_screen = doom_misc_setup_screen_status_bar,
                                     });
    doom_misc_default_dyarray_append(&defaults,
                                     (doom_misc_default_t){
                                         .name = "show_messages",
                                         .location = {.pb = &doom_state->defaults_storage.dsda.show_messages},
                                         .default_value = {.b = true},
                                         .min_value = false,
                                         .max_value = true,
                                         .type = doom_misc_default_type_boolean,
                                         .setup_screen = doom_misc_setup_screen_none,
                                     });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "autorun",
                                                    .location = {.pb = &doom_state->defaults_storage.dsda.autorun},
                                                    .default_value = {.b = true},
                                                    .min_value = false,
                                                    .max_value = true,
                                                    .type = doom_misc_default_type_boolean,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });

    doom_misc_default_dyarray_append(&defaults, s_header_default("Dehacked settings"));
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "deh_apply_cheats",
                                                    .location = {.pb = &doom_state->defaults_storage.deh_apply_cheats},
                                                    .default_value = {.b = true},
                                                    .min_value = false,
                                                    .max_value = true,
                                                    .type = doom_misc_default_type_boolean,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                });

    doom_misc_default_dyarray_append(&defaults, s_header_default("Sound settings"));
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "snd_pcspeaker",
                                                    .location = {.pb = &doom_state->defaults_storage.snd_pcspeaker},
                                                    .default_value = {.b = false},
                                                    .min_value = false,
                                                    .max_value = true,
                                                    .type = doom_misc_default_type_boolean,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "sound_card",
                                                    .location = {.pi = &doom_state->defaults_storage.sound_card},
                                                    .default_value = {.i = -1},
                                                    .min_value = -1,
                                                    .max_value = 7,
                                                    .type = doom_misc_default_type_integer,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "music_card",
                                                    .location = {.pi = &doom_state->defaults_storage.music_card},
                                                    .default_value = {.i = -1},
                                                    .min_value = -1,
                                                    .max_value = 7,
                                                    .type = doom_misc_default_type_integer,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "pitched_sounds",
                                                    .location = {.pb = &doom_state->defaults_storage.pitched_sounds},
                                                    .default_value = {.b = false},
                                                    .min_value = false,
                                                    .max_value = true,
                                                    .type = doom_misc_default_type_boolean,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "samplerate",
                                                    .location = {.pi = &doom_state->defaults_storage.samplerate},
                                                    .default_value = {.i = 44100},
                                                    .min_value = 11025,
                                                    .max_value = 48000,
                                                    .type = doom_misc_default_type_integer,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "slice_samplecount",
                                                    .location = {.pi = &doom_state->defaults_storage.slice_samplecount},
                                                    .default_value = {.i = 512},
                                                    .min_value = 32,
                                                    .max_value = 8192,
                                                    .type = doom_misc_default_type_integer,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "sfx_volume",
                                                    .location = {.pi = &doom_state->defaults_storage.sfx_volume},
                                                    .default_value = {.i = 8},
                                                    .min_value = 0,
                                                    .max_value = 15,
                                                    .type = doom_misc_default_type_integer,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "music_volume",
                                                    .location = {.pi = &doom_state->defaults_storage.music_volume},
                                                    .default_value = {.i = 8},
                                                    .min_value = 0,
                                                    .max_value = 15,
                                                    .type = doom_misc_default_type_integer,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });
    doom_misc_default_dyarray_append(&defaults,
                                     (doom_misc_default_t){
                                         .name = "mus_pause_opt",
                                         .location = {.pi = (int*)&doom_state->defaults_storage.mus_pause_opt},
                                         .default_value = {.i = doom_mus_pause_opt_pause},
                                         .min_value = doom_mus_pause_opt_kill,
                                         .max_value = doom_mus_pause_opt_continue_playing,
                                         .type = doom_misc_default_type_integer,
                                         .setup_screen = doom_misc_setup_screen_none,
                                     });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "snd_channels",
                                                    .location = {.pi = &doom_state->defaults_storage.snd_channels},
                                                    .default_value = {.i = 32},
                                                    .min_value = 1,
                                                    .max_value = 32,
                                                    .type = doom_misc_default_type_integer,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "snd_midiplayer",
                                                    .location = {.ps = &doom_state->defaults_storage.snd_midiplayer},
                                                    .default_value = {.s = phyto_string_span_from_c("fluidsynth")},
                                                    .min_value = min_unset,
                                                    .max_value = max_unset,
                                                    .type = doom_misc_default_type_string,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });
    doom_misc_default_dyarray_append(
        &defaults, (doom_misc_default_t){
                       .name = "snd_soundfont",
                       .location = {.ps = &doom_state->defaults_storage.snd_soundfont},
                       .default_value = {.s = phyto_string_span_from_c("soundfonts/" PROJECT_NAME ".sf2")},
                       .min_value = min_unset,
                       .max_value = max_unset,
                       .type = doom_misc_default_type_string,
                       .setup_screen = doom_misc_setup_screen_none,
                   });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "snd_mididev",
                                                    .location = {.ps = &doom_state->defaults_storage.snd_mididev},
                                                    .default_value = {.s = phyto_string_span_empty()},
                                                    .min_value = min_unset,
                                                    .max_value = max_unset,
                                                    .type = doom_misc_default_type_string,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "full_sounds",
                                                    .location = {.pb = &doom_state->defaults_storage.full_sounds},
                                                    .default_value = {.b = false},
                                                    .min_value = false,
                                                    .max_value = true,
                                                    .type = doom_misc_default_type_boolean,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });
    doom_misc_default_dyarray_append(&defaults,
                                     (doom_misc_default_t){
                                         .name = "mus_fluidsynth_chorus",
                                         .location = {.pb = &doom_state->defaults_storage.mus_fluidsynth_chorus},
                                         .default_value = {.b = false},
                                         .min_value = false,
                                         .max_value = true,
                                         .type = doom_misc_default_type_boolean,
                                         .setup_screen = doom_misc_setup_screen_none,
                                     });
    doom_misc_default_dyarray_append(&defaults,
                                     (doom_misc_default_t){
                                         .name = "mus_fluidsynth_reverb",
                                         .location = {.pb = &doom_state->defaults_storage.mus_fluidsynth_reverb},
                                         .default_value = {.b = false},
                                         .min_value = false,
                                         .max_value = true,
                                         .type = doom_misc_default_type_boolean,
                                         .setup_screen = doom_misc_setup_screen_none,
                                     });
    doom_misc_default_dyarray_append(&defaults,
                                     (doom_misc_default_t){
                                         .name = "mus_fluidsynth_gain",
                                         .location = {.pi = &doom_state->defaults_storage.mus_fluidsynth_gain},
                                         .default_value = {.i = 50},
                                         .min_value = 0,
                                         .max_value = 1000,
                                         .type = doom_misc_default_type_integer,
                                         .setup_screen = doom_misc_setup_screen_none,
                                     });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "mus_opl_gain",
                                                    .location = {.pi = &doom_state->defaults_storage.mus_opl_gain},
                                                    .default_value = {.i = 50},
                                                    .min_value = 0,
                                                    .max_value = 1000,
                                                    .type = doom_misc_default_type_integer,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });

    doom_misc_default_dyarray_append(&defaults, s_header_default("Video settings"));
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "videomode",
                                                    .location = {.ps = &doom_state->defaults_storage.videomode},
                                                    .default_value = {.s = phyto_string_span_from_c("Software")},
                                                    .min_value = min_unset,
                                                    .max_value = max_unset,
                                                    .type = doom_misc_default_type_string,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "screen_resolution",
                                                    .location = {.ps = &doom_state->defaults_storage.screen_resolution},
                                                    .default_value = {.s = phyto_string_span_from_c("640x480")},
                                                    .min_value = min_unset,
                                                    .max_value = max_unset,
                                                    .type = doom_misc_default_type_string,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "custom_resoltuion",
                                                    .location = {.ps = &doom_state->defaults_storage.custom_resolution},
                                                    .default_value = {.s = phyto_string_span_empty()},
                                                    .min_value = min_unset,
                                                    .max_value = max_unset,
                                                    .type = doom_misc_default_type_string,
                                                    .setup_screen = doom_misc_setup_screen_chat,
                                                });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "use_fullscreen",
                                                    .location = {.pb = &doom_state->defaults_storage.use_fullscreen},
                                                    .default_value = {.b = false},
                                                    .min_value = false,
                                                    .max_value = true,
                                                    .type = doom_misc_default_type_boolean,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });
    doom_misc_default_dyarray_append(&defaults,
                                     (doom_misc_default_t){
                                         .name = "exclusive_fullscreen",
                                         .location = {.pb = &doom_state->defaults_storage.exclusive_fullscreen},
                                         .default_value = {.b = false},
                                         .min_value = false,
                                         .max_value = true,
                                         .type = doom_misc_default_type_boolean,
                                         .setup_screen = doom_misc_setup_screen_none,
                                     });
    doom_misc_default_dyarray_append(&defaults,
                                     (doom_misc_default_t){
                                         .name = "gl_exclusive_fullscreen",
                                         .location = {.pb = &doom_state->defaults_storage.gl_exclusive_fullscreen},
                                         .default_value = {.b = true},
                                         .min_value = false,
                                         .max_value = true,
                                         .type = doom_misc_default_type_boolean,
                                         .setup_screen = doom_misc_setup_screen_none,
                                     });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "render_vsync",
                                                    .location = {.pb = &doom_state->defaults_storage.render_vsync},
                                                    .default_value = {.b = true},
                                                    .min_value = false,
                                                    .max_value = true,
                                                    .type = doom_misc_default_type_boolean,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
                                                    .name = "tran_filter_pct",
                                                    .location = {.pi = &doom_state->defaults_storage.tran_filter_pct},
                                                    .default_value = {.i = 66},
                                                    .min_value = 0,
                                                    .max_value = 100,
                                                    .type = doom_misc_default_type_integer,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                });
    doom_misc_default_dyarray_append(&defaults, (doom_misc_default_t){
        .name = "screenblocks",
        .location = {.pi = &doom_state->defaults_storage.screenblocks},
        .default_value = {.i = 10},
        .min_value = 3,
        .max_value = 11,
        .type = doom_misc_default_type_integer,
        .setup_screen = doom_misc_setup_screen_none,
    });

    return defaults;
}

void doom_misc_load_defaults(void) {
}

doom_misc_default_t s_header_default(const char* name) {
    // Since C semantics make unset values equal to 0 in designated initializers, we can use a shorthand for header
    // defaults.
    return (doom_misc_default_t){
        .name = name,
        .min_value = min_unset,
        .max_value = max_unset,
    };
}
