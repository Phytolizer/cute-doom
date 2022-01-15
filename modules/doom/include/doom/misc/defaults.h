#pragma once

#include "doom/automap.h"
#include "doom/dsda/input.h"
#include "doom/gl/struct.h"
#include "doom/hud/chat.h"
#include "doom/render/demo.h"
#include "doom/render/draw.h"
#include "doom/render/things.h"
#include "doom/status_bar/ammo.h"
#include "doom/sys/compatibility.h"
#include "doom/video/patch.h"
#include "doom/weapons.h"

#include <phyto/collections/dynamic_array.h>
#include <phyto/string/string.h>
#include <stdbool.h>
#include <stdint.h>

enum
{
    ///
    /// \brief The maximum number of autoload files (WAD and DEH alike).
    ///
    doom_maxloadfiles = 3,
};

#define DOOM_CENTERWEAPONS_X                                                                                           \
    X(off)                                                                                                             \
    X(horizontal)                                                                                                      \
    X(horizontal_vertical)                                                                                             \
    X(bob)                                                                                                             \
    X(count)

///
/// \brief How the weapon sprite should be centered.
///
typedef enum
{
#define X(x) doom_centerweapon_##x,
    DOOM_CENTERWEAPONS_X
#undef X
} doom_centerweapon_t;

#define DOOM_ARMOR_COLOR_TYPES_X                                                                                       \
    X(strength)                                                                                                        \
    X(amount)

///
/// \brief Specifies how armor colorization should be done.
///
typedef enum
{
#define X(x) doom_armor_color_type_##x,
    DOOM_ARMOR_COLOR_TYPES_X
#undef X
} doom_armor_color_type_t;

#define DOOM_MUS_PAUSE_OPTS_X                                                                                          \
    X(kill)                                                                                                            \
    X(pause)                                                                                                           \
    X(continue_playing)

///
/// \brief Specifies music behavior when the game is paused.
///
typedef enum
{
#define X(x) doom_mus_pause_opt_##x,
    DOOM_MUS_PAUSE_OPTS_X
#undef X
} doom_mus_pause_opt_t;

#define DOOM_MISC_DEFAULT_TYPES_X                                                                                      \
    X(none, 0)                                                                                                         \
    X(string, 1)                                                                                                       \
    X(integer, 2)                                                                                                      \
    X(hex_integer, 3)                                                                                                  \
    X(array, 4)                                                                                                        \
    X(input, 5)                                                                                                        \
    X(boolean, 2)                                                                                                      \
    X(color, 3)

///
/// \brief The type of a default.
///
/// Specifies which fields of the default are valid.
///
typedef enum
{
#define X(x, y) doom_misc_default_type_##x = y,
    DOOM_MISC_DEFAULT_TYPES_X
#undef X
} doom_misc_default_type_t;

PHYTO_COLLECTIONS_DYNAMIC_ARRAY_DECL(doom_misc_default_array_value, phyto_string_t);

///
/// \brief A configured "default".
///
typedef struct {
    ///
    /// \brief The name of the default, for the config file.
    ///
    const char* name;

    ///
    /// \brief The location of the default. The field that should be accessed is determined by the type.
    ///
    /// For integers, you may access `pi`.
    /// For booleans, you may access `pb`.
    /// For strings, you may access `ps`.
    /// For arrays, you may access `array` and its members.
    ///
    struct {
        int32_t* pi;
        bool* pb;
        phyto_string_span_t* ps;
        struct {
            int32_t* size;
            doom_misc_default_array_value_t* data;
            int32_t index;
        } array;
    } location;

    ///
    /// \brief The value of the default, if it is not configured. The field that should be accessed is determined by the
    /// type.
    ///
    /// For integers, you may access `i`.
    /// For booleans, you may access `b`.
    /// For strings, you may access `s`.
    /// For arrays, you may access `array` and its members.
    ///
    struct {
        int32_t i;
        bool b;
        phyto_string_span_t s;
        struct {
            int32_t size;
            phyto_string_span_t* data;
        } array;
    } default_value;

    ///
    /// \brief The minimum value that may be configured, or `min_unset` for no minimum.
    ///
    int32_t min_value;
    ///
    /// \brief The maximum value that may be configured, or `max_unset` for no maximum.
    ///
    int32_t max_value;

    ///
    /// \brief The type of the default.
    ///
    doom_misc_default_type_t type;

    ///
    /// \brief The setup screen on which the default is displayed.
    ///
    int32_t setup_screen;

    ///
    /// \brief A pointer to the current value (?).
    ///
    int32_t* current;

    // This must be pre-declared due to a cyclic dependency.

    ///
    /// \brief The setup menu on which the default is displayed.
    ///
    struct doom_misc_setup_menu_s* setup_menu;

    ///
    /// \brief (DSDA-Doom) A unique identifier for this default.
    ///
    /// This is used to identify the default in the input configuration, because the DSDA subsystem doesn't use the
    /// pointer fields.
    ///
    doom_dsda_input_identifier_t identifier;

    ///
    /// \brief (DSDA-Doom) The default for an input, if it is not configured.
    ///
    /// Only input config options use this.
    ///
    doom_dsda_input_default_t input;
} doom_misc_default_t;

PHYTO_COLLECTIONS_DYNAMIC_ARRAY_DECL(doom_misc_default_dyarray, doom_misc_default_t);

///
/// \brief Construct an array of defaults, already filled with the default values.
///
doom_misc_default_dyarray_t doom_misc_default_dyarray_new(void);

#define DOOM_MISC_SETUP_SCREENS_X                                                                                      \
    X(none)                                                                                                            \
    X(keys)                                                                                                            \
    X(weapons)                                                                                                         \
    X(status_bar)                                                                                                      \
    X(auto)                                                                                                            \
    X(messages)                                                                                                        \
    X(chat)                                                                                                            \
    X(general)                                                                                                         \
    X(max)

///
/// \brief Setup screens.
///
typedef enum
{
#define X(x) doom_misc_setup_screen_##x,
    DOOM_MISC_SETUP_SCREENS_X
#undef X
} doom_misc_setup_screen_t;

typedef struct {
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
    phyto_string_span_t wad_files[doom_maxloadfiles];
    phyto_string_span_t deh_files[doom_maxloadfiles];

    // Game settings
    int32_t default_skill;
    doom_centerweapon_t weapon_attack_alignment;
    bool sts_always_red;
    bool sts_pct_always_gray;
    bool sts_traditional_keys;
    doom_armor_color_type_t sts_armorcolor_type;

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
    phyto_string_span_t snd_midiplayer;
    phyto_string_span_t snd_soundfont;
    phyto_string_span_t snd_mididev;
    bool full_sounds;
    bool mus_fluidsynth_chorus;
    bool mus_fluidsynth_reverb;
    int32_t mus_fluidsynth_gain;
    int32_t mus_opl_gain;

    // Video settings
    phyto_string_span_t videomode;
    phyto_string_span_t screen_resolution;
    phyto_string_span_t custom_resolution;
    bool use_fullscreen;
    bool exclusive_fullscreen;
    bool gl_exclusive_fullscreen;
    bool render_vsync;
    int32_t tran_filter_pct;
    int32_t screenblocks;
    int32_t usegamma;
    bool uncapped_framerate;
    doom_render_draw_filter_type_t filter_wall;
    doom_render_draw_filter_type_t filter_floor;
    doom_render_draw_filter_type_t filter_sprite;
    doom_render_draw_filter_type_t filter_z;
    doom_render_draw_filter_type_t filter_patch;
    int32_t filter_threshold;
    doom_render_draw_sloped_edge_type_t sprite_edges;
    doom_render_draw_sloped_edge_type_t patch_edges;

    // OpenGL settings
    bool gl_compatibility;
    struct {
        bool multitexture;
        bool texture_compression;
        bool texture_non_power_of_two;
        bool pixel_buffer_object;
        bool shader_objects;
    } gl_arb;
    struct {
        bool arb_vertex_buffer_object;
        bool blend_color;
        bool framebuffer_object;
        bool packed_depth_stencil;
        bool texture_filter_anisotropic;
    } gl_ext;
    bool gl_use_stencil;
    bool gl_use_display_lists;
    bool gl_finish;
    bool gl_clear;
    bool gl_ztrick;
    int32_t gl_nearclip;
    int32_t gl_colorbuffer_bits;
    int32_t gl_depthbuffer_bits;
    doom_gl_struct_filter_texture_mode_t gl_texture_filter;
    doom_gl_struct_filter_texture_mode_t gl_sprite_filter;
    doom_gl_struct_filter_texture_mode_t gl_patch_filter;
    doom_gl_struct_anisotropic_mode_t gl_texture_filter_anisotropic;
    phyto_string_span_t gl_tex_format_string;
    int32_t gl_sprite_offset;
    bool gl_sprite_blend;
    int32_t gl_mask_sprite_threshold;
    doom_gl_struct_skytype_t gl_skymode;
    int32_t gl_sky_detail;
    bool gl_use_paletted_texture;
    bool gl_use_shared_texture_palette;

    // Input settings
    int32_t input_profile;

    // The rest are handled by the DSDA subsystem.

    // Mouse settings
    bool use_mouse;
    int32_t mouse_sensitivity_horizontal;
    int32_t mouse_sensitivity_vertical;

    // Joystick settings
    int32_t use_joystick;

    // Chat macros
    phyto_string_span_t chat_macros[doom_hud_chat_macro_count];

    // Automap settings
    int32_t mapcolor_back;
    int32_t mapcolor_grid;
    int32_t mapcolor_wall;
    int32_t mapcolor_fchg;
    int32_t mapcolor_cchg;
    int32_t mapcolor_clsd;
    int32_t mapcolor_rkey;
    int32_t mapcolor_bkey;
    int32_t mapcolor_ykey;
    int32_t mapcolor_rdor;
    int32_t mapcolor_bdor;
    int32_t mapcolor_ydor;
    int32_t mapcolor_tele;
    int32_t mapcolor_secr;
    int32_t mapcolor_revsecr;
    int32_t mapcolor_exit;
    int32_t mapcolor_unsn;
    int32_t mapcolor_flat;
    int32_t mapcolor_sprt;
    int32_t mapcolor_item;
    int32_t mapcolor_hair;
    int32_t mapcolor_sngl;
    int32_t mapcolor_me;
    int32_t mapcolor_enemy;
    int32_t mapcolor_frnd;
    bool map_secret_after;
    bool map_point_coord;
    bool map_level_stat;
    doom_automap_mode_t automapmode;
    bool map_always_updates;
    int32_t map_grid_size;
    int32_t map_scroll_speed;
    bool map_wheel_zoom;
    bool map_use_multisampling;
    bool map_textured;
    int32_t map_textured_translucency;
    int32_t map_textured_overlay_translucency;
    int32_t map_lines_overlay_translucency;
    int32_t map_overlay_pos_x;
    int32_t map_overlay_pos_y;
    int32_t map_overlay_pos_width;
    int32_t map_overlay_pos_height;
    doom_automap_things_appearance_t map_things_appearance;

    // Heads-up display settings
    int32_t hudcolor_titl;
    int32_t hudcolor_xyco;
    int32_t hudcolor_mapstat_title;
    int32_t hudcolor_mapstat_value;
    int32_t hudcolor_mapstat_time;
    int32_t hudcolor_mesg;
    int32_t hudcolor_chat;
    int32_t hudcolor_list;
    int32_t hud_msg_lines;
    bool hud_list_bgon;
    int32_t health_red;
    int32_t health_yellow;
    int32_t health_green;
    int32_t armor_red;
    int32_t armor_yellow;
    int32_t armor_green;
    int32_t ammo_red;
    int32_t ammo_yellow;
    doom_status_bar_ammo_color_behavior_t ammo_color_behavior;
    int32_t hud_num;
    bool hud_displayed;

    // PrBoom+ heads-up display settings
    bool hudadd_gamespeed;
    bool hudadd_leveltime;
    bool hudadd_demotime;
    bool hudadd_secretarea;
    bool hudadd_maxtotals;
    bool hudadd_demoprogressbar;
    int32_t hudadd_crosshair;
    bool hudadd_crosshair_scale;
    int32_t hudadd_crosshair_color;
    bool hudadd_crosshair_health;
    bool hudadd_crosshair_target;
    int32_t hudadd_crosshair_target_color;
    bool hudadd_crosshair_lock_target;

    // PrBoom+ mouse settings
    int32_t mouse_acceleration;
    int32_t mouse_sensitivity_mlook;
    bool mouse_doubleclick_as_use;
    bool mouse_carrytics;

    // PrBoom+ demo settings
    phyto_string_span_t demo_demoex_filename;
    phyto_string_span_t getwad_cmdline;
    bool demo_overwriteexisting;
    int32_t quickstart_window_ms;

    // PrBoom+ game settings
    bool movement_strafe50;
    bool movement_strafe50onturns;
    bool movement_shorttics;
    int32_t interpolation_maxobjects;
    int32_t speed_step;

    // PrBoom+ misc settings
    bool showendoom;
    phyto_string_span_t screenshot_dir;
    bool health_bar;
    bool health_bar_full_length;
    int32_t health_bar_red;
    int32_t health_bar_yellow;
    int32_t health_bar_green;

    // DSDA settings
    struct {
        bool show_messages;
        bool autorun;
        int32_t fps_limit;

        bool strict_mode;
        bool cycle_ghost_colors;
        int32_t auto_key_frame_interval;
        int32_t auto_key_frame_depth;
        int32_t auto_key_frame_timeout;
        bool exhud;
        bool wipe_at_full_speed;
        bool show_demo_attempts;
        int32_t fine_sensitivity;
        bool hide_horns;
        bool organized_saves;
        bool command_display;
        int32_t command_history_size;
        bool hide_empty_commands;
        bool coordinate_display;
        bool skip_quit_prompt;
        bool show_split_data;
        phyto_string_span_t player_name;
        int32_t quickstart_cache_tics;
        int32_t death_use_action;
        bool mute_sfx;
        bool mute_music;
        bool cheat_codes;
        bool allow_jumping;
        int32_t parallel_sfx_limit;
        int32_t parallel_sfx_window;
        bool switch_when_ammo_runs_out;
        bool viewbob;
        bool weaponbob;
    } dsda;

    // Video capture encoding settings
    phyto_string_span_t cap_soundcommand;
    phyto_string_span_t cap_videocommand;
    phyto_string_span_t cap_muxcommand;
    phyto_string_span_t cap_tempfile1;
    phyto_string_span_t cap_tempfile2;
    bool cap_remove_tempfiles;
    int32_t cap_fps;

    // PrBoom+ video settings
    phyto_string_span_t sdl_video_window_pos;
    bool palette_ondamage;
    bool palette_onbonus;
    bool palette_onpowers;
    bool render_wipescreen;
    int32_t render_screen_multiply;
    bool integer_scaling;
    int32_t render_aspect;
    bool render_doom_lightmaps;
    bool fake_contrast;
    doom_video_patch_stretch_t render_stretch_hud;
    int32_t render_patches_scalex;
    int32_t render_patches_scaley;
    bool render_stretchsky;
    doom_render_things_sprite_order_t sprites_doom_order;
    bool movement_mouselook;
    bool movement_mousenovert;
    int32_t movement_mouseviewpitch;
    int32_t movement_mousestrafedivisor;
    bool movement_mouseinvert;

    // PrBoom+ OpenGL settings
    bool gl_allow_detail_textures;
    int32_t gl_detail_maxdist;
    int32_t render_multisampling;
    int32_t render_fov;
    doom_gl_struct_spriteclipmode_t gl_spriteclip;
    int32_t gl_spriteclip_threshold;
    bool gl_sprites_frustum_culling;
    bool render_paperitems;
    bool gl_boom_colormaps;
    bool gl_hires_24bit_colormap;
    bool gl_texture_internal_hires;
    bool gl_texture_external_hires;
    bool gl_hires_override_pwads;
    phyto_string_span_t gl_texture_hires_dir;
    bool gl_texture_hqresize;
    doom_gl_struct_hqresizemode_t gl_texture_hqresize_textures;
    doom_gl_struct_hqresizemode_t gl_texture_hqresize_sprites;
    doom_gl_struct_hqresizemode_t gl_texture_hqresize_patches;
    bool gl_motionblur;
    phyto_string_span_t gl_motionblur_min_speed;
    phyto_string_span_t gl_motionblur_min_angle;
    phyto_string_span_t gl_motionblur_att_a;
    phyto_string_span_t gl_motionblur_att_b;
    phyto_string_span_t gl_motionblur_att_c;
    doom_gl_struct_lightmode_t gl_lightmode;
    int32_t gl_light_ambient;
    bool gl_fog;
    uint32_t gl_fog_color;
    int32_t useglgamma;
    bool gl_color_mip_levels;
    bool gl_shadows;
    int32_t gl_shadows_maxdist;
    int32_t gl_shadows_factor;
    bool gl_blend_animations;

    // PrBoom+ emulation settings
    bool overrun_spechit_warn;
    bool overrun_spechit_emulate;
    bool overrun_reject_warn;
    bool overrun_reject_emulate;
    bool overrun_intercept_warn;
    bool overrun_intercept_emulate;
    bool overrun_playeringame_warn;
    bool overrun_playeringame_emulate;
    bool overrun_donut_warn;
    bool overrun_donut_emulate;
    bool overrun_missedbackside_warn;
    bool overrun_missedbackside_emulate;

    // PrBoom+ compatibility with mapping errors
    bool comperr_zerotag;
    bool comperr_passuse;
    bool comperr_hangsolid;
    bool comperr_blockmap;
    bool comperr_freeaim;

    // PrBoom+ demo patterns list
    phyto_string_span_t demo_patterns_mask;
    doom_demo_patterns_list_t demo_patterns_list_def;

    // Weapon preferences
    int32_t weapon_preferences[2][doom_weapon_type_count + 1];
} doom_misc_default_storage_t;

///
/// \brief Load default values from the config file.
///
void doom_misc_load_defaults(void);

///
/// \brief Save default values from the config file.
///
void doom_misc_save_defaults(void);
