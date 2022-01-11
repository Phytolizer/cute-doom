#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define DOOM_SYS_EXIT_PRIORITIES_X                                                                                     \
    X(first)                                                                                                           \
    X(normal)                                                                                                          \
    X(last)                                                                                                            \
    X(max)

typedef enum
{
#define X(x) doom_sys_exit_priority_##x,
    DOOM_SYS_EXIT_PRIORITIES_X
#undef X
} doom_sys_exit_priority_t;

typedef void (*doom_sys_atexit_func_t)(void);

typedef struct doom_sys_atexit_list_entry_s {
    doom_sys_atexit_func_t func;
    bool run_if_error;
    const char* name;
    struct doom_sys_atexit_list_entry_s* next;
} doom_sys_atexit_list_entry_t;

///
/// \brief Run a function at exit.
///
/// \param func The function to run.
/// \param run_if_error Whether to run the function if the program exits with an error.
/// \param name The name of the function.
/// \param priority The priority of the function.
///
void doom_sys_atexit(doom_sys_atexit_func_t func, bool run_if_error, const char* name,
                     doom_sys_exit_priority_t priority);

// Convenience macro for doom_sys_atexit.
#define DOOM_SYS_ATEXIT(Func, RunIfError, Priority) doom_sys_atexit(Func, RunIfError, #Func, Priority)

///
/// \brief Returns a version string in the given buffer.
///
/// \param buffer The buffer to write the version string to.
/// \param buffer_len The length of `buffer`.
///
/// \return buffer.
///
const char* doom_sys_get_version_string(char* buffer, size_t buffer_len);

///
/// \brief Run through the exit hooks and quit with the given exit code.
///
/// \param exit_code The exit code to exit with.
///
void doom_sys_safe_exit(int32_t exit_code) __attribute__((noreturn));
