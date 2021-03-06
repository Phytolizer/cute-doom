#include "doom/init.h"

#include "doom/log/printf.h"
#include "doom/misc/argv.h"
#include "doom/misc/defaults.h"
#include "doom/state.h"
#include "doom/sys/system.h"

#include <stdlib.h>

doom_state_t* doom_state;

static void s_print_version(void);

void doom_init(int argc, char** argv) {
    doom_state = doom_state_new(argc, argv);
    doom_state->defaults = doom_misc_default_dyarray_new();

    if (doom_misc_check_parameter("-v") > 0) {
        s_print_version();
        doom_quit(0);
    }

    doom_misc_detect_clashing_parameters();

    doom_log_printf(doom_log_level_info, "\n");
    s_print_version();
}

noreturn void doom_quit(int32_t exit_code) {
    doom_state_free(&doom_state);
    exit(exit_code);
}

void s_print_version(void) {
    char version_buffer[200];
    doom_log_printf(doom_log_level_info, "%s\n", doom_sys_get_version_string(version_buffer, sizeof(version_buffer)));
}
