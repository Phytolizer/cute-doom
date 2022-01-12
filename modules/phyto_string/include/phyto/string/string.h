#pragma once

#include <phyto/collections/dynamic_array.h>
#include <stdint.h>
#include <stdio.h>

PHYTO_COLLECTIONS_DYNAMIC_ARRAY_DECL(phyto_string, char);

void phyto_string_print_nosep(phyto_string_t str, FILE* fp);
int32_t phyto_string_compare(phyto_string_t a, phyto_string_t b);

phyto_string_t phyto_string_new(void);
phyto_string_t phyto_string_from_c(const char* c);

phyto_string_span_t phyto_string_span_from_c(const char* c);
