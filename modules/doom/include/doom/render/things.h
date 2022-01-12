#pragma once

#define DOOM_RENDER_THINGS_SPRITE_ORDERS_X                                                                             \
    X(no_order)                                                                                                        \
    X(static_order)                                                                                                    \
    X(dynamic_order)                                                                                                   \
    X(count)

///
/// \brief Sprite draw order.
///
typedef enum
{
#define X(x) doom_render_things_sprite_order_##x,
    DOOM_RENDER_THINGS_SPRITE_ORDERS_X
#undef X
} doom_render_things_sprite_order_t;
