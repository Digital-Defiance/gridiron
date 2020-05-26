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
target_link_libraries(gridiron ${HTMLCXX_LIBRARIES})
add_subdirectory(controls)

# CONFIGURE GRIDIRON
target_compile_definitions(gridiron PUBLIC GRIDIRON_XHTML_NS=${GRIDIRON_XHTML_NS})
target_compile_definitions(gridiron PUBLIC GRIDIRON_HTML_DOCROOT="${GRIDIRON_HTML_DOCROOT}")