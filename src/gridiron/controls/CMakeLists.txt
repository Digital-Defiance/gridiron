

# src/gridiron/controls/CMakeLists.txt
set(GRIDIRON_CONTROLS_SOURCE_ROOT ${GRIDIRON_SOURCE_ROOT}/controls)
set(GRIDIRON_CONTROLS_INCLUDE_ROOT ${GRIDIRON_INCLUDE_ROOT}/controls)

# create an initial list, counting controls in this list
set(GRIDIRON_CONTROL_SOURCES
    ${GRIDIRON_CONTROLS_SOURCE_ROOT}/Control.cpp
    ${GRIDIRON_CONTROLS_INCLUDE_ROOT}/Control.hpp
    ${GRIDIRON_CONTROLS_SOURCE_ROOT}/Page.cpp
    ${GRIDIRON_CONTROLS_INCLUDE_ROOT}/Page.hpp
    ${GRIDIRON_CONTROLS_SOURCE_ROOT}/controls.hpp // meta includer
)

# add all UI subdirectories
add_subdirectory(ui)