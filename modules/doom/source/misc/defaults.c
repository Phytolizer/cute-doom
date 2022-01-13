#include "doom/misc/defaults.h"

#include "doom/init.h"
#include "doom/render/demo.h"
#include "doom/render/draw.h"
#include "doom/state.h"
#include "phyto/string/string.h"

#include <config.h>
#include <stdint.h>

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

typedef struct {
    const char* name;
    bool* location;
    bool default_value;
    doom_misc_setup_screen_t setup_screen;
} boolean_default_t;

typedef struct {
    const char* name;
    phyto_string_span_t* location;
    phyto_string_span_t default_value;
    doom_misc_setup_screen_t setup_screen;
} string_default_t;

typedef struct {
    const char* name;
    int32_t* location;
    int32_t default_value;
    int32_t min_value;
    int32_t max_value;
    doom_misc_setup_screen_t setup_screen;
} integer_default_t;

static doom_misc_default_t s_header_default(const char* name);
static doom_misc_default_t s_boolean_default(boolean_default_t value);
static doom_misc_default_t s_string_default(string_default_t value);
static doom_misc_default_t s_integer_default(integer_default_t value);
static doom_misc_default_t s_hexint_default(integer_default_t value);

doom_misc_default_dyarray_t doom_misc_default_dyarray_new(void) {
    doom_misc_default_dyarray_t defaults = doom_misc_default_dyarray_init(&sc_default_dyarray_callbacks);

    doom_misc_default_dyarray_append(&defaults, s_header_default("System settings"));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "process_priority",
                                                    .location = &doom_state->defaults_storage.process_priority,
                                                    .default_value = 0,
                                                    .min_value = 0,
                                                    .max_value = 2,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));

    doom_misc_default_dyarray_append(&defaults, s_header_default("Misc settings"));
    doom_misc_default_dyarray_append(&defaults,
                                     s_integer_default((integer_default_t){
                                         .name = "default_compatibility_level",
                                         .location = (int*)&doom_state->defaults_storage.default_compatibility_level,
                                         .default_value = -1,
                                         .min_value = -1,
                                         .max_value = doom_compatibility_level_max - 1,
                                         .setup_screen = doom_misc_setup_screen_none,
                                     }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "realtic_clock_rate",
                                                    .location = &doom_state->defaults_storage.realtic_clock_rate,
                                                    .default_value = 100,
                                                    .min_value = 0,
                                                    .max_value = max_unset,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "menu_background",
                                                    .location = &doom_state->defaults_storage.menu_background,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "max_player_corpse",
                                                    .location = &doom_state->defaults_storage.max_player_corpse,
                                                    .default_value = 32,
                                                    .min_value = -1,
                                                    .max_value = max_unset,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "flashing_hom",
                                                    .location = &doom_state->defaults_storage.flashing_hom,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_hexint_default((integer_default_t){
                                                    .name = "endoom_mode",
                                                    .location = &doom_state->defaults_storage.endoom_mode,
                                                    .default_value = 5,
                                                    .min_value = 0,
                                                    .max_value = 7,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "level_precache",
                                                    .location = &doom_state->defaults_storage.level_precache,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "demo_smoothturns",
                                                    .location = &doom_state->defaults_storage.demo_smoothturns,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "demo_smoothturnsfactor",
                                                    .location = &doom_state->defaults_storage.demo_smoothturnsfactor,
                                                    .default_value = 6,
                                                    .min_value = 1,
                                                    .max_value = doom_demo_smoothturnsfactor_max,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));

    doom_misc_default_dyarray_append(&defaults, s_header_default("Files"));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "wadfile_1",
                                                    .location = &doom_state->defaults_storage.wad_files[1],
                                                    .default_value = phyto_string_span_empty(),
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "wadfile_2",
                                                    .location = &doom_state->defaults_storage.wad_files[2],
                                                    .default_value = phyto_string_span_empty(),
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "dehfile_1",
                                                    .location = &doom_state->defaults_storage.deh_files[0],
                                                    .default_value = phyto_string_span_empty(),
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "dehfile_2",
                                                    .location = &doom_state->defaults_storage.deh_files[1],
                                                    .default_value = phyto_string_span_empty(),
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));

    doom_misc_default_dyarray_append(&defaults, s_header_default("Game settings"));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "default_skill",
                                                    .location = &doom_state->defaults_storage.default_skill,
                                                    .default_value = 4,
                                                    .min_value = 0,
                                                    .max_value = 5,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_integer_default((integer_default_t){
                                         .name = "weapon_attack_alignment",
                                         .location = (int*)&doom_state->defaults_storage.weapon_attack_alignment,
                                         .default_value = doom_centerweapon_off,
                                         .min_value = doom_centerweapon_off,
                                         .max_value = doom_centerweapon_bob,
                                         .setup_screen = doom_misc_setup_screen_weapons,
                                     }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "sts_always_red",
                                                    .location = &doom_state->defaults_storage.sts_always_red,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "sts_pct_always_gray",
                                                    .location = &doom_state->defaults_storage.sts_pct_always_gray,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "sts_traditional_keys",
                                                    .location = &doom_state->defaults_storage.sts_traditional_keys,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "sts_armorcolor_type",
                                                    .location = (int*)&doom_state->defaults_storage.sts_armorcolor_type,
                                                    .default_value = doom_armor_color_type_strength,
                                                    .min_value = doom_armor_color_type_strength,
                                                    .max_value = doom_armor_color_type_amount,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "show_messages",
                                                    .location = &doom_state->defaults_storage.dsda.show_messages,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "autorun",
                                                    .location = &doom_state->defaults_storage.dsda.autorun,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));

    doom_misc_default_dyarray_append(&defaults, s_header_default("Dehacked settings"));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "deh_apply_cheats",
                                                    .location = &doom_state->defaults_storage.deh_apply_cheats,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));

    doom_misc_default_dyarray_append(&defaults, s_header_default("Sound settings"));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "snd_pcspeaker",
                                                    .location = &doom_state->defaults_storage.snd_pcspeaker,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "sound_card",
                                                    .location = &doom_state->defaults_storage.sound_card,
                                                    .default_value = -1,
                                                    .min_value = -1,
                                                    .max_value = 7,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "music_card",
                                                    .location = &doom_state->defaults_storage.music_card,
                                                    .default_value = -1,
                                                    .min_value = -1,
                                                    .max_value = 7,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "pitched_sounds",
                                                    .location = &doom_state->defaults_storage.pitched_sounds,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "samplerate",
                                                    .location = &doom_state->defaults_storage.samplerate,
                                                    .default_value = 44100,
                                                    .min_value = 11025,
                                                    .max_value = 48000,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "slice_samplecount",
                                                    .location = &doom_state->defaults_storage.slice_samplecount,
                                                    .default_value = 512,
                                                    .min_value = 32,
                                                    .max_value = 8192,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "sfx_volume",
                                                    .location = &doom_state->defaults_storage.sfx_volume,
                                                    .default_value = 8,
                                                    .min_value = 0,
                                                    .max_value = 15,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "music_volume",
                                                    .location = &doom_state->defaults_storage.music_volume,
                                                    .default_value = 8,
                                                    .min_value = 0,
                                                    .max_value = 15,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mus_pause_opt",
                                                    .location = (int*)&doom_state->defaults_storage.mus_pause_opt,
                                                    .default_value = doom_mus_pause_opt_pause,
                                                    .min_value = doom_mus_pause_opt_kill,
                                                    .max_value = doom_mus_pause_opt_continue_playing,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "snd_channels",
                                                    .location = &doom_state->defaults_storage.snd_channels,
                                                    .default_value = 32,
                                                    .min_value = 1,
                                                    .max_value = 32,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "snd_midiplayer",
                                                    .location = &doom_state->defaults_storage.snd_midiplayer,
                                                    .default_value = phyto_string_span_from_c("fluidsynth"),
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_string_default((string_default_t){
                                         .name = "snd_soundfont",
                                         .location = &doom_state->defaults_storage.snd_soundfont,
                                         .default_value = phyto_string_span_from_c("soundfonts/" PROJECT_NAME ".sf2"),
                                         .setup_screen = doom_misc_setup_screen_none,
                                     }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "snd_mididev",
                                                    .location = &doom_state->defaults_storage.snd_mididev,
                                                    .default_value = phyto_string_span_empty(),
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "full_sounds",
                                                    .location = &doom_state->defaults_storage.full_sounds,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "mus_fluidsynth_chorus",
                                                    .location = &doom_state->defaults_storage.mus_fluidsynth_chorus,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "mus_fluidsynth_reverb",
                                                    .location = &doom_state->defaults_storage.mus_fluidsynth_reverb,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mus_fluidsynth_gain",
                                                    .location = &doom_state->defaults_storage.mus_fluidsynth_gain,
                                                    .default_value = 50,
                                                    .min_value = 0,
                                                    .max_value = 1000,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mus_opl_gain",
                                                    .location = &doom_state->defaults_storage.mus_opl_gain,
                                                    .default_value = 50,
                                                    .min_value = 0,
                                                    .max_value = 1000,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));

    doom_misc_default_dyarray_append(&defaults, s_header_default("Video settings"));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "videomode",
                                                    .location = &doom_state->defaults_storage.videomode,
                                                    .default_value = phyto_string_span_from_c("Software"),
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "screen_resolution",
                                                    .location = &doom_state->defaults_storage.screen_resolution,
                                                    .default_value = phyto_string_span_from_c("640x480"),
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "custom_resoltuion",
                                                    .location = &doom_state->defaults_storage.custom_resolution,
                                                    .default_value = phyto_string_span_empty(),
                                                    .setup_screen = doom_misc_setup_screen_chat,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "use_fullscreen",
                                                    .location = &doom_state->defaults_storage.use_fullscreen,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "exclusive_fullscreen",
                                                    .location = &doom_state->defaults_storage.exclusive_fullscreen,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "gl_exclusive_fullscreen",
                                                    .location = &doom_state->defaults_storage.gl_exclusive_fullscreen,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "render_vsync",
                                                    .location = &doom_state->defaults_storage.render_vsync,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "tran_filter_pct",
                                                    .location = &doom_state->defaults_storage.tran_filter_pct,
                                                    .default_value = 66,
                                                    .min_value = 0,
                                                    .max_value = 100,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "screenblocks",
                                                    .location = &doom_state->defaults_storage.screenblocks,
                                                    .default_value = 10,
                                                    .min_value = 3,
                                                    .max_value = 11,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "usegamma",
                                                    .location = &doom_state->defaults_storage.usegamma,
                                                    .default_value = 0,
                                                    .min_value = 0,
                                                    .max_value = 4,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "uncapped_framerate",
                                                    .location = &doom_state->defaults_storage.uncapped_framerate,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "dsda_fps_limit",
                                                    .location = &doom_state->defaults_storage.dsda.fps_limit,
                                                    .default_value = 0,
                                                    .min_value = 0,
                                                    .max_value = 1000,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "filter_wall",
                                                    .location = (int*)&doom_state->defaults_storage.filter_wall,
                                                    .default_value = doom_render_draw_filter_type_point,
                                                    .min_value = doom_render_draw_filter_type_point,
                                                    .max_value = doom_render_draw_filter_type_rounded,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));

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

doom_misc_default_t s_boolean_default(boolean_default_t value) {
    return (doom_misc_default_t){
        .name = value.name,
        .location = {.pb = value.location},
        .default_value = {.b = value.default_value},
        .min_value = false,
        .max_value = true,
        .type = doom_misc_default_type_boolean,
        .setup_screen = value.setup_screen,
    };
}

doom_misc_default_t s_string_default(string_default_t value) {
    return (doom_misc_default_t){
        .name = value.name,
        .location = {.ps = value.location},
        .default_value = {.s = value.default_value},
        .min_value = min_unset,
        .max_value = max_unset,
        .type = doom_misc_default_type_string,
        .setup_screen = value.setup_screen,
    };
}

doom_misc_default_t s_integer_default(integer_default_t value) {
    return (doom_misc_default_t){
        .name = value.name,
        .location = {.pi = value.location},
        .default_value = {.i = value.default_value},
        .min_value = value.min_value,
        .max_value = value.max_value,
        .type = doom_misc_default_type_integer,
        .setup_screen = value.setup_screen,
    };
}

doom_misc_default_t s_hexint_default(integer_default_t value) {
    return (doom_misc_default_t){
        .name = value.name,
        .location = {.pi = value.location},
        .default_value = {.i = value.default_value},
        .min_value = value.min_value,
        .max_value = value.max_value,
        .type = doom_misc_default_type_hex_integer,
        .setup_screen = value.setup_screen,
    };
}
