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

///
/// \brief Add one parameter to the state's argv.
///
/// \param parameter The parameter to add.
///
void doom_misc_add_parameter(const char* parameter);

///
/// \brief Parse the command line and set up doom_state->argv.
///
/// \param cmd_start the beginning of the command-line string.
/// \param argv an optional pointer to an existing array that will be filled with the parsed args.
/// \param args an optional pointer to an existing array that will hold the args.
/// \param out_argv_len a required pointer to an int32_t that will be filled with the number of args in `argv`.
/// \param out_args_len a required pointer to an int32_t that will be filled with the number of characters in `args`.
///
void doom_misc_parse_command_line(char* cmd_start, char** argv, char* args, int32_t* out_argv_len,
                                  int32_t* out_args_len) __attribute__((nonnull(1, 4, 5)));

///
/// \brief Detect mismatching parameters, i.e. trying to record and play back at the same time.
///
void doom_misc_detect_clashing_parameters(void);
