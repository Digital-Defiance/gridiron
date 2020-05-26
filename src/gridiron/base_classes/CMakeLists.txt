# src/gridiron/base_classes/CmakeLists.txt
add_library(gridiron SHARED "")
set_target_properties(gridiron PROPERTIES VERSION ${PROJECT_VERSION})
target_sources(gridiron
    PUBLIC
        control.cpp ${CMAKE_SOURCE_DIR}/include/gridiron/base_classes/controls/control.hpp
        ${CMAKE_SOURCE_DIR}/include/gridiron/base_classes/exceptions.hpp
        gridiron.cpp ${CMAKE_SOURCE_DIR}/include/gridiron/base_classes/gridiron.hpp
        page.cpp ${CMAKE_SOURCE_DIR}/include/gridiron/base_classes/page.hpp
)
message(STATUS ${HTMLCXX_LIBRARIES})
target_link_libraries(gridiron ${HTMLCXX_LIBRARIES})
add_subdirectory(controls)