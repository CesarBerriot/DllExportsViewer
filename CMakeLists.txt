cmake_minimum_required(VERSION 3.27)

# project settings
project(DllExportsViewer)
set(CMAKE_CXX_STANDARD 17)
include(cmake/sources.cmake)

# disable the console for non-debug build configurations
set(CMAKE_WIN32_EXECUTABLE $<IF:$<STREQUAL:${CMAKE_BUILD_TYPE},Debug>,OFF,ON>)

# import libraries
include(cmake/import_qt6.cmake)
include(cmake/import_jansson.cmake)

# microsoft symbol loader import
add_compile_options(-limagehlp -Wall -Wno-sign-compare -Wno-unused-variable)
link_libraries(Dbghelp.lib)

# main executable
add_compile_options(--include src/macros/macros.hpp)
include_directories(. src)
add_executable(DllExportsViewer src/main.cpp ${SOURCES})

# unit tests
option(BUILD_TESTS "Build unit tests." ON)
if(BUILD_TESTS STREQUAL ON)
	enable_testing()
	include(tests/tests.cmake)
endif()

# embed qt resources
include(cmake/embed_qt_resources/include_all.cmake)

# copy shared library files to the build folder
include(cmake/copy_qt6_dlls_to_bin.cmake)
include(cmake/copy_mingw_dlls_to_bin.cmake)