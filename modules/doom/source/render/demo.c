#include "doom/render/demo.h"

PHYTO_COLLECTIONS_DYNAMIC_ARRAY_IMPL(doom_demo_patterns_list, phyto_string_t);

static const doom_demo_patterns_list_callbacks_t doom_demo_patterns_callbacks = {
    .free_cb = phyto_string_free,
    .compare_cb = phyto_string_compare,
    .copy_cb = phyto_string_copy,
    .print_cb = phyto_string_print_nosep,
};

doom_demo_patterns_list_t doom_demo_patterns_list_new(void) {
    return doom_demo_patterns_list_init(&doom_demo_patterns_callbacks);
}
