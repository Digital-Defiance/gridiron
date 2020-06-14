set(GRIDIRON_TEST_LIBRARIES
    PRIVATE gridiron-static
)

set(GRIDIRON_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tests.cpp
)

add_executable(gridiron-test ${GRIDIRON_TEST_SOURCES})
target_sources(gridiron-test INTERFACE ${GRIDIRON_TEST_SOURCES})
target_include_directories(gridiron-test INTERFACE "${GRIDIRON_INCLUDE_ROOT}/../") # need to go one below gridiron directory
target_compile_definitions(gridiron-test ${GRIDIRON_COMPILE_DEFS})
target_link_libraries(gridiron-test INTERFACE ${GRIDIRON_TEST_LIBRARIES})
target_link_libraries(gridiron-test INTERFACE htmlcxx2)
add_test(project-tests gridiron-test)
