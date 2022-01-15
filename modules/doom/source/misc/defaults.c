#include "doom/misc/defaults.h"

#include "doom/dsda/input.h"
#include "doom/gl/struct.h"
#include "doom/init.h"
#include "doom/keys.h"
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

typedef struct {
    const char* name;
    doom_misc_setup_screen_t setup_screen;
    doom_dsda_input_identifier_t identifier;
    doom_dsda_input_default_t input;
} input_default_t;

static doom_misc_default_t s_header_default(const char* name);
static doom_misc_default_t s_boolean_default(boolean_default_t value);
static doom_misc_default_t s_string_default(string_default_t value);
static doom_misc_default_t s_integer_default(integer_default_t value);
static doom_misc_default_t s_hexint_default(integer_default_t value);
static doom_misc_default_t s_input_default(input_default_t value);

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
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "filter_floor",
                                                    .location = (int*)&doom_state->defaults_storage.filter_floor,
                                                    .default_value = doom_render_draw_filter_type_point,
                                                    .min_value = doom_render_draw_filter_type_point,
                                                    .max_value = doom_render_draw_filter_type_rounded,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "filter_sprite",
                                                    .location = (int*)&doom_state->defaults_storage.filter_sprite,
                                                    .default_value = doom_render_draw_filter_type_point,
                                                    .min_value = doom_render_draw_filter_type_point,
                                                    .max_value = doom_render_draw_filter_type_rounded,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "filter_z",
                                                    .location = (int*)&doom_state->defaults_storage.filter_z,
                                                    .default_value = doom_render_draw_filter_type_point,
                                                    .min_value = doom_render_draw_filter_type_point,
                                                    .max_value = doom_render_draw_filter_type_linear,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "filter_patch",
                                                    .location = (int*)&doom_state->defaults_storage.filter_patch,
                                                    .default_value = doom_render_draw_filter_type_point,
                                                    .min_value = doom_render_draw_filter_type_point,
                                                    .max_value = doom_render_draw_filter_type_rounded,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "filter_threshold",
                                                    .location = &doom_state->defaults_storage.filter_threshold,
                                                    .default_value = 49152,
                                                    .min_value = 0,
                                                    .max_value = max_unset,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "sprite_edges",
                                                    .location = (int*)&doom_state->defaults_storage.sprite_edges,
                                                    .default_value = doom_render_draw_sloped_edge_type_square,
                                                    .min_value = doom_render_draw_sloped_edge_type_square,
                                                    .max_value = doom_render_draw_sloped_edge_type_sloped,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "patch_edges",
                                                    .location = (int*)&doom_state->defaults_storage.patch_edges,
                                                    .default_value = doom_render_draw_sloped_edge_type_square,
                                                    .min_value = doom_render_draw_sloped_edge_type_square,
                                                    .max_value = doom_render_draw_sloped_edge_type_sloped,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));

    doom_misc_default_dyarray_append(&defaults, s_header_default("OpenGL settings"));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "gl_compatibility",
                                                    .location = &doom_state->defaults_storage.gl_compatibility,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "gl_arb_multitexture",
                                                    .location = &doom_state->defaults_storage.gl_arb.multitexture,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_boolean_default((boolean_default_t){
                                         .name = "gl_arb_texture_compression",
                                         .location = &doom_state->defaults_storage.gl_arb.texture_compression,
                                         .default_value = true,
                                         .setup_screen = doom_misc_setup_screen_status_bar,
                                     }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_boolean_default((boolean_default_t){
                                         .name = "gl_arb_texture_non_power_of_two",
                                         .location = &doom_state->defaults_storage.gl_arb.texture_non_power_of_two,
                                         .default_value = true,
                                         .setup_screen = doom_misc_setup_screen_status_bar,
                                     }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_boolean_default((boolean_default_t){
                                         .name = "gl_ext_arb_vertex_buffer_object",
                                         .location = &doom_state->defaults_storage.gl_ext.arb_vertex_buffer_object,
                                         .default_value = true,
                                         .setup_screen = doom_misc_setup_screen_status_bar,
                                     }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_boolean_default((boolean_default_t){
                                         .name = "gl_arb_pixel_buffer_object",
                                         .location = &doom_state->defaults_storage.gl_arb.pixel_buffer_object,
                                         .default_value = true,
                                         .setup_screen = doom_misc_setup_screen_status_bar,
                                     }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "gl_arb_shader_objects",
                                                    .location = &doom_state->defaults_storage.gl_arb.shader_objects,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "gl_ext_blend_color",
                                                    .location = &doom_state->defaults_storage.gl_ext.blend_color,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "gl_ext_framebuffer_object",
                                                    .location = &doom_state->defaults_storage.gl_ext.framebuffer_object,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_boolean_default((boolean_default_t){
                                         .name = "gl_ext_packed_depth_stencil",
                                         .location = &doom_state->defaults_storage.gl_ext.packed_depth_stencil,
                                         .default_value = true,
                                         .setup_screen = doom_misc_setup_screen_status_bar,
                                     }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_boolean_default((boolean_default_t){
                                         .name = "gl_ext_texture_filter_anisotropic",
                                         .location = &doom_state->defaults_storage.gl_ext.texture_filter_anisotropic,
                                         .default_value = true,
                                         .setup_screen = doom_misc_setup_screen_status_bar,
                                     }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "gl_use_stencil",
                                                    .location = &doom_state->defaults_storage.gl_use_stencil,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "gl_use_display_lists",
                                                    .location = &doom_state->defaults_storage.gl_use_display_lists,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "gl_finish",
                                                    .location = &doom_state->defaults_storage.gl_finish,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "gl_clear",
                                                    .location = &doom_state->defaults_storage.gl_clear,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "gl_ztrick",
                                                    .location = &doom_state->defaults_storage.gl_ztrick,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "gl_nearclip",
                                                    .location = &doom_state->defaults_storage.gl_nearclip,
                                                    .default_value = 5,
                                                    .min_value = 0,
                                                    .max_value = max_unset,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "gl_colorbuffer_bits",
                                                    .location = &doom_state->defaults_storage.gl_colorbuffer_bits,
                                                    .default_value = 32,
                                                    .min_value = 16,
                                                    .max_value = 32,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "gl_depthbuffer_bits",
                                                    .location = &doom_state->defaults_storage.gl_depthbuffer_bits,
                                                    .default_value = 24,
                                                    .min_value = 16,
                                                    .max_value = 32,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_integer_default((integer_default_t){
                                         .name = "gl_texture_filter",
                                         .location = (int*)&doom_state->defaults_storage.gl_texture_filter,
                                         .default_value = doom_gl_struct_filter_texture_mode_nearest_mipmap_linear,
                                         .min_value = doom_gl_struct_filter_texture_mode_nearest,
                                         .max_value = doom_gl_struct_filter_texture_mode_count - 1,
                                         .setup_screen = doom_misc_setup_screen_none,
                                     }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_integer_default((integer_default_t){
                                         .name = "gl_sprite_filter",
                                         .location = (int*)&doom_state->defaults_storage.gl_sprite_filter,
                                         .default_value = doom_gl_struct_filter_texture_mode_nearest,
                                         .min_value = doom_gl_struct_filter_texture_mode_nearest,
                                         .max_value = doom_gl_struct_filter_texture_mode_linear_mipmap_nearest,
                                         .setup_screen = doom_misc_setup_screen_none,
                                     }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "gl_patch_filter",
                                                    .location = (int*)&doom_state->defaults_storage.gl_patch_filter,
                                                    .default_value = doom_gl_struct_filter_texture_mode_nearest,
                                                    .min_value = doom_gl_struct_filter_texture_mode_nearest,
                                                    .max_value = doom_gl_struct_filter_texture_mode_linear,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_integer_default((integer_default_t){
                                         .name = "gl_texture_filter_anisotropic",
                                         .location = (int*)&doom_state->defaults_storage.gl_texture_filter_anisotropic,
                                         .default_value = doom_gl_struct_anisotropic_mode_on_8x,
                                         .min_value = doom_gl_struct_anisotropic_mode_off,
                                         .max_value = doom_gl_struct_anisotropic_mode_on_16x,
                                         .setup_screen = doom_misc_setup_screen_none,
                                     }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "gl_tex_format_string",
                                                    .location = &doom_state->defaults_storage.gl_tex_format_string,
                                                    .default_value = phyto_string_span_from_c("GL_RGBA"),
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "gl_sprite_offset",
                                                    .location = &doom_state->defaults_storage.gl_sprite_offset,
                                                    .default_value = 0,
                                                    .min_value = 0,
                                                    .max_value = 5,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "gl_sprite_blend",
                                                    .location = &doom_state->defaults_storage.gl_sprite_blend,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "gl_mask_sprite_threshold",
                                                    .location = &doom_state->defaults_storage.gl_mask_sprite_threshold,
                                                    .default_value = 50,
                                                    .min_value = 0,
                                                    .max_value = 100,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "gl_skymode",
                                                    .location = (int*)&doom_state->defaults_storage.gl_skymode,
                                                    .default_value = doom_gl_struct_skytype_auto,
                                                    .min_value = doom_gl_struct_skytype_auto,
                                                    .max_value = doom_gl_struct_skytype_count - 1,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "gl_sky_detail",
                                                    .location = &doom_state->defaults_storage.gl_sky_detail,
                                                    .default_value = 16,
                                                    .min_value = 1,
                                                    .max_value = 32,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "gl_use_paletted_texture",
                                                    .location = &doom_state->defaults_storage.gl_use_paletted_texture,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_boolean_default((boolean_default_t){
                                         .name = "gl_use_shared_texture_palette",
                                         .location = &doom_state->defaults_storage.gl_use_shared_texture_palette,
                                         .default_value = false,
                                         .setup_screen = doom_misc_setup_screen_none,
                                     }));

    doom_misc_default_dyarray_append(&defaults, s_header_default("Input settings"));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "input_profile",
                                                    .location = &doom_state->defaults_storage.input_profile,
                                                    .default_value = 0,
                                                    .min_value = 0,
                                                    .max_value = doom_dsda_input_profile_count - 1,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_forward",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_forward,
                                                    .input = {.key = 'w', .mouse_button = 2, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_backward",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_backward,
                                                    .input = {.key = 's', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_turnleft",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_turn_left,
                                                    .input = {.key = 'e', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_turnright",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_turn_right,
                                                    .input = {.key = 'q', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(
        &defaults, s_input_default((input_default_t){
                       .name = "input_speed",
                       .setup_screen = doom_misc_setup_screen_keys,
                       .identifier = doom_dsda_input_speed,
                       .input = {.key = doom_key_right_shift, .mouse_button = -1, .joystick_button = -1},
                   }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_strafeleft",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_strafe_left,
                                                    .input = {.key = 'a', .mouse_button = -1, .joystick_button = 4},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_straferight",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_strafe_right,
                                                    .input = {.key = 'd', .mouse_button = -1, .joystick_button = 5},
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

doom_misc_default_t s_input_default(input_default_t value) {
    return (doom_misc_default_t){
        .name = value.name,
        .min_value = min_unset,
        .max_value = max_unset,
        .type = doom_misc_default_type_input,
        .setup_screen = value.setup_screen,
        .identifier = value.identifier,
        .input = value.input,
    };
}
