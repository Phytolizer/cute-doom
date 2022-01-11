#include "doom/misc/argv.h"

#include "doom/init.h"
#include "doom/log/printf.h"

#include <nonstd/ctype.h>
#include <nonstd/strdup.h>
#include <nonstd/stricmp.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

int32_t doom_misc_check_parameter(const char* parameter) {
    return doom_misc_check_parameter_ex(parameter, doom_state->argv, doom_state->argc);
}

int32_t doom_misc_check_parameter_ex(const char* parameter, char** params, int32_t params_len) {
    for (int32_t i = 0; i < params_len; i++) {
        if (nonstd_stricmp(params[i], parameter) == 0) {
            return i;
        }
    }
    return -1;
}

void doom_misc_add_parameter(const char* parameter) {
    doom_state->argv = realloc(doom_state->argv, sizeof(char*) * (doom_state->argc + 1));
    doom_state->argv[doom_state->argc] = nonstd_strdup(parameter);
    doom_state->argc++;
}

void doom_misc_parse_command_line(char* cmd_start, char** argv, char* args, int32_t* out_argv_len,
                                  int32_t* out_args_len) {
    char* p = cmd_start;

    *out_args_len = 0;
    *out_argv_len = 0;
    bool in_quote = false;

    // each argument
    while (true) {
        while (nonstd_isspace(*p)) {
            ++p;
        }

        // detect end of arguments
        if (*p == '\0') {
            break;
        }

        // store pointer to arg
        if (argv) {
            *argv++ = args;
        }
        ++(*out_argv_len);

        // scan one argument
        while (true) {
            bool copy_char = true;
            size_t num_slashes = 0;

            // translation rules:
            // 2N backslashes + " -> N backslashes and begin/end quote
            // 2N+1 backslashes + " -> N backslashes + literal "
            // N backslashes -> N backslashes
            while (*p == '\\') {
                ++p;
                ++num_slashes;
            }
            if (*p == '"') {
                if ((num_slashes % 2) == 0) {
                    if (in_quote) {
                        if (p[1] == '"') {
                            // double quote in quoted string
                            // skip first quote, copy second
                            ++p;
                        } else {
                            copy_char = false;
                        }
                    } else {
                        copy_char = false;
                    }
                    in_quote = !in_quote;
                }
                num_slashes /= 2;
            }

            // copy remaining slashes
            while (num_slashes--) {
                if (args) {
                    *args++ = '\\';
                }
                ++(*out_args_len);
            }

            // detect end of argument
            if (*p == '\0' || (!in_quote && nonstd_isspace(*p))) {
                break;
            }

            if (copy_char) {
                if (args) {
                    *args++ = *p;
                }
                ++(*out_args_len);
            }
            ++p;
        }

        if (args) {
            *args++ = '\0';
        }
        ++(*out_args_len);
    }
}

void doom_misc_detect_clashing_parameters(void) {
    bool recording_attempt =
        (doom_misc_check_parameter("-record") >= 0) || (doom_misc_check_parameter("-recordfromto") >= 0);
    bool playback_attempt = (doom_misc_check_parameter("-playdemo") >= 0) ||
                            (doom_misc_check_parameter("-timedemo") >= 0) ||
                            (doom_misc_check_parameter("-fastdemo") >= 0);

    if (recording_attempt && playback_attempt) {
        doom_log_error("Cannot record and play back a demo at the same time.");
    }
}
