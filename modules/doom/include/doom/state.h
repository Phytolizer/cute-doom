#pragma once

#include "doom/misc/defaults.h"
#include "doom/sys/compatibility.h"
#include "doom/sys/system.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum
{ doom_maxloadfiles = 3, };

enum
{
    doom_centerweapon_off,
    doom_centerweapon_horizontal,
    doom_centerweapon_horizontal_vertical,
    doom_centerweapon_bob,
    doom_centerweapon_count,
};

typedef enum
{
    doom_armorcolortype_strength,
    doom_armorcolortype_amount,
} doom_armorcolortype_t;

typedef enum
{
    doom_mus_pause_opt_kill,
    doom_mus_pause_opt_pause,
    doom_mus_pause_opt_continue,
} doom_mus_pause_opt_t;

typedef struct {
    int32_t argc;
    char** argv;
    doom_sys_atexit_list_entry_t* exit_funcs[doom_sys_exit_priority_max];
    doom_misc_default_dyarray_t defaults;

    // ========
    // Defaults
    // ========

    // System settings
    int32_t process_priority;

    // Misc settings
    doom_compatibility_level_t default_compatibility_level;
    int32_t realtic_clock_rate;
    bool menu_background;
    // Renamed from bodyquesize
    int32_t max_player_corpse;
    bool flashing_hom;
    int32_t endoom_mode;
    bool level_precache;
    bool demo_smoothturns;
    int32_t demo_smoothturnsfactor;

    // Files
    const char* wad_files[doom_maxloadfiles];
    const char* deh_files[doom_maxloadfiles];

    // Game settings
    int32_t default_skill;
    int32_t weapon_attack_alignment;
    bool sts_always_red;
    bool sts_pct_always_gray;
    bool sts_traditional_keys;
    doom_armorcolortype_t sts_armorcolor_type;

    // Dehacked settings
    bool deh_apply_cheats;

    // Sound settings
    bool snd_pcspeaker;
    int32_t sound_card;
    int32_t music_card;
    bool pitched_sounds;
    // Renamed from snd_samplerate
    int32_t samplerate;
    // Renamed from snd_samplecount
    int32_t slice_samplecount;
    // Renamed from snd_SfxVolume
    int32_t sfx_volume;
    // Renamed from snd_MusicVolume
    int32_t music_volume;
    doom_mus_pause_opt_t mus_pause_opt;
    // Renamed from default_numChannels
    int32_t snd_channels;
    const char* snd_midiplayer;
    const char* snd_soundfont;
    const char* snd_mididev;
    bool full_sounds;
    bool mus_fluidsynth_chorus;
    bool mus_fluidsynth_reverb;
    int32_t mus_fluidsynth_gain;
    int32_t mus_opl_gain;

    // Video settings
    const char* videomode;
    const char* screen_resolution;
    const char* custom_resolution;
    bool use_fullscreen;
    bool exclusive_fullscreen;
    bool gl_exclusive_fullscreen;
    bool render_vsync;
    int32_t tran_filter_pct;
    int32_t screenblocks;
    int32_t usegamma;
    bool uncapped_framerate;

    // DSDA settings
    struct {
        bool show_messages;
        bool autorun;
        int32_t fps_limit;
    } dsda;
} doom_state_t;

doom_state_t* doom_state_new(int argc, char** argv);
void doom_state_free(doom_state_t** p_state);
