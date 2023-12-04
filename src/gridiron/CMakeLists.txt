# src/gridiron/CMakeLists.txt
set(GRIDIRON_SOURCES
    ${GRIDIRON_INCLUDE_ROOT}/exceptions.hpp
    ${GRIDIRON_SOURCE_ROOT}/gridiron.cpp
    ${GRIDIRON_INCLUDE_ROOT}/gridiron.hpp
    ${GRIDIRON_INCLUDE_ROOT}/tag.hpp
    ${GRIDIRON_SOURCE_ROOT}/tag.cpp
${GRIDIRON_CONTROL_SOURCES}
)

add_subdirectory(controls)
add_subdirectory(html)
# do NOT include test or docs. they're included conditionally further up

add_library(gridiron-static STATIC ${GRIDIRON_SOURCES})
add_library(gridiron-shared SHARED ${GRIDIRON_SOURCES})
set_target_properties(gridiron-shared PROPERTIES
    VERSION ${GRIDIRON_VERSION}
    SOVERSION ${GRIDIRON_VERSION_MAJOR}
)

set_target_properties(gridiron-static PROPERTIES
    LINKER_LANGUAGE CXX
    OUTPUT_NAME "gridiron"
)
set_target_properties(gridiron-shared PROPERTIES
    LINKER_LANGUAGE CXX
    OUTPUT_NAME "gridiron"
)

set_property(TARGET gridiron-static PROPERTY CXX_STANDARD 17)
set_property(TARGET gridiron-shared PROPERTY CXX_STANDARD 17)

## link libs
#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
