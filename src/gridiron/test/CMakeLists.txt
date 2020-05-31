set(GRIDIRON_TEST_LIBRARIES
    PUBLIC oatpp::oatpp
    PUBLIC oatpp::oatpp-test
    PUBLIC oatpp::oatpp-swagger
    gridiron-static
)

add_executable(gridiron-test tests.cpp)
add_dependencies(gridiron-test gridiron-static)
target_include_directories(gridiron-test SYSTEM PUBLIC ${oatpp_INCLUDE_DIRS})
target_link_libraries(gridiron-test ${GRIDIRON_TEST_LIBRARIES})
add_test(project-tests gridiron-test)
