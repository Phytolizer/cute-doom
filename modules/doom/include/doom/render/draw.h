#pragma once

#define DOOM_RENDER_DRAW_FILTER_TYPES_X                                                                                \
    X(none)                                                                                                            \
    X(point)                                                                                                           \
    X(linear)                                                                                                          \
    X(rounded)                                                                                                         \
    X(count)

typedef enum
{
#define X(x) doom_render_draw_filter_type_##x,
    DOOM_RENDER_DRAW_FILTER_TYPES_X
#undef X
} doom_render_draw_filter_type_t;

#define DOOM_RENDER_DRAW_SLOPED_EDGE_TYPES_X                                                                           \
    X(square)                                                                                                          \
    X(sloped)

typedef enum
{
#define X(x) doom_render_draw_sloped_edge_type_##x,
    DOOM_RENDER_DRAW_SLOPED_EDGE_TYPES_X
#undef X
} doom_render_draw_sloped_edge_type_t;
