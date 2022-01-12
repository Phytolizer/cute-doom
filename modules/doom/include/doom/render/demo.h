#pragma once

#include <phyto/string/string.h>

enum
{
    ///
    /// \brief The maximum smooth demo playback factor.
    ///
    doom_demo_smoothturnsfactor_max = 16,
};

PHYTO_COLLECTIONS_DYNAMIC_ARRAY_DECL(doom_demo_patterns_list, phyto_string_t);

///
/// \brief Construct a new demo patterns list.
///
doom_demo_patterns_list_t doom_demo_patterns_list_new(void);
