# src/gridiron/CMakeLists.txt
set(GRIDIRON_SOURCES
    ${GRIDIRON_INCLUDE_ROOT}/exceptions.hpp
    ${GRIDIRON_SOURCE_ROOT}/gridiron.cpp
    ${GRIDIRON_INCLUDE_ROOT}/gridiron.hpp
    ${GRIDIRON_INCLUDE_ROOT}/property.hpp
${GRIDIRON_CONTROL_SOURCES}
)

add_subdirectory(controls)
add_subdirectory(html)
# do NOT include test or docs. they're included conditionally further up

add_library(gridiron-object OBJECT ${GRIDIRON_SOURCES})
add_library(gridiron-static STATIC $<TARGET_OBJECTS:gridiron-object>)
add_library(gridiron-shared SHARED $<TARGET_OBJECTS:gridiron-object>)

set_target_properties(gridiron-object PROPERTIES
    VERSION ${GRIDIRON_VERSION}
    SOVERSION ${GRIDIRON_VERSION_MAJOR}
    POSITION_INDEPENDENT_CODE 1
)

set_target_properties(gridiron-static PROPERTIES
    VERSION ${GRIDIRON_VERSION}
    SOVERSION ${GRIDIRON_VERSION_MAJOR}
    POSITION_INDEPENDENT_CODE 1
    LINKER_LANGUAGE CXX
    OUTPUT_NAME "gridiron"
    CXX_STANDARD 17
)

set_target_properties(gridiron-shared PROPERTIES
    VERSION ${GRIDIRON_VERSION}
    SOVERSION ${GRIDIRON_VERSION_MAJOR}
    POSITION_INDEPENDENT_CODE 1
    LINKER_LANGUAGE CXX
    OUTPUT_NAME "gridiron"
    CXX_STANDARD 17
)

target_compile_definitions(gridiron-object ${GRIDIRON_COMPILE_DEFS})
target_compile_definitions(gridiron-static ${GRIDIRON_COMPILE_DEFS})
target_compile_definitions(gridiron-shared ${GRIDIRON_COMPILE_DEFS})
target_include_directories(gridiron-object PRIVATE "${GRIDIRON_INCLUDE_ROOT}/../" "${HTMLCXX2_INCLUDE_ROOT}")
target_include_directories(gridiron-static PRIVATE "${GRIDIRON_INCLUDE_ROOT}/../" "${HTMLCXX2_INCLUDE_ROOT}")
target_include_directories(gridiron-shared PRIVATE "${GRIDIRON_INCLUDE_ROOT}/../" "${HTMLCXX2_INCLUDE_ROOT}")