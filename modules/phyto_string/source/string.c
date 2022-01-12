#include "phyto/string/string.h"

#include <stdio.h>

PHYTO_COLLECTIONS_DYNAMIC_ARRAY_IMPL(phyto_string, char);

int32_t charcmp(char a, char b) {
    return (a > b) - (a < b);
}

void charprn(char c, FILE* fp) {
    fputc(c, fp);
}

static const phyto_string_callbacks_t phyto_string_callbacks = {
    .compare_cb = charcmp,
    .print_cb = charprn,
};

void phyto_string_print_nosep(phyto_string_t str, FILE* fp) {
    for (size_t i = 0; i < str.size; i++) {
        fputc(str.data[i], fp);
    }
}

int32_t phyto_string_compare(phyto_string_t a, phyto_string_t b) {
    if (a.size != b.size) {
        return (int32_t)a.size - (int32_t)b.size;
    }

    for (size_t i = 0; i < a.size; i++) {
        if (a.data[i] != b.data[i]) {
            return a.data[i] - b.data[i];
        }
    }

    return 0;
}

phyto_string_t phyto_string_new(void) {
    return phyto_string_init(&phyto_string_callbacks);
}

phyto_string_t phyto_string_from_c(const char* c) {
    return phyto_string_init_from_span(&phyto_string_callbacks, phyto_string_span_from_c(c));
}

phyto_string_span_t phyto_string_span_from_c(const char* c) {
    return phyto_string_span_from_array(c, strlen(c));
}
