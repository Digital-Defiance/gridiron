# src/user_controls/CMakeLists.txt
add_library(gridironUserControls "")
target_sources(gridironUserControls
    PUBLIC
        userControl.hpp
)
add_dependencies(gridironUserControls gridironControls)
target_link_libraries(gridironUserControls gridiron gridironControls)
set_target_properties(gridironUserControls PROPERTIES LINKER_LANGUAGE CXX)

# CONFIGURE GRIDIRON
target_compile_definitions(gridiron PUBLIC GRIDIRON_XHTML_NS=${GRIDIRON_XHTML_NS})
target_compile_definitions(gridiron PUBLIC GRIDIRON_HTML_DOCROOT="${GRIDIRON_HTML_DOCROOT}")