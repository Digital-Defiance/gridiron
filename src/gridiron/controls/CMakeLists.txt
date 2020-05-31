

# src/gridiron/controls/CMakeLists.txt
set(GRIDIRON_CONTROLS_SOURCE_ROOT ${GRIDIRON_SOURCE_ROOT}/controls)
set(GRIDIRON_CONTROLS_INCLUDE_ROOT ${GRIDIRON_INCLUDE_ROOT}/controls)

# create an initial list, counting controls in this list
set(GRIDIRON_CONTROL_SOURCES
    ${GRIDIRON_CONTROLS_SOURCE_ROOT}/control.cpp
    ${GRIDIRON_CONTROLS_INCLUDE_ROOT}/control.hpp
    ${GRIDIRON_CONTROLS_SOURCE_ROOT}/page.cpp
    ${GRIDIRON_CONTROLS_INCLUDE_ROOT}/page.hpp
    ${GRIDIRON_CONTROLS_SOURCE_ROOT}/value.cpp
    ${GRIDIRON_CONTROLS_INCLUDE_ROOT}/value.hpp
)

# add all ui subdirectories
add_subdirectory(ui)