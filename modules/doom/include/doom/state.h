#pragma once

#include "doom/automap.h"
#include "doom/gl/struct.h"
#include "doom/hud/chat.h"
#include "doom/misc/argv.h"
#include "doom/misc/defaults.h"
#include "doom/render/draw.h"
#include "doom/render/things.h"
#include "doom/status_bar/ammo.h"
#include "doom/sys/compatibility.h"
#include "doom/sys/system.h"
#include "doom/video/patch.h"

#include <phyto/string/string.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum
{ doom_maxloadfiles = 3, };

#define DOOM_CENTERWEAPONS_X                                                                                           \
    X(off)                                                                                                             \
    X(horizontal)                                                                                                      \
    X(horizontal_vertical)                                                                                             \
    X(bob)                                                                                                             \
    X(count)

typedef enum
{
#define X(x) doom_centerweapon_##x,
    DOOM_CENTERWEAPONS_X
#undef X
} doom_centerweapon_t;

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
    doom_misc_parameters_t params;
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
    phyto_string_span_t wad_files[doom_maxloadfiles];
    phyto_string_span_t deh_files[doom_maxloadfiles];

    // Game settings
    int32_t default_skill;
    doom_centerweapon_t weapon_attack_alignment;
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
} doom_state_t;

doom_state_t* doom_state_new(int argc, char** argv);
void doom_state_free(doom_state_t** p_state);
