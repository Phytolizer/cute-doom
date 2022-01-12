#include "doom/misc/argv.h"

#include "doom/init.h"
#include "doom/log/printf.h"
#include "phyto/string/string.h"

#include <nonstd/ctype.h>
#include <nonstd/strdup.h>
#include <nonstd/stricmp.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

PHYTO_COLLECTIONS_DYNAMIC_ARRAY_IMPL(doom_misc_parameters, phyto_string_t);

static const doom_misc_parameters_callbacks_t doom_misc_parameters_callbacks = {
    .free_cb = phyto_string_free,
    .compare_cb = phyto_string_compare,
    .copy_cb = phyto_string_copy,
    .print_cb = phyto_string_print_nosep,
};

doom_misc_parameters_t doom_misc_parameters_new(void) {
    return doom_misc_parameters_init(&doom_misc_parameters_callbacks);
}

int32_t doom_misc_check_parameter(const char* parameter) {
    return doom_misc_check_parameter_ex(parameter, doom_state->params);
}

int32_t doom_misc_check_parameter_ex(const char* parameter, doom_misc_parameters_t params) {
    for (size_t i = 0; i < params.size; i++) {
        char* nt = nonstd_strndup(params.data[i].data, params.data[i].size);
        if (nonstd_stricmp(nt, parameter) == 0) {
            free(nt);
            return (int32_t)i;
        }
        free(nt);
    }
    return -1;
}

void doom_misc_add_parameter(const char* parameter) {
    doom_misc_parameters_append(&doom_state->params, phyto_string_from_c(parameter));
}

void doom_misc_parse_command_line(char* cmd_start, doom_misc_parameters_t* out_params) {
    char* p = cmd_start;
    *out_params = doom_misc_parameters_new();
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
        phyto_string_t arg = phyto_string_new();

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
                phyto_string_append(&arg, '\\');
            }

            // detect end of argument
            if (*p == '\0' || (!in_quote && nonstd_isspace(*p))) {
                break;
            }

            if (copy_char) {
                phyto_string_append(&arg, *p);
            }
            ++p;
        }

        doom_misc_parameters_append(out_params, arg);
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
