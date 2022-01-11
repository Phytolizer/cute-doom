#pragma once

#include <stddef.h>

///
/// \brief Returns a version string in the given buffer.
///
/// \param buffer The buffer to write the version string to.
/// \param buffer_len The length of `buffer`.
///
/// \return buffer.
///
const char* doom_sys_get_version_string(char* buffer, size_t buffer_len);
