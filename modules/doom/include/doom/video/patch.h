#pragma once

#define DOOM_VIDEO_PATCH_STRETCHES_X                                                                                   \
    X(_16x10)                                                                                                          \
    X(_4x3)                                                                                                            \
    X(_full)                                                                                                           \
    X(_count)

///
/// \brief Specifies to what aspect ratio patches should be stretched.
///
typedef enum
{
#define X(x) doom_video_patch_stretch##x,
    DOOM_VIDEO_PATCH_STRETCHES_X
#undef X
} doom_video_patch_stretch_t;
