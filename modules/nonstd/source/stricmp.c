#include "nonstd/stricmp.h"

#include <ctype.h>

int32_t nonstd_stricmp(const char* a, const char* b) {
    while (*a && *b) {
        if (tolower(*a) != tolower(*b)) {
            return *a - *b;
        }
        a++;
        b++;
    }
    return tolower(*a) - tolower(*b);
}
