set(GRIDIRON_TEST_LIBRARIES
    PRIVATE gridiron-static
)

add_executable(gridiron-test tests.cpp)
target_include_directories(gridiron-test PRIVATE "${GRIDIRON_INCLUDE_ROOT}/../") # need to go one below gridiron directory
target_compile_definitions(gridiron-test ${GRIDIRON_COMPILE_DEFS})
target_link_libraries(gridiron-test ${GRIDIRON_TEST_LIBRARIES})
add_test(project-tests gridiron-test)
