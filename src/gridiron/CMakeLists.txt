# src/gridiron/CMakeLists.txt
set(GRIDIRON_SOURCES
    ${GRIDIRON_INCLUDE_ROOT}/exceptions.hpp
    ${GRIDIRON_INCLUDE_ROOT}/gridiron.hpp
    ${GRIDIRON_SOURCE_ROOT}/gridiron.cpp
    ${GRIDIRON_SOURCE_ROOT}/Node.hpp
    ${GRIDIRON_SOURCE_ROOT}/ParserDom.hpp
    ${GRIDIRON_INCLUDE_ROOT}/properties.hpp
    ${GRIDIRON_SOURCE_ROOT}/AttributeMappedProperty.hpp
    ${GRIDIRON_SOURCE_ROOT}/ContentMappedProperty.hpp
    ${GRIDIRON_SOURCE_ROOT}/ComparedAttributeProperty.hpp
${GRIDIRON_CONTROL_SOURCES}
)

add_subdirectory(controls)
add_subdirectory(html)
# do NOT include test or docs. they're included conditionally further up

add_library(gridiron-object OBJECT ${GRIDIRON_SOURCES})
add_library(gridiron-static STATIC $<TARGET_OBJECTS:gridiron-object>)
add_library(gridiron-shared SHARED $<TARGET_OBJECTS:gridiron-object>)
target_sources(gridiron-object PUBLIC ${GRIDIRON_SOURCES})
target_sources(gridiron-static PUBLIC ${GRIDIRON_SOURCES})
target_sources(gridiron-shared PUBLIC ${GRIDIRON_SOURCES})
target_include_directories(gridiron-object PUBLIC "${GRIDIRON_INCLUDE_ROOT}/../")
target_include_directories(gridiron-static PUBLIC "${GRIDIRON_INCLUDE_ROOT}/../")
target_include_directories(gridiron-shared PUBLIC "${GRIDIRON_INCLUDE_ROOT}/../")

add_dependencies(gridiron-object htmlcxx2)
target_link_libraries(gridiron-object PUBLIC htmlcxx2)
target_link_libraries(gridiron-static PUBLIC htmlcxx2)
target_link_libraries(gridiron-shared PUBLIC htmlcxx2)

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