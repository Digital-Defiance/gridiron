add_executable(gridiron-test tests.cpp)
target_link_libraries(gridiron-test gridiron-oatpp-lib)
enable_testing()
add_test(project-tests gridiron-test)
