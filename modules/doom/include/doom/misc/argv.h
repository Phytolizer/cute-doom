#pragma once

#include <stdint.h>

///
/// \brief Check if a parameter is present in the command line.
///
/// \param parameter The parameter to check.
///
/// \return The index of the parameter in the command line, or -1 if it is not present.
///
int32_t doom_misc_check_parameter(const char* parameter);

///
/// \brief Check if a parameter is present in `params`.
///
/// \param params The parameters to check.
/// \param params_len The length of `params`.
/// \param parameter The parameter to check.
///
/// \return The index of the parameter in `params`, or -1 if it is not present.
///
int32_t doom_misc_check_parameter_ex(const char* parameter, char** params, int32_t params_len);
void doom_misc_add_parameter(const char* parameter);
void doom_misc_parse_command_line(char* cmd_start, char** argv, int32_t* out_num_args, int32_t* out_num_chars);
