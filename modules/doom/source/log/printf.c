#include "doom/log/printf.h"

#include "doom/sys/system.h"

#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const doom_log_level_t console_stdout_mask = doom_log_level_info;
static const doom_log_level_t console_stderr_mask = doom_log_level_warn | doom_log_level_error;

#define MAX_MESSAGE_LENGTH 2048

int32_t doom_log_printf(doom_log_level_t level, const char* format, ...) {
    char message[MAX_MESSAGE_LENGTH];

    va_list args;
    va_start(args, format);
    doom_vsnprintf(message, sizeof(message), format, args);
    va_end(args);

    int32_t result = 0;

    if (level & console_stdout_mask) {
        result = fprintf(stdout, "%s", message);
    }

    if (level & console_stderr_mask) {
        result = fprintf(stderr, "%s", message);
    }

    return result;
}

void doom_log_error(const char* format, ...) {
    char error_message[MAX_MESSAGE_LENGTH];

    va_list args;
    va_start(args, format);
    doom_vsnprintf(error_message, sizeof(error_message), format, args);
    va_end(args);

    doom_log_printf(doom_log_level_error, "%s\n", error_message);
    doom_sys_safe_exit(-1);
}

int32_t doom_vsnprintf(char* buffer, size_t buffer_len, const char* format, va_list args) {
    assert((buffer_len == 0 && buffer == NULL) || (buffer_len > 0 && buffer != NULL));
    assert(format != NULL);

    va_list args_copy;
    va_copy(args_copy, args);
    int32_t result = vsnprintf(buffer, buffer_len, format, args_copy);
    va_end(args_copy);

    if (result < 0) {
        va_copy(args_copy, args);
        int32_t len = vsnprintf(NULL, 0, format, args);
        va_end(args_copy);

        if (len < 0) {
            return len;
        }

        char* backbuffer = malloc((size_t)len + 1);
        if (backbuffer == NULL) {
            return -1;
        }

        result = vsnprintf(backbuffer, (size_t)len + 1, format, args);
        if (result < 0) {
            free(backbuffer);
            return result;
        }

        if (buffer_len > (size_t)result) {
            memcpy(buffer, backbuffer, (size_t)result);
            buffer[result] = '\0';
        } else {
            memcpy(buffer, backbuffer, buffer_len - 1);
            buffer[buffer_len - 1] = '\0';
        }
        free(backbuffer);
    } else if ((size_t)result >= buffer_len && buffer_len > 0 && buffer[buffer_len - 1] != '\0') {
        buffer[buffer_len - 1] = '\0';
    }
    return result;
}
