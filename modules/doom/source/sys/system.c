#include "doom/sys/system.h"

#include <config.h>
#include <stdio.h>

const char* doom_sys_get_version_string(char* buffer, size_t buffer_len) {
    snprintf(buffer, buffer_len, "%s v%s (%s)", PROJECT_NAME, PROJECT_VERSION, PROJECT_HOMEPAGE_URL);
    return buffer;
}
