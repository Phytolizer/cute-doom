#include "doom/sys/system.h"

#include "doom/init.h"
#include "doom/log/printf.h"

#include <config.h>
#include <stdio.h>
#include <stdlib.h>

void doom_sys_atexit(doom_sys_atexit_func_t func, bool run_if_error, const char* name,
                     doom_sys_exit_priority_t priority) {
    doom_sys_atexit_list_entry_t* entry = malloc(sizeof(doom_sys_atexit_list_entry_t));
    entry->func = func;
    entry->run_if_error = run_if_error;
    entry->name = name;
    entry->next = doom_state->exit_funcs[priority];
    doom_state->exit_funcs[priority] = entry;
}

const char* doom_sys_get_version_string(char* buffer, size_t buffer_len) {
    snprintf(buffer, buffer_len, "%s v%s (%s)", PROJECT_NAME, PROJECT_VERSION, PROJECT_HOMEPAGE_URL);
    return buffer;
}

void doom_sys_safe_exit(int32_t exit_code) {
    for (doom_sys_exit_priority_t exit_priority = doom_sys_exit_priority_first;
         exit_priority < doom_sys_exit_priority_max; ++exit_priority) {
        doom_sys_atexit_list_entry_t* entry;
        while ((entry = doom_state->exit_funcs[exit_priority]) != NULL) {
            doom_state->exit_funcs[exit_priority] = entry->next;
            if (entry->run_if_error || exit_code == 0) {
                doom_log_printf(doom_log_level_info, "Exit Sequence [%d]: %s (%d)\n", exit_priority, entry->name,
                                exit_code);
                entry->func();
            }
            free(entry);
        }
    }
    exit(exit_code);
}
