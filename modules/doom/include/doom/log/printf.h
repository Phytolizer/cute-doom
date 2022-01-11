#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#define DOOM_LOG_LEVELS_X                                                                                              \
    X(info, 0x1U)                                                                                                      \
    X(warn, 0x2U)                                                                                                      \
    X(error, 0x4U)                                                                                                     \
    X(debug, 0x8U)

typedef enum
{
#define X(x, y) doom_log_##x = y,
    DOOM_LOG_LEVELS_X
#undef X
} doom_log_level_t;

int32_t doom_log_printf(doom_log_level_t level, const char* format, ...) __attribute__((format(printf, 2, 3)));

int32_t doom_vsnprintf(char* buffer, size_t buffer_len, const char* format, va_list args);
