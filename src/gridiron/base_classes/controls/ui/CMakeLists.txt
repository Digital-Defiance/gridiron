# src/gridiron/base_classes/controls/ui/CMakeLists.txt
add_library(gridironControls "")
target_sources(gridironControls
    PUBLIC
        label.cpp ${CMAKE_SOURCE_DIR}/include/gridiron/base_classes/controls/ui/label.hpp
)
add_dependencies(gridironControls gridiron)
target_link_libraries(gridironControls gridiron)