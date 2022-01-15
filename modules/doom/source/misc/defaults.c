#include "doom/misc/defaults.h"

#include "doom/automap.h"
#include "doom/dsda/input.h"
#include "doom/gl/struct.h"
#include "doom/hud/crosshair.h"
#include "doom/hud/strings.h"
#include "doom/init.h"
#include "doom/keys.h"
#include "doom/render/demo.h"
#include "doom/render/draw.h"
#include "doom/render/things.h"
#include "doom/state.h"
#include "doom/status_bar/ammo.h"
#include "doom/sys/compatibility.h"
#include "doom/video/patch.h"
#include "phyto/string/string.h"

#include <assert.h>
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
    uint32_t* location;
    uint32_t default_value;
    int32_t min_value;
    int32_t max_value;
    doom_misc_setup_screen_t setup_screen;
} hexint_default_t;

typedef struct {
    const char* name;
    doom_misc_setup_screen_t setup_screen;
    doom_dsda_input_identifier_t identifier;
    doom_dsda_input_default_t input;
} input_default_t;

typedef struct {
    const char* name;
    phyto_string_t* location;
    phyto_string_span_t default_value;
    doom_misc_setup_screen_t setup_screen;
} owning_string_default_t;

static doom_misc_default_t s_header_default(const char* name);
static doom_misc_default_t s_boolean_default(boolean_default_t value);
static doom_misc_default_t s_string_default(string_default_t value);
static doom_misc_default_t s_integer_default(integer_default_t value);
static doom_misc_default_t s_hexint_default(hexint_default_t value);
static doom_misc_default_t s_input_default(input_default_t value);
static doom_misc_default_t s_owning_string_default(owning_string_default_t value);

doom_misc_default_dyarray_t doom_misc_default_dyarray_new(void) {
    static_assert(sizeof(doom_compatibility_level_t) == sizeof(int), "i can't cast enums to ints");

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
    doom_misc_default_dyarray_append(&defaults, s_hexint_default((hexint_default_t){
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
    doom_misc_default_dyarray_append(&defaults,
                                     s_input_default((input_default_t){
                                         .name = "input_strafe",
                                         .setup_screen = doom_misc_setup_screen_keys,
                                         .identifier = doom_dsda_input_strafe,
                                         .input = {.key = doom_key_right_alt, .mouse_button = 1, .joystick_button = 1},
                                     }));
    doom_misc_default_dyarray_append(
        &defaults, s_input_default((input_default_t){
                       .name = "input_autorun",
                       .setup_screen = doom_misc_setup_screen_keys,
                       .identifier = doom_dsda_input_autorun,
                       .input = {.key = doom_key_caps_lock, .mouse_button = -1, .joystick_button = -1},
                   }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_reverse",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_reverse,
                                                    .input = {.key = '/', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_use",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_use,
                                                    .input = {.key = ' ', .mouse_button = -1, .joystick_button = 3},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_flyup",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_fly_up,
                                                    .input = {.key = '.', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_flydown",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_fly_down,
                                                    .input = {.key = ',', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_flycenter",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_fly_center,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_mlook",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_mouselook,
                                                    .input = {.key = '\\', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_novert",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_novert,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_weapon1",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_weapon1,
                                                    .input = {.key = '1', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_weapon2",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_weapon2,
                                                    .input = {.key = '2', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_weapon3",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_weapon3,
                                                    .input = {.key = '3', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_weapon4",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_weapon4,
                                                    .input = {.key = '4', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_weapon5",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_weapon5,
                                                    .input = {.key = '5', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_weapon6",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_weapon6,
                                                    .input = {.key = '6', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_weapon7",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_weapon7,
                                                    .input = {.key = '7', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_weapon8",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_weapon8,
                                                    .input = {.key = '8', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_weapon9",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_weapon9,
                                                    .input = {.key = '9', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(
        &defaults, s_input_default((input_default_t){
                       .name = "input_nextweapon",
                       .setup_screen = doom_misc_setup_screen_keys,
                       .identifier = doom_dsda_input_next_weapon,
                       .input = {.key = doom_key_mouse_wheel_up, .mouse_button = -1, .joystick_button = -1},
                   }));
    doom_misc_default_dyarray_append(
        &defaults, s_input_default((input_default_t){
                       .name = "input_prevweapon",
                       .setup_screen = doom_misc_setup_screen_keys,
                       .identifier = doom_dsda_input_prev_weapon,
                       .input = {.key = doom_key_mouse_wheel_down, .mouse_button = -1, .joystick_button = -1},
                   }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_toggleweapon",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_toggle_weapon,
                                                    .input = {.key = '0', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_input_default((input_default_t){
                                         .name = "input_fire",
                                         .setup_screen = doom_misc_setup_screen_keys,
                                         .identifier = doom_dsda_input_fire,
                                         .input = {.key = doom_key_right_ctrl, .mouse_button = 0, .joystick_button = 0},
                                     }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_setup",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_setup,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_input_default((input_default_t){
                                         .name = "input_pause",
                                         .setup_screen = doom_misc_setup_screen_keys,
                                         .identifier = doom_dsda_input_pause,
                                         .input = {.key = doom_key_pause, .mouse_button = -1, .joystick_button = -1},
                                     }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_input_default((input_default_t){
                                         .name = "input_map",
                                         .setup_screen = doom_misc_setup_screen_keys,
                                         .identifier = doom_dsda_input_map,
                                         .input = {.key = doom_key_tab, .mouse_button = -1, .joystick_button = -1},
                                     }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_input_default((input_default_t){
                                         .name = "input_soundvolume",
                                         .setup_screen = doom_misc_setup_screen_keys,
                                         .identifier = doom_dsda_input_sound_volume,
                                         .input = {.key = doom_key_f4, .mouse_button = -1, .joystick_button = -1},
                                     }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_input_default((input_default_t){
                                         .name = "input_hud",
                                         .setup_screen = doom_misc_setup_screen_keys,
                                         .identifier = doom_dsda_input_hud,
                                         .input = {.key = doom_key_f5, .mouse_button = -1, .joystick_button = -1},
                                     }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_input_default((input_default_t){
                                         .name = "input_messages",
                                         .setup_screen = doom_misc_setup_screen_keys,
                                         .identifier = doom_dsda_input_messages,
                                         .input = {.key = doom_key_f8, .mouse_button = -1, .joystick_button = -1},
                                     }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_input_default((input_default_t){
                                         .name = "input_gamma",
                                         .setup_screen = doom_misc_setup_screen_keys,
                                         .identifier = doom_dsda_input_gamma,
                                         .input = {.key = doom_key_f11, .mouse_button = -1, .joystick_button = -1},
                                     }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_input_default((input_default_t){
                                         .name = "input_spy",
                                         .setup_screen = doom_misc_setup_screen_keys,
                                         .identifier = doom_dsda_input_spy,
                                         .input = {.key = doom_key_f12, .mouse_button = -1, .joystick_button = -1},
                                     }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_zoomin",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_zoom_in,
                                                    .input = {.key = '=', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_zoomout",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_zoom_out,
                                                    .input = {.key = '-', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_screenshot",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_screenshot,
                                                    .input = {.key = '*', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_input_default((input_default_t){
                                         .name = "input_savegame",
                                         .setup_screen = doom_misc_setup_screen_keys,
                                         .identifier = doom_dsda_input_save_game,
                                         .input = {.key = doom_key_f2, .mouse_button = -1, .joystick_button = -1},
                                     }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_input_default((input_default_t){
                                         .name = "input_loadgame",
                                         .setup_screen = doom_misc_setup_screen_keys,
                                         .identifier = doom_dsda_input_load_game,
                                         .input = {.key = doom_key_f3, .mouse_button = -1, .joystick_button = -1},
                                     }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_input_default((input_default_t){
                                         .name = "input_quicksave",
                                         .setup_screen = doom_misc_setup_screen_keys,
                                         .identifier = doom_dsda_input_quick_save,
                                         .input = {.key = doom_key_f6, .mouse_button = -1, .joystick_button = -1},
                                     }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_input_default((input_default_t){
                                         .name = "input_quickload",
                                         .setup_screen = doom_misc_setup_screen_keys,
                                         .identifier = doom_dsda_input_quick_load,
                                         .input = {.key = doom_key_f9, .mouse_button = -1, .joystick_button = -1},
                                     }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_input_default((input_default_t){
                                         .name = "input_endgame",
                                         .setup_screen = doom_misc_setup_screen_keys,
                                         .identifier = doom_dsda_input_end_game,
                                         .input = {.key = doom_key_f7, .mouse_button = -1, .joystick_button = -1},
                                     }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_input_default((input_default_t){
                                         .name = "input_quit",
                                         .setup_screen = doom_misc_setup_screen_keys,
                                         .identifier = doom_dsda_input_quit,
                                         .input = {.key = doom_key_f10, .mouse_button = -1, .joystick_button = -1},
                                     }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_map_follow",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_map_follow,
                                                    .input = {.key = 'f', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_map_zoomin",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_map_zoom_in,
                                                    .input = {.key = '=', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_map_zoomout",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_map_zoom_out,
                                                    .input = {.key = '-', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_input_default((input_default_t){
                                         .name = "input_map_up",
                                         .setup_screen = doom_misc_setup_screen_keys,
                                         .identifier = doom_dsda_input_map_up,
                                         .input = {.key = doom_key_up_arrow, .mouse_button = -1, .joystick_button = -1},
                                     }));
    doom_misc_default_dyarray_append(
        &defaults, s_input_default((input_default_t){
                       .name = "input_map_down",
                       .setup_screen = doom_misc_setup_screen_keys,
                       .identifier = doom_dsda_input_map_down,
                       .input = {.key = doom_key_down_arrow, .mouse_button = -1, .joystick_button = -1},
                   }));
    doom_misc_default_dyarray_append(
        &defaults, s_input_default((input_default_t){
                       .name = "input_map_left",
                       .setup_screen = doom_misc_setup_screen_keys,
                       .identifier = doom_dsda_input_map_left,
                       .input = {.key = doom_key_left_arrow, .mouse_button = -1, .joystick_button = -1},
                   }));
    doom_misc_default_dyarray_append(
        &defaults, s_input_default((input_default_t){
                       .name = "input_map_right",
                       .setup_screen = doom_misc_setup_screen_keys,
                       .identifier = doom_dsda_input_map_right,
                       .input = {.key = doom_key_right_arrow, .mouse_button = -1, .joystick_button = -1},
                   }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_map_mark",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_map_mark,
                                                    .input = {.key = 'm', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_map_clear",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_map_clear,
                                                    .input = {.key = 'c', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_map_gobig",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_map_go_big,
                                                    .input = {.key = '0', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_map_grid",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_map_grid,
                                                    .input = {.key = 'g', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_map_rotate",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_map_rotate,
                                                    .input = {.key = 'r', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_map_overlay",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_map_overlay,
                                                    .input = {.key = 'o', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_map_textured",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_map_textured,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_chat",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_chat,
                                                    .input = {.key = 't', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_chat_dest0",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_chat_dest0,
                                                    .input = {.key = 'g', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_chat_dest1",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_chat_dest1,
                                                    .input = {.key = 'i', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_chat_dest2",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_chat_dest2,
                                                    .input = {.key = 'b', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_chat_dest3",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_chat_dest3,
                                                    .input = {.key = 'r', .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(
        &defaults, s_input_default((input_default_t){
                       .name = "input_chat_backspace",
                       .setup_screen = doom_misc_setup_screen_keys,
                       .identifier = doom_dsda_input_chat_backspace,
                       .input = {.key = doom_key_backspace, .mouse_button = -1, .joystick_button = -1},
                   }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_input_default((input_default_t){
                                         .name = "input_chat_enter",
                                         .setup_screen = doom_misc_setup_screen_keys,
                                         .identifier = doom_dsda_input_chat_enter,
                                         .input = {.key = doom_key_enter, .mouse_button = -1, .joystick_button = -1},
                                     }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_speed_up",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_speed_up,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_speed_down",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_speed_down,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_speed_default",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_speed_default,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_input_default((input_default_t){
                                         .name = "input_demo_skip",
                                         .setup_screen = doom_misc_setup_screen_keys,
                                         .identifier = doom_dsda_input_demo_skip,
                                         .input = {.key = doom_key_insert, .mouse_button = -1, .joystick_button = -1},
                                     }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_input_default((input_default_t){
                                         .name = "input_demo_endlevel",
                                         .setup_screen = doom_misc_setup_screen_keys,
                                         .identifier = doom_dsda_input_demo_end_level,
                                         .input = {.key = doom_key_end, .mouse_button = -1, .joystick_button = -1},
                                     }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_input_default((input_default_t){
                                         .name = "input_walkcamera",
                                         .setup_screen = doom_misc_setup_screen_keys,
                                         .identifier = doom_dsda_input_walk_camera,
                                         .input = {.key = doom_key_keypad0, .mouse_button = -1, .joystick_button = -1},
                                     }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_join_demo",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_join_demo,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_input_default((input_default_t){
                                         .name = "input_restart",
                                         .setup_screen = doom_misc_setup_screen_keys,
                                         .identifier = doom_dsda_input_restart,
                                         .input = {.key = doom_key_home, .mouse_button = -1, .joystick_button = -1},
                                     }));
    doom_misc_default_dyarray_append(
        &defaults, s_input_default((input_default_t){
                       .name = "input_nextlevel",
                       .setup_screen = doom_misc_setup_screen_keys,
                       .identifier = doom_dsda_input_next_level,
                       .input = {.key = doom_key_page_down, .mouse_button = -1, .joystick_button = -1},
                   }));
    doom_misc_default_dyarray_append(
        &defaults, s_input_default((input_default_t){
                       .name = "input_showalive",
                       .setup_screen = doom_misc_setup_screen_keys,
                       .identifier = doom_dsda_input_show_alive,
                       .input = {.key = doom_key_keypad_divide, .mouse_button = -1, .joystick_button = -1},
                   }));
    doom_misc_default_dyarray_append(
        &defaults, s_input_default((input_default_t){
                       .name = "input_menu_down",
                       .setup_screen = doom_misc_setup_screen_keys,
                       .identifier = doom_dsda_input_menu_down,
                       .input = {.key = doom_key_down_arrow, .mouse_button = -1, .joystick_button = -1},
                   }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_input_default((input_default_t){
                                         .name = "input_menu_up",
                                         .setup_screen = doom_misc_setup_screen_keys,
                                         .identifier = doom_dsda_input_menu_up,
                                         .input = {.key = doom_key_up_arrow, .mouse_button = -1, .joystick_button = -1},
                                     }));
    doom_misc_default_dyarray_append(
        &defaults, s_input_default((input_default_t){
                       .name = "input_menu_left",
                       .setup_screen = doom_misc_setup_screen_keys,
                       .identifier = doom_dsda_input_menu_left,
                       .input = {.key = doom_key_left_arrow, .mouse_button = -1, .joystick_button = -1},
                   }));
    doom_misc_default_dyarray_append(
        &defaults, s_input_default((input_default_t){
                       .name = "input_menu_right",
                       .setup_screen = doom_misc_setup_screen_keys,
                       .identifier = doom_dsda_input_menu_right,
                       .input = {.key = doom_key_right_arrow, .mouse_button = -1, .joystick_button = -1},
                   }));
    doom_misc_default_dyarray_append(
        &defaults, s_input_default((input_default_t){
                       .name = "input_menu_backspace",
                       .setup_screen = doom_misc_setup_screen_keys,
                       .identifier = doom_dsda_input_menu_backspace,
                       .input = {.key = doom_key_backspace, .mouse_button = -1, .joystick_button = -1},
                   }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_input_default((input_default_t){
                                         .name = "input_menu_enter",
                                         .setup_screen = doom_misc_setup_screen_keys,
                                         .identifier = doom_dsda_input_menu_enter,
                                         .input = {.key = doom_key_enter, .mouse_button = -1, .joystick_button = -1},
                                     }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_input_default((input_default_t){
                                         .name = "input_menu_escape",
                                         .setup_screen = doom_misc_setup_screen_keys,
                                         .identifier = doom_dsda_input_menu_escape,
                                         .input = {.key = doom_key_escape, .mouse_button = -1, .joystick_button = -1},
                                     }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_input_default((input_default_t){
                                         .name = "input_menu_clear",
                                         .setup_screen = doom_misc_setup_screen_keys,
                                         .identifier = doom_dsda_input_menu_clear,
                                         .input = {.key = doom_key_delete, .mouse_button = -1, .joystick_button = -1},
                                     }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_iddqd",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_iddqd,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_idkfa",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_idkfa,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_idfa",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_idfa,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_idclip",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_idclip,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_idbeholdh",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_idbeholdh,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_idbeholdm",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_idbeholdm,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_idbeholdv",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_idbeholdv,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_idbeholds",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_idbeholds,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_idbeholdi",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_idbeholdi,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_idbeholdr",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_idbeholdr,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_idbeholda",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_idbeholda,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_idbeholdl",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_idbeholdl,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_idmypos",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_idmypos,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_idrate",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_idrate,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_iddt",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_iddt,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_ponce",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_ponce,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_shazam",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_shazam,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_chicken",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_chicken,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_lookup",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_look_up,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_lookdown",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_look_down,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_lookcenter",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_look_center,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_use_artifact",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_use_artifact,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_arti_tome",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_arti_tome,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_arti_quartz",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_arti_quartz,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_arti_urn",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_arti_urn,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_arti_bomb",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_arti_bomb,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_arti_ring",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_arti_ring,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_arti_chaosdevice",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_arti_chaos_device,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_arti_shadowsphere",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_arti_shadow_sphere,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_arti_wings",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_arti_wings,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_arti_torch",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_arti_torch,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_arti_morph",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_arti_morph,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_invleft",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_inventory_left,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_invright",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_inventory_right,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_store_quick_key_frame",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_store_quick_key_frame,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_restore_quick_key_frame",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_restore_quick_key_frame,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_rewind",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_rewind,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_cycle_profile",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_cycle_profile,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_cycle_palette",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_cycle_palette,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_command_display",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_command_display,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_strict_mode",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_strict_mode,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_console",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_console,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_coordinate_display",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_coordinate_display,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_avj",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_avj,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_exhud",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_exhud,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_mute_sfx",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_mute_sfx,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_mute_music",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_mute_music,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_cheat_codes",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_cheat_codes,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_notarget",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_notarget,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_jump",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_jump,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_hexen_arti_incant",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_hexen_arti_incant,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_hexen_arti_summon",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_hexen_arti_summon,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_hexen_arti_disk",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_hexen_arti_disk,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_hexen_arti_flechette",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_hexen_arti_flechette,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_hexen_arti_banishment",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_hexen_arti_banishment,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_hexen_arti_boots",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_hexen_arti_boots,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_hexen_arti_krater",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_hexen_arti_krater,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_input_default((input_default_t){
                                                    .name = "input_hexen_arti_bracers",
                                                    .setup_screen = doom_misc_setup_screen_keys,
                                                    .identifier = doom_dsda_input_hexen_arti_bracers,
                                                    .input = {.key = 0, .mouse_button = -1, .joystick_button = -1},
                                                }));

    doom_misc_default_dyarray_append(&defaults, s_header_default("Mouse settings"));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "use_mouse",
                                                    .location = &doom_state->defaults_storage.use_mouse,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_integer_default((integer_default_t){
                                         .name = "mouse_sensitivity_horiz",
                                         .location = &doom_state->defaults_storage.mouse_sensitivity_horizontal,
                                         .default_value = 10,
                                         .min_value = 0,
                                         .max_value = max_unset,
                                         .setup_screen = doom_misc_setup_screen_none,
                                     }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_integer_default((integer_default_t){
                                         .name = "mouse_sensitivity_vert",
                                         .location = &doom_state->defaults_storage.mouse_sensitivity_vertical,
                                         .default_value = 1,
                                         .min_value = 0,
                                         .max_value = max_unset,
                                         .setup_screen = doom_misc_setup_screen_none,
                                     }));

    doom_misc_default_dyarray_append(&defaults, s_header_default("Joystick settings"));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "use_joystick",
                                                    .location = &doom_state->defaults_storage.use_joystick,
                                                    .default_value = 0,
                                                    .min_value = 0,
                                                    .max_value = 2,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));

    doom_misc_default_dyarray_append(&defaults, s_header_default("Chat macros"));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "chatmacro0",
                                                    .location = &doom_state->defaults_storage.chat_macros[0],
                                                    .default_value = DOOM_HUD_STRING_CHAT_MACRO_0,
                                                    .setup_screen = doom_misc_setup_screen_chat,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "chatmacro1",
                                                    .location = &doom_state->defaults_storage.chat_macros[1],
                                                    .default_value = DOOM_HUD_STRING_CHAT_MACRO_1,
                                                    .setup_screen = doom_misc_setup_screen_chat,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "chatmacro2",
                                                    .location = &doom_state->defaults_storage.chat_macros[2],
                                                    .default_value = DOOM_HUD_STRING_CHAT_MACRO_2,
                                                    .setup_screen = doom_misc_setup_screen_chat,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "chatmacro3",
                                                    .location = &doom_state->defaults_storage.chat_macros[3],
                                                    .default_value = DOOM_HUD_STRING_CHAT_MACRO_3,
                                                    .setup_screen = doom_misc_setup_screen_chat,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "chatmacro4",
                                                    .location = &doom_state->defaults_storage.chat_macros[4],
                                                    .default_value = DOOM_HUD_STRING_CHAT_MACRO_4,
                                                    .setup_screen = doom_misc_setup_screen_chat,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "chatmacro5",
                                                    .location = &doom_state->defaults_storage.chat_macros[5],
                                                    .default_value = DOOM_HUD_STRING_CHAT_MACRO_5,
                                                    .setup_screen = doom_misc_setup_screen_chat,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "chatmacro6",
                                                    .location = &doom_state->defaults_storage.chat_macros[6],
                                                    .default_value = DOOM_HUD_STRING_CHAT_MACRO_6,
                                                    .setup_screen = doom_misc_setup_screen_chat,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "chatmacro7",
                                                    .location = &doom_state->defaults_storage.chat_macros[7],
                                                    .default_value = DOOM_HUD_STRING_CHAT_MACRO_7,
                                                    .setup_screen = doom_misc_setup_screen_chat,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "chatmacro8",
                                                    .location = &doom_state->defaults_storage.chat_macros[8],
                                                    .default_value = DOOM_HUD_STRING_CHAT_MACRO_8,
                                                    .setup_screen = doom_misc_setup_screen_chat,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "chatmacro9",
                                                    .location = &doom_state->defaults_storage.chat_macros[9],
                                                    .default_value = DOOM_HUD_STRING_CHAT_MACRO_9,
                                                    .setup_screen = doom_misc_setup_screen_chat,
                                                }));

    doom_misc_default_dyarray_append(&defaults, s_header_default("Automap settings"));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mapcolor_back",
                                                    .location = &doom_state->defaults_storage.mapcolor_back,
                                                    .default_value = 247,
                                                    .min_value = 0,
                                                    .max_value = 255,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mapcolor_grid",
                                                    .location = &doom_state->defaults_storage.mapcolor_grid,
                                                    .default_value = 104,
                                                    .min_value = 0,
                                                    .max_value = 255,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mapcolor_wall",
                                                    .location = &doom_state->defaults_storage.mapcolor_wall,
                                                    .default_value = 23,
                                                    .min_value = 0,
                                                    .max_value = 255,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mapcolor_fchg",
                                                    .location = &doom_state->defaults_storage.mapcolor_fchg,
                                                    .default_value = 55,
                                                    .min_value = 0,
                                                    .max_value = 255,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mapcolor_cchg",
                                                    .location = &doom_state->defaults_storage.mapcolor_cchg,
                                                    .default_value = 215,
                                                    .min_value = 0,
                                                    .max_value = 255,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mapcolor_clsd",
                                                    .location = &doom_state->defaults_storage.mapcolor_clsd,
                                                    .default_value = 208,
                                                    .min_value = 0,
                                                    .max_value = 255,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mapcolor_rkey",
                                                    .location = &doom_state->defaults_storage.mapcolor_rkey,
                                                    .default_value = 175,
                                                    .min_value = 0,
                                                    .max_value = 255,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mapcolor_bkey",
                                                    .location = &doom_state->defaults_storage.mapcolor_bkey,
                                                    .default_value = 204,
                                                    .min_value = 0,
                                                    .max_value = 255,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mapcolor_ykey",
                                                    .location = &doom_state->defaults_storage.mapcolor_ykey,
                                                    .default_value = 231,
                                                    .min_value = 0,
                                                    .max_value = 255,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mapcolor_rdor",
                                                    .location = &doom_state->defaults_storage.mapcolor_rdor,
                                                    .default_value = 175,
                                                    .min_value = 0,
                                                    .max_value = 255,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mapcolor_bdor",
                                                    .location = &doom_state->defaults_storage.mapcolor_bdor,
                                                    .default_value = 204,
                                                    .min_value = 0,
                                                    .max_value = 255,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mapcolor_ydor",
                                                    .location = &doom_state->defaults_storage.mapcolor_ydor,
                                                    .default_value = 231,
                                                    .min_value = 0,
                                                    .max_value = 255,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mapcolor_tele",
                                                    .location = &doom_state->defaults_storage.mapcolor_tele,
                                                    .default_value = 119,
                                                    .min_value = 0,
                                                    .max_value = 255,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mapcolor_secr",
                                                    .location = &doom_state->defaults_storage.mapcolor_secr,
                                                    .default_value = 252,
                                                    .min_value = 0,
                                                    .max_value = 255,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mapcolor_revsecr",
                                                    .location = &doom_state->defaults_storage.mapcolor_revsecr,
                                                    .default_value = 112,
                                                    .min_value = 0,
                                                    .max_value = 255,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mapcolor_exit",
                                                    .location = &doom_state->defaults_storage.mapcolor_exit,
                                                    .default_value = 0,
                                                    .min_value = 0,
                                                    .max_value = 255,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mapcolor_unsn",
                                                    .location = &doom_state->defaults_storage.mapcolor_unsn,
                                                    .default_value = 104,
                                                    .min_value = 0,
                                                    .max_value = 255,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mapcolor_flat",
                                                    .location = &doom_state->defaults_storage.mapcolor_flat,
                                                    .default_value = 88,
                                                    .min_value = 0,
                                                    .max_value = 255,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mapcolor_sprt",
                                                    .location = &doom_state->defaults_storage.mapcolor_sprt,
                                                    .default_value = 112,
                                                    .min_value = 0,
                                                    .max_value = 255,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mapcolor_item",
                                                    .location = &doom_state->defaults_storage.mapcolor_item,
                                                    .default_value = 231,
                                                    .min_value = 0,
                                                    .max_value = 255,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mapcolor_hair",
                                                    .location = &doom_state->defaults_storage.mapcolor_hair,
                                                    .default_value = 208,
                                                    .min_value = 0,
                                                    .max_value = 255,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mapcolor_sngl",
                                                    .location = &doom_state->defaults_storage.mapcolor_sngl,
                                                    .default_value = 208,
                                                    .min_value = 0,
                                                    .max_value = 255,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mapcolor_me",
                                                    .location = &doom_state->defaults_storage.mapcolor_me,
                                                    .default_value = 112,
                                                    .min_value = 0,
                                                    .max_value = 255,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mapcolor_enemy",
                                                    .location = &doom_state->defaults_storage.mapcolor_enemy,
                                                    .default_value = 177,
                                                    .min_value = 0,
                                                    .max_value = 255,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mapcolor_frnd",
                                                    .location = &doom_state->defaults_storage.mapcolor_frnd,
                                                    .default_value = 112,
                                                    .min_value = 0,
                                                    .max_value = 255,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "map_secret_after",
                                                    .location = &doom_state->defaults_storage.map_secret_after,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "map_point_coord",
                                                    .location = &doom_state->defaults_storage.map_point_coord,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "map_level_stat",
                                                    .location = &doom_state->defaults_storage.map_level_stat,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_hexint_default((hexint_default_t){
                                                    .name = "automapmode",
                                                    .location = (uint32_t*)&doom_state->defaults_storage.automapmode,
                                                    .default_value = doom_automap_mode_follow,
                                                    .min_value = 0,
                                                    .max_value = 31,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "map_always_updates",
                                                    .location = &doom_state->defaults_storage.map_always_updates,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "map_grid_size",
                                                    .location = &doom_state->defaults_storage.map_grid_size,
                                                    .default_value = 128,
                                                    .min_value = 8,
                                                    .max_value = 256,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "map_scroll_speed",
                                                    .location = &doom_state->defaults_storage.map_scroll_speed,
                                                    .default_value = 8,
                                                    .min_value = 1,
                                                    .max_value = 32,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "map_wheel_zoom",
                                                    .location = &doom_state->defaults_storage.map_wheel_zoom,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "map_use_multisampling",
                                                    .location = &doom_state->defaults_storage.map_use_multisampling,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "map_textured",
                                                    .location = &doom_state->defaults_storage.map_textured,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "map_textured_trans",
                                                    .location = &doom_state->defaults_storage.map_textured_translucency,
                                                    .default_value = 100,
                                                    .min_value = 0,
                                                    .max_value = 100,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_integer_default((integer_default_t){
                                         .name = "map_textured_overlay_trans",
                                         .location = &doom_state->defaults_storage.map_textured_overlay_translucency,
                                         .default_value = 66,
                                         .min_value = 0,
                                         .max_value = 100,
                                         .setup_screen = doom_misc_setup_screen_auto,
                                     }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_integer_default((integer_default_t){
                                         .name = "map_lines_overlay_trans",
                                         .location = &doom_state->defaults_storage.map_lines_overlay_translucency,
                                         .default_value = 100,
                                         .min_value = 0,
                                         .max_value = 100,
                                         .setup_screen = doom_misc_setup_screen_auto,
                                     }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "map_overlay_pos_x",
                                                    .location = &doom_state->defaults_storage.map_overlay_pos_x,
                                                    .default_value = 0,
                                                    .min_value = 0,
                                                    .max_value = 319,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "map_overlay_pos_y",
                                                    .location = &doom_state->defaults_storage.map_overlay_pos_y,
                                                    .default_value = 0,
                                                    .min_value = 0,
                                                    .max_value = 199,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "map_overlay_pos_width",
                                                    .location = &doom_state->defaults_storage.map_overlay_pos_width,
                                                    .default_value = 320,
                                                    .min_value = 0,
                                                    .max_value = 320,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "map_overlay_pos_height",
                                                    .location = &doom_state->defaults_storage.map_overlay_pos_height,
                                                    .default_value = 200,
                                                    .min_value = 0,
                                                    .max_value = 200,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_integer_default((integer_default_t){
                                         .name = "map_things_appearance",
                                         .location = (int*)&doom_state->defaults_storage.map_things_appearance,
                                         .default_value = doom_automap_things_appearance_count - 1,
                                         .min_value = 0,
                                         .max_value = doom_automap_things_appearance_count - 1,
                                         .setup_screen = doom_misc_setup_screen_auto,
                                     }));

    doom_misc_default_dyarray_append(&defaults, s_header_default("Heads-up display settings"));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "hudcolor_titl",
                                                    .location = &doom_state->defaults_storage.hudcolor_titl,
                                                    .default_value = 5,
                                                    .min_value = 0,
                                                    .max_value = 9,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "hudcolor_xyco",
                                                    .location = &doom_state->defaults_storage.hudcolor_xyco,
                                                    .default_value = 3,
                                                    .min_value = 0,
                                                    .max_value = 9,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "hudcolor_mapstat_title",
                                                    .location = &doom_state->defaults_storage.hudcolor_mapstat_title,
                                                    .default_value = 6,
                                                    .min_value = 0,
                                                    .max_value = 9,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "hudcolor_mapstat_value",
                                                    .location = &doom_state->defaults_storage.hudcolor_mapstat_value,
                                                    .default_value = 2,
                                                    .min_value = 0,
                                                    .max_value = 9,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "hudcolor_mapstat_time",
                                                    .location = &doom_state->defaults_storage.hudcolor_mapstat_time,
                                                    .default_value = 2,
                                                    .min_value = 0,
                                                    .max_value = 9,
                                                    .setup_screen = doom_misc_setup_screen_auto,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "hudcolor_mesg",
                                                    .location = &doom_state->defaults_storage.hudcolor_mesg,
                                                    .default_value = 6,
                                                    .min_value = 0,
                                                    .max_value = 9,
                                                    .setup_screen = doom_misc_setup_screen_messages,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "hudcolor_chat",
                                                    .location = &doom_state->defaults_storage.hudcolor_chat,
                                                    .default_value = 5,
                                                    .min_value = 0,
                                                    .max_value = 9,
                                                    .setup_screen = doom_misc_setup_screen_messages,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "hudcolor_list",
                                                    .location = &doom_state->defaults_storage.hudcolor_list,
                                                    .default_value = 5,
                                                    .min_value = 0,
                                                    .max_value = 9,
                                                    .setup_screen = doom_misc_setup_screen_messages,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "hud_msg_lines",
                                                    .location = &doom_state->defaults_storage.hud_msg_lines,
                                                    .default_value = 1,
                                                    .min_value = 1,
                                                    .max_value = 16,
                                                    .setup_screen = doom_misc_setup_screen_messages,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "hud_list_bgon",
                                                    .location = &doom_state->defaults_storage.hud_list_bgon,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_messages,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "health_red",
                                                    .location = &doom_state->defaults_storage.health_red,
                                                    .default_value = 25,
                                                    .min_value = 0,
                                                    .max_value = 200,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "health_yellow",
                                                    .location = &doom_state->defaults_storage.health_yellow,
                                                    .default_value = 50,
                                                    .min_value = 0,
                                                    .max_value = 200,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "health_green",
                                                    .location = &doom_state->defaults_storage.health_green,
                                                    .default_value = 100,
                                                    .min_value = 0,
                                                    .max_value = 200,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "armor_red",
                                                    .location = &doom_state->defaults_storage.armor_red,
                                                    .default_value = 25,
                                                    .min_value = 0,
                                                    .max_value = 200,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "armor_yellow",
                                                    .location = &doom_state->defaults_storage.armor_yellow,
                                                    .default_value = 50,
                                                    .min_value = 0,
                                                    .max_value = 200,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "armor_green",
                                                    .location = &doom_state->defaults_storage.armor_green,
                                                    .default_value = 100,
                                                    .min_value = 0,
                                                    .max_value = 200,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "ammo_red",
                                                    .location = &doom_state->defaults_storage.ammo_red,
                                                    .default_value = 25,
                                                    .min_value = 0,
                                                    .max_value = 100,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "ammo_yellow",
                                                    .location = &doom_state->defaults_storage.ammo_yellow,
                                                    .default_value = 50,
                                                    .min_value = 0,
                                                    .max_value = 100,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "ammo_color_behavior",
                                                    .location = (int*)&doom_state->defaults_storage.ammo_color_behavior,
                                                    .default_value = doom_status_bar_ammo_color_behavior_count - 1,
                                                    .min_value = 0,
                                                    .max_value = doom_status_bar_ammo_color_behavior_count - 1,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "hud_num",
                                                    .location = &doom_state->defaults_storage.hud_num,
                                                    .default_value = 6,
                                                    .min_value = 0,
                                                    .max_value = 100,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "hud_displayed",
                                                    .location = &doom_state->defaults_storage.hud_displayed,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));

    doom_misc_default_dyarray_append(&defaults, s_header_default("PrBoom+ heads-up display settings"));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "hudadd_gamespeed",
                                                    .location = &doom_state->defaults_storage.hudadd_gamespeed,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "hudadd_leveltime",
                                                    .location = &doom_state->defaults_storage.hudadd_leveltime,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "hudadd_demotime",
                                                    .location = &doom_state->defaults_storage.hudadd_demotime,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "hudadd_secretarea",
                                                    .location = &doom_state->defaults_storage.hudadd_secretarea,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "hudadd_maxtotals",
                                                    .location = &doom_state->defaults_storage.hudadd_maxtotals,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "hudadd_demoprogressbar",
                                                    .location = &doom_state->defaults_storage.hudadd_demoprogressbar,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "hudadd_crosshair",
                                                    .location = &doom_state->defaults_storage.hudadd_crosshair,
                                                    .default_value = 0,
                                                    .min_value = 0,
                                                    .max_value = doom_hud_crosshair_count - 1,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "hudadd_crosshair_scale",
                                                    .location = &doom_state->defaults_storage.hudadd_crosshair_scale,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "hudadd_crosshair_color",
                                                    .location = &doom_state->defaults_storage.hudadd_crosshair_color,
                                                    .default_value = 3,
                                                    .min_value = 0,
                                                    .max_value = 9,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "hudadd_crosshair_health",
                                                    .location = &doom_state->defaults_storage.hudadd_crosshair_health,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "hudadd_crosshair_target",
                                                    .location = &doom_state->defaults_storage.hudadd_crosshair_target,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_integer_default((integer_default_t){
                                         .name = "hudadd_crosshair_target_color",
                                         .location = &doom_state->defaults_storage.hudadd_crosshair_target_color,
                                         .default_value = 9,
                                         .min_value = 0,
                                         .max_value = 9,
                                         .setup_screen = doom_misc_setup_screen_status_bar,
                                     }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_boolean_default((boolean_default_t){
                                         .name = "hudadd_crosshair_lock_target",
                                         .location = &doom_state->defaults_storage.hudadd_crosshair_lock_target,
                                         .default_value = false,
                                         .setup_screen = doom_misc_setup_screen_status_bar,
                                     }));

    doom_misc_default_dyarray_append(&defaults, s_header_default("PrBoom+ mouse settings"));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mouse_acceleration",
                                                    .location = &doom_state->defaults_storage.mouse_acceleration,
                                                    .default_value = 0,
                                                    .min_value = 0,
                                                    .max_value = max_unset,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "mouse_sensitivity_mlook",
                                                    .location = &doom_state->defaults_storage.mouse_sensitivity_mlook,
                                                    .default_value = 10,
                                                    .min_value = 0,
                                                    .max_value = max_unset,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "mouse_doubleclick_as_use",
                                                    .location = &doom_state->defaults_storage.mouse_doubleclick_as_use,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "mouse_carrytics",
                                                    .location = &doom_state->defaults_storage.mouse_carrytics,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));

    doom_misc_default_dyarray_append(&defaults, s_header_default("PrBoom+ demo settings"));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "demo_demoex_filename",
                                                    .location = &doom_state->defaults_storage.demo_demoex_filename,
                                                    .default_value = phyto_string_span_empty(),
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "getwad_cmdline",
                                                    .location = &doom_state->defaults_storage.getwad_cmdline,
                                                    .default_value = phyto_string_span_empty(),
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "demo_overwriteexisting",
                                                    .location = &doom_state->defaults_storage.demo_overwriteexisting,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "quickstart_window_ms",
                                                    .location = &doom_state->defaults_storage.quickstart_window_ms,
                                                    .default_value = 0,
                                                    .min_value = 0,
                                                    .max_value = 1000,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));

    doom_misc_default_dyarray_append(&defaults, s_header_default("PrBoom+ game settings"));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "movement_strafe50",
                                                    .location = &doom_state->defaults_storage.movement_strafe50,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "movement_strafe50onturns",
                                                    .location = &doom_state->defaults_storage.movement_strafe50onturns,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "movement_shorttics",
                                                    .location = &doom_state->defaults_storage.movement_shorttics,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "interpolation_maxobjects",
                                                    .location = &doom_state->defaults_storage.interpolation_maxobjects,
                                                    .default_value = 0,
                                                    .min_value = 0,
                                                    .max_value = max_unset,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "speed_step",
                                                    .location = &doom_state->defaults_storage.speed_step,
                                                    .default_value = 0,
                                                    .min_value = 0,
                                                    .max_value = 1000,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));

    doom_misc_default_dyarray_append(&defaults, s_header_default("PrBoom+ miscellaneous settings"));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "showendoom",
                                                    .location = &doom_state->defaults_storage.showendoom,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "screenshot_dir",
                                                    .location = &doom_state->defaults_storage.screenshot_dir,
                                                    .default_value = phyto_string_span_empty(),
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "health_bar",
                                                    .location = &doom_state->defaults_storage.health_bar,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "health_bar_full_length",
                                                    .location = &doom_state->defaults_storage.health_bar_full_length,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "health_bar_red",
                                                    .location = &doom_state->defaults_storage.health_bar_red,
                                                    .default_value = 50,
                                                    .min_value = 0,
                                                    .max_value = 100,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "health_bar_yellow",
                                                    .location = &doom_state->defaults_storage.health_bar_yellow,
                                                    .default_value = 99,
                                                    .min_value = 0,
                                                    .max_value = 100,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "health_bar_green",
                                                    .location = &doom_state->defaults_storage.health_bar_green,
                                                    .default_value = 0,
                                                    .min_value = 0,
                                                    .max_value = 100,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));

    doom_misc_default_dyarray_append(&defaults, s_header_default("DSDA-Doom settings"));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "dsda_strict_mode",
                                                    .location = &doom_state->defaults_storage.dsda.strict_mode,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "dsda_cycle_ghost_colors",
                                                    .location = &doom_state->defaults_storage.dsda.cycle_ghost_colors,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_integer_default((integer_default_t){
                                         .name = "dsda_auto_key_frame_interval",
                                         .location = &doom_state->defaults_storage.dsda.auto_key_frame_interval,
                                         .default_value = 1,
                                         .min_value = 1,
                                         .max_value = 600,
                                         .setup_screen = doom_misc_setup_screen_status_bar,
                                     }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "dsda_auto_key_frame_depth",
                                                    .location = &doom_state->defaults_storage.dsda.auto_key_frame_depth,
                                                    .default_value = 60,
                                                    .min_value = 1,
                                                    .max_value = 600,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_integer_default((integer_default_t){
                                         .name = "dsda_auto_key_frame_timeout",
                                         .location = &doom_state->defaults_storage.dsda.auto_key_frame_timeout,
                                         .default_value = 10,
                                         .min_value = 0,
                                         .max_value = 25,
                                         .setup_screen = doom_misc_setup_screen_status_bar,
                                     }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "dsda_exhud",
                                                    .location = &doom_state->defaults_storage.dsda.exhud,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "dsda_wipe_at_full_speed",
                                                    .location = &doom_state->defaults_storage.dsda.wipe_at_full_speed,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "dsda_show_demo_attempts",
                                                    .location = &doom_state->defaults_storage.dsda.show_demo_attempts,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "dsda_fine_sensitivity",
                                                    .location = &doom_state->defaults_storage.dsda.fine_sensitivity,
                                                    .default_value = 0,
                                                    .min_value = 0,
                                                    .max_value = 99,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "dsda_hide_horns",
                                                    .location = &doom_state->defaults_storage.dsda.hide_horns,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "dsda_organized_saves",
                                                    .location = &doom_state->defaults_storage.dsda.organized_saves,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "dsda_command_display",
                                                    .location = &doom_state->defaults_storage.dsda.command_display,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "dsda_command_history_size",
                                                    .location = &doom_state->defaults_storage.dsda.command_history_size,
                                                    .default_value = 10,
                                                    .min_value = 1,
                                                    .max_value = 20,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "dsda_hide_empty_commands",
                                                    .location = &doom_state->defaults_storage.dsda.hide_empty_commands,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "dsda_coordinate_display",
                                                    .location = &doom_state->defaults_storage.dsda.coordinate_display,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "dsda_skip_quit_prompt",
                                                    .location = &doom_state->defaults_storage.dsda.skip_quit_prompt,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "dsda_show_split_data",
                                                    .location = &doom_state->defaults_storage.dsda.show_split_data,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "dsda_player_name",
                                                    .location = &doom_state->defaults_storage.dsda.player_name,
                                                    .default_value = phyto_string_span_from_c("Anonymous"),
                                                    .setup_screen = doom_misc_setup_screen_chat,
                                                }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_integer_default((integer_default_t){
                                         .name = "dsda_quickstart_cache_tics",
                                         .location = &doom_state->defaults_storage.dsda.quickstart_cache_tics,
                                         .default_value = 0,
                                         .min_value = 0,
                                         .max_value = 35,
                                         .setup_screen = doom_misc_setup_screen_status_bar,
                                     }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "dsda_death_use_action",
                                                    .location = &doom_state->defaults_storage.dsda.death_use_action,
                                                    .default_value = 0,
                                                    .min_value = 0,
                                                    .max_value = 2,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "dsda_mute_sfx",
                                                    .location = &doom_state->defaults_storage.dsda.mute_sfx,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "dsda_mute_music",
                                                    .location = &doom_state->defaults_storage.dsda.mute_music,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "dsda_cheat_codes",
                                                    .location = &doom_state->defaults_storage.dsda.cheat_codes,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "dsda_allow_jumping",
                                                    .location = &doom_state->defaults_storage.dsda.allow_jumping,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "dsda_parallel_sfx_limit",
                                                    .location = &doom_state->defaults_storage.dsda.parallel_sfx_limit,
                                                    .default_value = 0,
                                                    .min_value = 0,
                                                    .max_value = 32,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "dsda_parallel_sfx_window",
                                                    .location = &doom_state->defaults_storage.dsda.parallel_sfx_window,
                                                    .default_value = 1,
                                                    .min_value = 1,
                                                    .max_value = 32,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_boolean_default((boolean_default_t){
                                         .name = "dsda_switch_when_ammo_runs_out",
                                         .location = &doom_state->defaults_storage.dsda.switch_when_ammo_runs_out,
                                         .default_value = true,
                                         .setup_screen = doom_misc_setup_screen_status_bar,
                                     }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "dsda_viewbob",
                                                    .location = &doom_state->defaults_storage.dsda.viewbob,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "dsda_weaponbob",
                                                    .location = &doom_state->defaults_storage.dsda.weaponbob,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));

    doom_misc_default_dyarray_append(&defaults, s_header_default("Video capture encoding settings"));
    doom_misc_default_dyarray_append(
        &defaults,
        s_string_default((string_default_t){
            .name = "cap_soundcommand",
            .location = &doom_state->defaults_storage.cap_soundcommand,
            .default_value = phyto_string_span_from_c("ffmpeg -f s16le -ar %s -ac 2 -i - -c:a libopus -y temp_a.nut"),
            .setup_screen = doom_misc_setup_screen_none,
        }));
    doom_misc_default_dyarray_append(
        &defaults, s_string_default((string_default_t){
                       .name = "cap_videocommand",
                       .location = &doom_state->defaults_storage.cap_videocommand,
                       .default_value = phyto_string_span_from_c(
                           "ffmpeg -f rawvideo -pix_fmt rgb24 -r %r -s %wx%h -i - -c:v libx264 -y temp_v.nut"),
                       .setup_screen = doom_misc_setup_screen_none,
                   }));
    doom_misc_default_dyarray_append(
        &defaults, s_string_default((string_default_t){
                       .name = "cap_muxcommand",
                       .location = &doom_state->defaults_storage.cap_muxcommand,
                       .default_value = phyto_string_span_from_c("ffmpeg -i temp_v.nut -i temp_a.nut -c copy -y %f"),
                       .setup_screen = doom_misc_setup_screen_none,
                   }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "cap_tempfile1",
                                                    .location = &doom_state->defaults_storage.cap_tempfile1,
                                                    .default_value = phyto_string_span_from_c("temp_a.nut"),
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "cap_tempfile2",
                                                    .location = &doom_state->defaults_storage.cap_tempfile2,
                                                    .default_value = phyto_string_span_from_c("temp_v.nut"),
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "cap_remove_tempfiles",
                                                    .location = &doom_state->defaults_storage.cap_remove_tempfiles,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "cap_fps",
                                                    .location = &doom_state->defaults_storage.cap_fps,
                                                    .default_value = 60,
                                                    .min_value = 16,
                                                    .max_value = 300,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));

    doom_misc_default_dyarray_append(&defaults, s_header_default("PrBoom+ video settings"));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "sdl_video_window_pos",
                                                    .location = &doom_state->defaults_storage.sdl_video_window_pos,
                                                    .default_value = phyto_string_span_from_c("center"),
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "palette_ondamage",
                                                    .location = &doom_state->defaults_storage.palette_ondamage,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "palette_onbonus",
                                                    .location = &doom_state->defaults_storage.palette_onbonus,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "palette_onpowers",
                                                    .location = &doom_state->defaults_storage.palette_onpowers,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "render_wipescreen",
                                                    .location = &doom_state->defaults_storage.render_wipescreen,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "render_screen_multiply",
                                                    .location = &doom_state->defaults_storage.render_screen_multiply,
                                                    .default_value = 1,
                                                    .min_value = 1,
                                                    .max_value = 5,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "integer_scaling",
                                                    .location = &doom_state->defaults_storage.integer_scaling,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "render_aspect",
                                                    .location = &doom_state->defaults_storage.render_aspect,
                                                    .default_value = 0,
                                                    .min_value = 0,
                                                    .max_value = 4,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "render_doom_lightmaps",
                                                    .location = &doom_state->defaults_storage.render_doom_lightmaps,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "fake_contrast",
                                                    .location = &doom_state->defaults_storage.fake_contrast,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "render_stretch_hud",
                                                    .location = (int*)&doom_state->defaults_storage.render_stretch_hud,
                                                    .default_value = doom_video_patch_stretch_16x10,
                                                    .min_value = 0,
                                                    .max_value = doom_video_patch_stretch_count - 1,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "render_patches_scalex",
                                                    .location = &doom_state->defaults_storage.render_patches_scalex,
                                                    .default_value = 0,
                                                    .min_value = 0,
                                                    .max_value = 16,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "render_patches_scaley",
                                                    .location = &doom_state->defaults_storage.render_patches_scaley,
                                                    .default_value = 0,
                                                    .min_value = 0,
                                                    .max_value = 16,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "render_stretchsky",
                                                    .location = &doom_state->defaults_storage.render_stretchsky,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "sprites_doom_order",
                                                    .location = (int*)&doom_state->defaults_storage.sprites_doom_order,
                                                    .default_value = doom_render_things_sprite_order_static_order,
                                                    .min_value = 0,
                                                    .max_value = doom_render_things_sprite_order_count - 1,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "movement_mouselook",
                                                    .location = &doom_state->defaults_storage.movement_mouselook,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "movement_mousenovert",
                                                    .location = &doom_state->defaults_storage.movement_mousenovert,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "movement_maxviewpitch",
                                                    .location = &doom_state->defaults_storage.movement_maxviewpitch,
                                                    .default_value = 90,
                                                    .min_value = 0,
                                                    .max_value = 90,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_integer_default((integer_default_t){
                                         .name = "movement_mousestrafedivisor",
                                         .location = &doom_state->defaults_storage.movement_mousestrafedivisor,
                                         .default_value = 4,
                                         .min_value = 1,
                                         .max_value = 512,
                                         .setup_screen = doom_misc_setup_screen_status_bar,
                                     }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "movement_mouseinvert",
                                                    .location = &doom_state->defaults_storage.movement_mouseinvert,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));

    doom_misc_default_dyarray_append(&defaults, s_header_default("PrBoom+ OpenGL settings"));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "gl_allow_detail_textures",
                                                    .location = &doom_state->defaults_storage.gl_allow_detail_textures,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "gl_detail_maxdist",
                                                    .location = &doom_state->defaults_storage.gl_detail_maxdist,
                                                    .default_value = 0,
                                                    .min_value = 0,
                                                    .max_value = 65535,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "render_multisampling",
                                                    .location = &doom_state->defaults_storage.render_multisampling,
                                                    .default_value = 0,
                                                    .min_value = 0,
                                                    .max_value = 8,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "render_fov",
                                                    .location = &doom_state->defaults_storage.render_fov,
                                                    .default_value = 90,
                                                    .min_value = 20,
                                                    .max_value = 160,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "gl_spriteclip",
                                                    .location = (int*)&doom_state->defaults_storage.gl_spriteclip,
                                                    .default_value = doom_gl_struct_spriteclipmode_smart,
                                                    .min_value = 0,
                                                    .max_value = doom_gl_struct_spriteclipmode_smart,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "gl_spriteclip_threshold",
                                                    .location = &doom_state->defaults_storage.gl_spriteclip_threshold,
                                                    .default_value = 10,
                                                    .min_value = 0,
                                                    .max_value = 100,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_boolean_default((boolean_default_t){
                                         .name = "gl_sprites_frustum_culling",
                                         .location = &doom_state->defaults_storage.gl_sprites_frustum_culling,
                                         .default_value = true,
                                         .setup_screen = doom_misc_setup_screen_status_bar,
                                     }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "render_paperitems",
                                                    .location = &doom_state->defaults_storage.render_paperitems,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "gl_boom_colormaps",
                                                    .location = &doom_state->defaults_storage.gl_boom_colormaps,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "gl_hires_24bit_colormap",
                                                    .location = &doom_state->defaults_storage.gl_hires_24bit_colormap,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "gl_texture_internal_hires",
                                                    .location = &doom_state->defaults_storage.gl_texture_internal_hires,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "gl_texture_external_hires",
                                                    .location = &doom_state->defaults_storage.gl_texture_external_hires,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "gl_hires_override_pwads",
                                                    .location = &doom_state->defaults_storage.gl_hires_override_pwads,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "gl_texture_hires_dir",
                                                    .location = &doom_state->defaults_storage.gl_texture_hires_dir,
                                                    .default_value = phyto_string_span_empty(),
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "gl_texture_hqresize",
                                                    .location = &doom_state->defaults_storage.gl_texture_hqresize,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_integer_default((integer_default_t){
                                         .name = "gl_texture_hqresize_textures",
                                         .location = (int*)&doom_state->defaults_storage.gl_texture_hqresize_textures,
                                         .default_value = doom_gl_struct_hqresizemode_on_2x,
                                         .min_value = 0,
                                         .max_value = doom_gl_struct_hqresizemode_count - 1,
                                         .setup_screen = doom_misc_setup_screen_status_bar,
                                     }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_integer_default((integer_default_t){
                                         .name = "gl_texture_hqresize_sprites",
                                         .location = (int*)&doom_state->defaults_storage.gl_texture_hqresize_sprites,
                                         .default_value = doom_gl_struct_hqresizemode_none,
                                         .min_value = 0,
                                         .max_value = doom_gl_struct_hqresizemode_count - 1,
                                         .setup_screen = doom_misc_setup_screen_status_bar,
                                     }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_integer_default((integer_default_t){
                                         .name = "gl_texture_hqresize_patches",
                                         .location = (int*)&doom_state->defaults_storage.gl_texture_hqresize_patches,
                                         .default_value = doom_gl_struct_hqresizemode_on_2x,
                                         .min_value = 0,
                                         .max_value = doom_gl_struct_hqresizemode_count - 1,
                                         .setup_screen = doom_misc_setup_screen_status_bar,
                                     }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "gl_motionblur",
                                                    .location = &doom_state->defaults_storage.gl_motionblur,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "gl_motionblur_min_speed",
                                                    .location = &doom_state->defaults_storage.gl_motionblur_min_speed,
                                                    .default_value = phyto_string_span_from_c("21.36"),
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "gl_motionblur_min_angle",
                                                    .location = &doom_state->defaults_storage.gl_motionblur_min_angle,
                                                    .default_value = phyto_string_span_from_c("20.0"),
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "gl_motionblur_att_a",
                                                    .location = &doom_state->defaults_storage.gl_motionblur_att_a,
                                                    .default_value = phyto_string_span_from_c("55.0"),
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "gl_motionblur_att_b",
                                                    .location = &doom_state->defaults_storage.gl_motionblur_att_b,
                                                    .default_value = phyto_string_span_from_c("1.8"),
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "gl_motionblur_att_c",
                                                    .location = &doom_state->defaults_storage.gl_motionblur_att_c,
                                                    .default_value = phyto_string_span_from_c("0.9"),
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "gl_lightmode",
                                                    .location = (int*)&doom_state->defaults_storage.gl_lightmode,
                                                    .default_value = doom_gl_struct_lightmode_glboom,
                                                    .min_value = 0,
                                                    .max_value = doom_gl_struct_lightmode_count - 1,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "gl_light_ambient",
                                                    .location = (int*)&doom_state->defaults_storage.gl_light_ambient,
                                                    .default_value = 20,
                                                    .min_value = 1,
                                                    .max_value = 255,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "gl_fog",
                                                    .location = &doom_state->defaults_storage.gl_fog,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_hexint_default((hexint_default_t){
                                                    .name = "gl_fog_color",
                                                    .location = &doom_state->defaults_storage.gl_fog_color,
                                                    .default_value = 0x000000,
                                                    .min_value = 0x000000,
                                                    .max_value = 0xFFFFFF,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "useglgamma",
                                                    .location = &doom_state->defaults_storage.useglgamma,
                                                    .default_value = 6,
                                                    .min_value = 0,
                                                    .max_value = doom_gl_struct_max_glgamma,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "gl_color_mip_levels",
                                                    .location = &doom_state->defaults_storage.gl_color_mip_levels,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "gl_shadows",
                                                    .location = &doom_state->defaults_storage.gl_shadows,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "gl_shadows_maxdist",
                                                    .location = &doom_state->defaults_storage.gl_shadows_maxdist,
                                                    .default_value = 1000,
                                                    .min_value = 0,
                                                    .max_value = 32767,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "gl_shadows_factor",
                                                    .location = &doom_state->defaults_storage.gl_shadows_factor,
                                                    .default_value = 128,
                                                    .min_value = 0,
                                                    .max_value = 255,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "gl_blend_animations",
                                                    .location = &doom_state->defaults_storage.gl_blend_animations,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));

    doom_misc_default_dyarray_append(&defaults, s_header_default("PrBoom+ emulation settings"));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "overrun_spechit_warn",
                                                    .location = &doom_state->defaults_storage.overrun_spechit_warn,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "overrun_spechit_emulate",
                                                    .location = &doom_state->defaults_storage.overrun_spechit_emulate,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "overrun_reject_warn",
                                                    .location = &doom_state->defaults_storage.overrun_reject_warn,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "overrun_reject_emulate",
                                                    .location = &doom_state->defaults_storage.overrun_reject_emulate,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "overrun_intercept_warn",
                                                    .location = &doom_state->defaults_storage.overrun_intercept_warn,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "overrun_intercept_emulate",
                                                    .location = &doom_state->defaults_storage.overrun_intercept_emulate,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "overrun_playeringame_warn",
                                                    .location = &doom_state->defaults_storage.overrun_playeringame_warn,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_boolean_default((boolean_default_t){
                                         .name = "overrun_playeringame_emulate",
                                         .location = &doom_state->defaults_storage.overrun_playeringame_emulate,
                                         .default_value = true,
                                         .setup_screen = doom_misc_setup_screen_status_bar,
                                     }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "overrun_donut_warn",
                                                    .location = &doom_state->defaults_storage.overrun_donut_warn,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "overrun_donut_emulate",
                                                    .location = &doom_state->defaults_storage.overrun_donut_emulate,
                                                    .default_value = true,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_boolean_default((boolean_default_t){
                                         .name = "overrun_missedbackside_warn",
                                         .location = &doom_state->defaults_storage.overrun_missedbackside_warn,
                                         .default_value = false,
                                         .setup_screen = doom_misc_setup_screen_status_bar,
                                     }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_boolean_default((boolean_default_t){
                                         .name = "overrun_missedbackside_emulate",
                                         .location = &doom_state->defaults_storage.overrun_missedbackside_emulate,
                                         .default_value = true,
                                         .setup_screen = doom_misc_setup_screen_status_bar,
                                     }));

    doom_misc_default_dyarray_append(&defaults, s_header_default("PrBoom+ 'bad' compatibility settings"));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "comperr_zerotag",
                                                    .location = &doom_state->defaults_storage.comperr_zerotag,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "comperr_passuse",
                                                    .location = &doom_state->defaults_storage.comperr_passuse,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "comperr_hangsolid",
                                                    .location = &doom_state->defaults_storage.comperr_hangsolid,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "comperr_blockmap",
                                                    .location = &doom_state->defaults_storage.comperr_blockmap,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_boolean_default((boolean_default_t){
                                                    .name = "comperr_freeaim",
                                                    .location = &doom_state->defaults_storage.comperr_freeaim,
                                                    .default_value = false,
                                                    .setup_screen = doom_misc_setup_screen_status_bar,
                                                }));

    doom_misc_default_dyarray_append(&defaults, s_header_default("PrBoom+ demo patterns list"));
    doom_misc_default_dyarray_append(&defaults, s_string_default((string_default_t){
                                                    .name = "demo_patterns_mask",
                                                    .location = &doom_state->defaults_storage.demo_patterns_mask,
                                                    .default_value = phyto_string_span_from_c("demo_pattern"),
                                                    .setup_screen = doom_misc_setup_screen_none,
                                                }));
    doom_misc_default_dyarray_append(
        &defaults, s_owning_string_default((owning_string_default_t){
                       .name = "demo_pattern0",
                       .location = &doom_state->defaults_storage.demo_patterns_list_def.data[0],
                       .default_value = phyto_string_span_from_c(
                           "DOOM 2: Hell on Earth/((lv)|(nm)|(pa)|(ty))\\d\\d.\\d\\d\\d\\.lmp/doom2.wad"),
                       .setup_screen = doom_misc_setup_screen_none,
                   }));
    doom_misc_default_dyarray_append(
        &defaults,
        s_owning_string_default((owning_string_default_t){
            .name = "demo_pattern1",
            .location = &doom_state->defaults_storage.demo_patterns_list_def.data[1],
            .default_value = phyto_string_span_from_c(
                "DOOM 2: Plutonia Experiment/p(c|f|l|n|p|r|s|t)\\d\\d.\\d\\d\\d\\.lmp/doom2.wad|plutonia.wad"),
            .setup_screen = doom_misc_setup_screen_none,
        }));
    doom_misc_default_dyarray_append(
        &defaults, s_owning_string_default((owning_string_default_t){
                       .name = "demo_pattern2",
                       .location = &doom_state->defaults_storage.demo_patterns_list_def.data[2],
                       .default_value = phyto_string_span_from_c(
                           "DOOM 2: TNT - Evilution/((e(c|f|v|p|r|s|t))|(tn))\\d\\d.\\d\\d\\d\\.lmp/doom2.wad|tnt.wad"),
                       .setup_screen = doom_misc_setup_screen_none,
                   }));
    doom_misc_default_dyarray_append(
        &defaults, s_owning_string_default((owning_string_default_t){
                       .name = "demo_pattern3",
                       .location = &doom_state->defaults_storage.demo_patterns_list_def.data[3],
                       .default_value = phyto_string_span_from_c(
                           "The Ultimate DOOM/(((e|f|n|p|r|t|u)\\dm\\d)|(n\\ds\\d)).\\d\\d\\d\\.lmp/doomu.wad"),
                       .setup_screen = doom_misc_setup_screen_none,
                   }));
    doom_misc_default_dyarray_append(
        &defaults, s_owning_string_default((owning_string_default_t){
                       .name = "demo_pattern4",
                       .location = &doom_state->defaults_storage.demo_patterns_list_def.data[4],
                       .default_value = phyto_string_span_from_c(
                           "Alien Vendetta/a(c|f|n|p|r|s|t|v)\\d\\d.\\d\\d\\d\\.lmp/doom2.wad|av.wad|av.deh"),
                       .setup_screen = doom_misc_setup_screen_none,
                   }));
    doom_misc_default_dyarray_append(
        &defaults,
        s_owning_string_default((owning_string_default_t){
            .name = "demo_pattern5",
            .location = &doom_state->defaults_storage.demo_patterns_list_def.data[5],
            .default_value = phyto_string_span_from_c(
                "Requiem/r(c|f|n|p|q|r|s|t)\\d\\d.\\d\\d\\d\\.lmp/doom2.wad|requiem.wad|req21fix.wad|reqmus.wad"),
            .setup_screen = doom_misc_setup_screen_none,
        }));
    doom_misc_default_dyarray_append(
        &defaults, s_owning_string_default((owning_string_default_t){
                       .name = "demo_pattern6",
                       .location = &doom_state->defaults_storage.demo_patterns_list_def.data[6],
                       .default_value = phyto_string_span_from_c(
                           "Hell Revealed/h(c|e|f|n|p|r|s|t)\\d\\d.\\d\\d\\d\\.lmp/doom2.wad|hr.wad|hrmus.wad"),
                       .setup_screen = doom_misc_setup_screen_none,
                   }));
    doom_misc_default_dyarray_append(
        &defaults, s_owning_string_default((owning_string_default_t){
                       .name = "demo_pattern7",
                       .location = &doom_state->defaults_storage.demo_patterns_list_def.data[7],
                       .default_value =
                           phyto_string_span_from_c("Memento Mori/mm\\d\\d.\\d\\d\\d\\.lmp/doom2.wad|mm.wad|mmmus.wad"),
                       .setup_screen = doom_misc_setup_screen_none,
                   }));
    doom_misc_default_dyarray_append(&defaults,
                                     s_owning_string_default((owning_string_default_t){
                                         .name = "demo_pattern8",
                                         .location = &doom_state->defaults_storage.demo_patterns_list_def.data[8],
                                         .default_value = phyto_string_span_from_c(
                                             "Memento Mori 2/m2\\d\\d.\\d\\d\\d\\.lmp/doom2.wad|mm2.wad|mm2mus.wad"),
                                         .setup_screen = doom_misc_setup_screen_none,
                                     }));

    doom_misc_default_dyarray_append(&defaults, s_header_default("Weapon preferences"));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "weapon_choice_1",
                                                    .location = &doom_state->defaults_storage.weapon_preferences[0][0],
                                                    .default_value = 6,
                                                    .min_value = 0,
                                                    .max_value = 9,
                                                    .setup_screen = doom_misc_setup_screen_weapons,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "weapon_choice_2",
                                                    .location = &doom_state->defaults_storage.weapon_preferences[0][1],
                                                    .default_value = 9,
                                                    .min_value = 0,
                                                    .max_value = 9,
                                                    .setup_screen = doom_misc_setup_screen_weapons,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "weapon_choice_3",
                                                    .location = &doom_state->defaults_storage.weapon_preferences[0][2],
                                                    .default_value = 4,
                                                    .min_value = 0,
                                                    .max_value = 9,
                                                    .setup_screen = doom_misc_setup_screen_weapons,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "weapon_choice_4",
                                                    .location = &doom_state->defaults_storage.weapon_preferences[0][3],
                                                    .default_value = 3,
                                                    .min_value = 0,
                                                    .max_value = 9,
                                                    .setup_screen = doom_misc_setup_screen_weapons,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "weapon_choice_5",
                                                    .location = &doom_state->defaults_storage.weapon_preferences[0][4],
                                                    .default_value = 2,
                                                    .min_value = 0,
                                                    .max_value = 9,
                                                    .setup_screen = doom_misc_setup_screen_weapons,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "weapon_choice_6",
                                                    .location = &doom_state->defaults_storage.weapon_preferences[0][5],
                                                    .default_value = 8,
                                                    .min_value = 0,
                                                    .max_value = 9,
                                                    .setup_screen = doom_misc_setup_screen_weapons,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "weapon_choice_7",
                                                    .location = &doom_state->defaults_storage.weapon_preferences[0][6],
                                                    .default_value = 5,
                                                    .min_value = 0,
                                                    .max_value = 9,
                                                    .setup_screen = doom_misc_setup_screen_weapons,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "weapon_choice_8",
                                                    .location = &doom_state->defaults_storage.weapon_preferences[0][7],
                                                    .default_value = 7,
                                                    .min_value = 0,
                                                    .max_value = 9,
                                                    .setup_screen = doom_misc_setup_screen_weapons,
                                                }));
    doom_misc_default_dyarray_append(&defaults, s_integer_default((integer_default_t){
                                                    .name = "weapon_choice_9",
                                                    .location = &doom_state->defaults_storage.weapon_preferences[0][8],
                                                    .default_value = 1,
                                                    .min_value = 0,
                                                    .max_value = 9,
                                                    .setup_screen = doom_misc_setup_screen_weapons,
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

doom_misc_default_t s_hexint_default(hexint_default_t value) {
    return (doom_misc_default_t){
        .name = value.name,
        .location = {.px = value.location},
        .default_value = {.x = value.default_value},
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

doom_misc_default_t s_owning_string_default(owning_string_default_t value) {
    return (doom_misc_default_t){
        .name = value.name,
        .location = {.pos = value.location},
        .default_value = {.s = value.default_value},
        .min_value = min_unset,
        .max_value = max_unset,
        .type = doom_misc_default_type_owning_string,
        .setup_screen = value.setup_screen,
    };
}
