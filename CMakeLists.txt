cmake_minimum_required(VERSION 3.0.0)
project(GIDE VERSION 0.1.0)

include(CTest)
enable_testing()


#recursive files
file(GLOB_RECURSE SRCFILES CONFIGURE_DEPENDS "source/*.cpp" "source/*.h" "source/*.hpp" )

add_executable(GIDE ${SRCFILES})

#use c++17
set(CMAKE_CXX_STANDARD 17)
target_compile_features(GIDE PRIVATE cxx_std_17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

#use include,lib and source folder
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${CMAKE_CURRENT_LIST_DIR}/lib")
include_directories("${CMAKE_CURRENT_LIST_DIR}/include" "${CMAKE_CURRENT_LIST_DIR}/source")

#use libraries
find_library(GLEW_LIB glew32)
find_library(SFML_MAIN_LIB sfml-main)
find_library(SFML_NETWORK_LIB sfml-network)
find_library(SFML_GRAPHICS_LIB sfml-graphics)
find_library(SFML_SYSTEM_LIB sfml-system)
find_library(SFML_WINDOW_LIB sfml-window)
find_library(SFML_AUDIO_LIB sfml-audio)

target_link_libraries(GIDE PRIVATE "${GLEW_LIB}" "opengl32" "${SFML_MAIN_LIB}" "${SFML_NETWORK_LIB}" "${SFML_GRAPHICS_LIB}" "${SFML_SYSTEM_LIB}" "${SFML_WINDOW_LIB}" "${SFML_AUDIO_LIB}" )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
