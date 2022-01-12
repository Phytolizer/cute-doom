#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#define DOOM_LOG_LEVELS_X                                                                                              \
    X(info, 0x1U)                                                                                                      \
    X(warn, 0x2U)                                                                                                      \
    X(error, 0x4U)                                                                                                     \
    X(debug, 0x8U)

///
/// \brief Log levels.
///
typedef enum
{
#define X(x, y) doom_log_level_##x = y,
    DOOM_LOG_LEVELS_X
#undef X
} doom_log_level_t;

///
/// \brief Logging function.
///
/// \param level Log level.
/// \param format Format string.
/// \param ... Format arguments.
///
/// \return Whatever `doom_vsnprintf` returns.
///
/// \see doom_vsnprintf
///
int32_t doom_log_printf(doom_log_level_t level, const char* format, ...) __attribute__((format(printf, 2, 3)));

///
/// \brief Log an error and exit.
///
void doom_log_error(const char* format, ...) __attribute__((format(printf, 1, 2))) __attribute__((noreturn));

///
/// \brief Non-standard version of vsnprintf. Could go in the `nonstd` library.
///
/// \param buffer The buffer to write to.
/// \param buffer_len The size of the buffer.
/// \param format The format string.
/// \param args The arguments to the format string.
///
/// \return The number of bytes written to the buffer, or a negative value on error.
///
int32_t doom_vsnprintf(char* buffer, size_t buffer_len, const char* format, va_list args);
