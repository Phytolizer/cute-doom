#pragma once

#include <phyto/string/string.h>
#include <stdint.h>

PHYTO_COLLECTIONS_DYNAMIC_ARRAY_DECL(doom_misc_parameters, phyto_string_t);

doom_misc_parameters_t doom_misc_parameters_new(void);

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
/// \param parameter The parameter to check.
/// \param params The parameters to look within.
///
/// \return The index of the parameter in `params`, or -1 if it is not present.
///
int32_t doom_misc_check_parameter_ex(const char* parameter, doom_misc_parameters_t params);

///
/// \brief Add one parameter to the state's argv.
///
/// \param parameter The parameter to add.
///
void doom_misc_add_parameter(const char* parameter);

///
/// \brief Parse the command line and set up doom_state->argv.
///
/// \param cmd_start The beginning of the command-line string.
/// \param out_params The parameters found in the command line.
///
void doom_misc_parse_command_line(char* cmd_start, doom_misc_parameters_t* out_params) __attribute__((nonnull(1, 2)));

///
/// \brief Detect mismatching parameters, i.e. trying to record and play back at the same time.
///
void doom_misc_detect_clashing_parameters(void);
