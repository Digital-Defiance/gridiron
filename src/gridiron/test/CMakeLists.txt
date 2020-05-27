add_executable(gridiron-test tests.cpp)
target_link_libraries(gridiron-test gridiron)
add_test(project-tests gridiron-test)
