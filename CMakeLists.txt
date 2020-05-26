# root CMakeLists.txt
set(CMAKE_VERBOSE_MAKEFILE True)
cmake_minimum_required(VERSION 3.15)
cmake_policy(VERSION 3.15)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

project(gridiron
    VERSION 0.1.10
    DESCRIPTION "GridIron C++ framework"
    LANGUAGES CXX
)

set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.72 REQUIRED COMPONENTS
        date_time filesystem iostreams)

include_directories(SYSTEM ${BOOST_INCLUDE_DIR})
find_package(HtmlCxx REQUIRED)
include_directories(SYSTEM ${HTMLCXX_INCLUDE_DIRS})

find_package(oatpp          1.1.0 REQUIRED)
find_library(oatpp          1.1.0 REQUIRED)
find_package(oatpp-swagger  1.1.0 REQUIRED)
include_directories(SYSTEM ${oatpp_INCLUDE_DIRS})

link_libraries(
        Boost::date_time
        Boost::filesystem
        Boost::iostreams
)

set(CMAKE_REQUIRED_INCLUDES ${BOOST_INCLUDE_DIR})
set(CMAKE_REQUIRED_LIBRARIES ${HTMLCXX_LIBRARY})

include_directories(SYSTEM include)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    # Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)

    # Let's ensure -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Let's nicely support folders in IDE's
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Testing only available if this is the main app
    # Note this needs to be done in the main CMakeLists
    # since it calls enable_testing, which must be in the
    # main CMakeLists.
    include(CTest)

    # Docs only available if this is the main app
    find_package(Doxygen)
    if(Doxygen_FOUND)
        add_subdirectory(src/gridiron/docs)
    else()
        message(STATUS "Doxygen not found, not building docs")
    endif()
endif()


# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND BUILD_TESTING)
    add_subdirectory(src/gridiron/test)
endif()

add_subdirectory(src/gridiron)
add_library(gridiron-oatpp-lib
        src/AppComponent.hpp
        src/SwaggerComponent.hpp
        src/controller/UserController.hpp
        src/db/Database.cpp
        src/db/Database.hpp
        src/db/model/User.hpp
        src/dto/UserDto.hpp
)

add_dependencies(gridiron-oatpp-lib gridiron)

## include directories

target_include_directories(gridiron-oatpp-lib
    PUBLIC
        src
)


## link libs


target_link_libraries(gridiron-oatpp-lib
    PUBLIC oatpp::oatpp
    PUBLIC oatpp::oatpp-test
    PUBLIC oatpp::oatpp-swagger
    gridiron
    gridironControls
    gridironUserControls
)

## define path to swagger-ui res folder

add_definitions(-DOATPP_SWAGGER_RES_PATH="${OATPP_BASE_DIR}/bin/oatpp-swagger/res")

## add executables

add_executable(crud-exe src/App.cpp)
add_dependencies(crud-exe gridiron-oatpp-lib)
target_link_libraries(crud-exe gridiron-oatpp-lib)

add_executable(crud-test test/tests.cpp)
target_link_libraries(crud-test gridiron-oatpp-lib)

enable_testing()
add_test(project-tests crud-test)
