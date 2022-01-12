#pragma once

#define DOOM_GL_STRUCT_FILTER_TEXTURE_MODES_X                                                                          \
    X(nearest)                                                                                                         \
    X(linear)                                                                                                          \
    X(nearest_mipmap_nearest)                                                                                          \
    X(nearest_mipmap_linear)                                                                                           \
    X(linear_mipmap_nearest)                                                                                           \
    X(linear_mipmap_linear)                                                                                            \
    X(count)

///
/// \brief Texture filter mode.
///
typedef enum
{
#define X(x) doom_gl_struct_filter_texture_mode_##x,
    DOOM_GL_STRUCT_FILTER_TEXTURE_MODES_X
#undef X
} doom_gl_struct_filter_texture_mode_t;

#define DOOM_GL_STRUCT_ANISOTROPIC_MODES_X                                                                             \
    X(off)                                                                                                             \
    X(on_2x)                                                                                                           \
    X(on_4x)                                                                                                           \
    X(on_8x)                                                                                                           \
    X(on_16x)

///
/// \brief Anisotropic mode.
///
typedef enum
{
#define X(x) doom_gl_struct_anisotropic_mode_##x,
    DOOM_GL_STRUCT_ANISOTROPIC_MODES_X
#undef X
} doom_gl_struct_anisotropic_mode_t;

#define DOOM_GL_STRUCT_SKYTYPES_X                                                                                      \
    X(auto)                                                                                                            \
    X(none)                                                                                                            \
    X(standard)                                                                                                        \
    X(skydome)                                                                                                         \
    X(screen)                                                                                                          \
    X(count)

///
/// \brief Sky type.
///
typedef enum
{
#define X(x) doom_gl_struct_skytype_##x,
    DOOM_GL_STRUCT_SKYTYPES_X
#undef X
} doom_gl_struct_skytype_t;

#define DOOM_GL_STRUCT_SPRITECLIPMODES_X                                                                               \
    X(constant)                                                                                                        \
    X(always)                                                                                                          \
    X(smart)

///
/// \brief Sprite clip mode.
///
typedef enum
{
#define X(x) doom_gl_struct_spriteclipmode_##x,
    DOOM_GL_STRUCT_SPRITECLIPMODES_X
#undef X
} doom_gl_struct_spriteclipmode_t;

#define DOOM_GL_STRUCT_HQRESIZEMODES_X                                                                                 \
    X(none)                                                                                                            \
    X(on_2x)                                                                                                           \
    X(on_3x)                                                                                                           \
    X(on_4x)                                                                                                           \
    X(count)

///
/// \brief High-quality resize mode.
///
typedef enum
{
#define X(x) doom_gl_struct_hqresizemode_##x,
    DOOM_GL_STRUCT_HQRESIZEMODES_X
#undef X
} doom_gl_struct_hqresizemode_t;

#define DOOM_GL_STRUCT_LIGHTMODES_X                                                                                    \
    X(glboom)                                                                                                          \
    X(gzdoom)                                                                                                          \
    X(fogbased)                                                                                                        \
    X(shaders)                                                                                                         \
    X(count)

///
/// \brief Light mode.
///
typedef enum
{
#define X(x) doom_gl_struct_lightmode_##x,
    DOOM_GL_STRUCT_LIGHTMODES_X
#undef X
} doom_gl_struct_lightmode_t;

enum
{
    ///
    /// \brief Maximum OpenGL gamma value.
    ///
    doom_gl_struct_max_glgamma = 32,
};
