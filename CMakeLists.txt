# root CMakeLists.txt
set(CMAKE_VERBOSE_MAKEFILE True)
cmake_minimum_required(VERSION 3.15)
cmake_policy(VERSION 3.15)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(GRIDIRON_VERSION "0.1.10")
set(GRIDIRON_VERSION_MAJOR "0")
set(GRIDIRON_XHTML_NS "\"gi\"")
set(GRIDIRON_HTML_DOCROOT "html")
set(GRIDIRON_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(GRIDIRON_SOURCE_ROOT ${GRIDIRON_ROOT}/src/gridiron)
set(GRIDIRON_INCLUDE_ROOT ${GRIDIRON_ROOT}/include/gridiron)

project(gridiron
    LANGUAGES CXX
    VERSION ${GRIDIRON_VERSION}
#    SOVERSION ${GRIDIRON_VERSION_MAJOR}
    DESCRIPTION "GridIron C++ framework"
)
find_package(HtmlCxx REQUIRED)
find_package(oatpp          1.1.0 REQUIRED)
#find_library(oatpp          1.1.0 REQUIRED)
find_package(oatpp-swagger  1.1.0 REQUIRED)

include_directories(SYSTEM PUBLIC ${HTMLCXX_INCLUDE_DIRS})
include_directories(SYSTEM PUBLIC ${oatpp_INCLUDE_DIRS})

set(CMAKE_REQUIRED_LIBRARIES ${HTMLCXX_LIBRARY})

include_directories(SYSTEM PUBLIC include)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # CONFIGURE GRIDIRON
    add_compile_definitions(PUBLIC GRIDIRON_XHTML_NS=${GRIDIRON_XHTML_NS})
    add_compile_definitions(PUBLIC GRIDIRON_HTML_DOCROOT="${GRIDIRON_HTML_DOCROOT}")

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
    enable_testing()
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

add_subdirectory(src)