cmake_minimum_required(VERSION 3.0.0)
project(GIDE VERSION 0.1.0)

include(CTest)
enable_testing()

#recursive files
file(GLOB_RECURSE SRCFILES CONFIGURE_DEPENDS "source/*.cpp" "source/*.h" "source/*.hpp")
add_executable(GIDE ${SRCFILES})

#use c++17
set(CMAKE_CXX_STANDARD 17)
target_compile_features(GIDE PRIVATE cxx_std_17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

#use include,lib and source folder
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${CMAKE_CURRENT_LIST_DIR}/lib")
include_directories("${CMAKE_CURRENT_LIST_DIR}/include" "${CMAKE_CURRENT_LIST_DIR}/source")

#use libraries
find_library(SDL_MAIN_LIB SDL2main)
find_library(SDL_LIB SDL2)
target_link_libraries(GIDE PRIVATE "${SDL_MAIN_LIB}" "${SDL_LIB}")

#set macros
add_compile_definitions(SDL_MAIN_HANDLED)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
