#pragma once

#define DOOM_KEYS_X                                                                                                    \
    X(right_arrow, 0xAE)                                                                                               \
    X(left_arrow, 0xAC)                                                                                                \
    X(up_arrow, 0xAD)                                                                                                  \
    X(down_arrow, 0xAF)                                                                                                \
    X(escape, 0x1B)                                                                                                    \
    X(enter, 0x0D)                                                                                                     \
    X(tab, 0x09)                                                                                                       \
    X(f1, 0x80 + 0x3B)                                                                                                 \
    X(f2, 0x80 + 0x3C)                                                                                                 \
    X(f3, 0x80 + 0x3D)                                                                                                 \
    X(f4, 0x80 + 0x3E)                                                                                                 \
    X(f5, 0x80 + 0x3F)                                                                                                 \
    X(f6, 0x80 + 0x40)                                                                                                 \
    X(f7, 0x80 + 0x41)                                                                                                 \
    X(f8, 0x80 + 0x42)                                                                                                 \
    X(f9, 0x80 + 0x43)                                                                                                 \
    X(f10, 0x80 + 0x44)                                                                                                \
    X(f11, 0x80 + 0x57)                                                                                                \
    X(f12, 0x80 + 0x58)                                                                                                \
    X(backspace, 0x7F)                                                                                                 \
    X(pause, 0xFF)                                                                                                     \
    X(equals, 0x3D)                                                                                                    \
    X(minus, 0x2D)                                                                                                     \
    X(right_shift, 0x80 + 0x36)                                                                                        \
    X(right_ctrl, 0x80 + 0x1D)                                                                                         \
    X(right_alt, 0x80 + 0x38)                                                                                          \
    X(left_alt, doom_key_right_alt)                                                                                    \
    X(caps_lock, 0xBA)                                                                                                 \
    X(print_screen, 0xFE)                                                                                              \
    X(insert, 0xD2)                                                                                                    \
    X(home, 0xC7)                                                                                                      \
    X(page_up, 0xC9)                                                                                                   \
    X(page_down, 0xD1)                                                                                                 \
    X(delete, 0xC8)                                                                                                    \
    X(end, 0xCF)                                                                                                       \
    X(scroll_lock, 0xC6)                                                                                               \
    X(space_bar, 0x20)                                                                                                 \
    X(num_lock, 0xC5)                                                                                                  \
    X(keypad0, 0x100 + '0')                                                                                            \
    X(keypad1, 0x100 + '1')                                                                                            \
    X(keypad2, 0x100 + '2')                                                                                            \
    X(keypad3, 0x100 + '3')                                                                                            \
    X(keypad4, 0x100 + '4')                                                                                            \
    X(keypad5, 0x100 + '5')                                                                                            \
    X(keypad6, 0x100 + '6')                                                                                            \
    X(keypad7, 0x100 + '7')                                                                                            \
    X(keypad8, 0x100 + '8')                                                                                            \
    X(keypad9, 0x100 + '9')                                                                                            \
    X(keypad_enter, 0x100 + doom_key_enter)                                                                            \
    X(keypad_divide, 0x100 + '/')                                                                                      \
    X(keypad_multiply, 0x100 + '*')                                                                                    \
    X(keypad_minus, 0x100 + '-')                                                                                       \
    X(keypad_plus, 0x100 + '+')                                                                                        \
    X(keypad_period, 0x100 + '.')                                                                                      \
    X(mouse1, 0x80 + 0x60)                                                                                             \
    X(mouse2, 0x80 + 0x61)                                                                                             \
    X(mouse3, 0x80 + 0x62)                                                                                             \
    X(mouse_wheel_up, 0x80 + 0x6B)                                                                                     \
    X(mouse_wheel_down, 0x80 + 0x6C)

typedef enum
{
#define X(x, y) doom_key_##x = y,
    DOOM_KEYS_X
#undef X
} doom_key_t;
