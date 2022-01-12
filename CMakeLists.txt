cmake_minimum_required(VERSION 3.12...3.23)

project(
    cute-doom
    LANGUAGES C CXX
    VERSION 0.1.0
    DESCRIPTION "Cute Doom"
    HOMEPAGE_URL "https://github.com/Phytolizer/cute-doom"
)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(DeclareModule)

configure_file(
    "${PROJECT_SOURCE_DIR}/cmake/config.h.cin" "${PROJECT_BINARY_DIR}/config.h"
)

declare_module(
    nonstd
    KIND library
    SOURCES asprintf.c
            ctype.c
            qsort.c
            strdup.c
            stricmp.c
            strpbrk.c
            strspn.c
            strtok.c
)
declare_module(phyto_span KIND interface)
declare_module(
    phyto_collections
    KIND interface
    DEPENDS phyto_span nonstd
)
declare_module(
    phyto_string
    KIND library
    SOURCES string.c
    DEPENDS phyto_collections
)
declare_module(
    doom
    KIND library
    SOURCES init.c log/printf.c misc/argv.c misc/defaults.c state.c sys/system.c
    DEPENDS nonstd phyto_collections phyto_string
    INCLUDES "${PROJECT_BINARY_DIR}"
)
declare_module(
    cute-doom
    KIND executable
    SOURCES main.c
    DEPENDS doom
)
