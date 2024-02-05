# path from the included OATPP lib/package (there's a trailing slash present)
set(OATPP_SWAGGER_RES_PATH "${OATPP_BASE_DIR}bin/oatpp-swagger/res")
#-- OATPP_BASE_DIR=/usr/local/include/oatpp-1.1.0/
## define path to swagger-ui res folder
add_definitions(-DOATPP_SWAGGER_RES_PATH="${OATPP_SWAGGER_RES_PATH}")
#message(STATUS "OATPP_BASE_DIR=${OATPP_BASE_DIR}")
#message(STATUS "OATPP_SWAGGER_RES_PATH=${OATPP_SWAGGER_RES_PATH}")

set(GRIDIRON_DEMO_SOURCE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(GRIDIRON_DEMO_SOURCES
    ${GRIDIRON_DEMO_SOURCE_ROOT}/App.cpp
    ${GRIDIRON_DEMO_SOURCE_ROOT}/AppComponent.hpp
    ${GRIDIRON_DEMO_SOURCE_ROOT}/controller/RootController.cpp
    ${GRIDIRON_DEMO_SOURCE_ROOT}/controller/RootController.hpp
)

add_subdirectory(gridiron)

set(GRIDIRON_DEMO_LIBRARIES
    PUBLIC oatpp::oatpp
    PUBLIC oatpp::oatpp-test
    PUBLIC oatpp::oatpp-swagger
    gridiron-static
)

get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

add_executable(gridiron-demo ${GRIDIRON_DEMO_SOURCES})
add_dependencies(gridiron-demo gridiron-static)
target_link_libraries(gridiron-demo ${GRIDIRON_DEMO_LIBRARIES})
#target_link_directories(gridiron-demo PUBLIC ${oatpp_LIBRARIES_DIRS})
target_include_directories(gridiron-demo SYSTEM PUBLIC ${oatpp_INCLUDE_DIRS})