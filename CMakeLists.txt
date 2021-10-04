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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -municode -std=c++17 -Wl,-subsystem,windows")

#use include,lib and source folder
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${CMAKE_CURRENT_LIST_DIR}/lib")
include_directories("${CMAKE_CURRENT_LIST_DIR}/include" "${CMAKE_CURRENT_LIST_DIR}/source")

#sciter - compile files
if(WIN32)
	add_custom_Target(
		uicompile
		COMMAND uicompile.bat
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
	ADD_DEPENDENCIES(GIDE uicompile)
endif()

#use libraries
#find_library(GLEW_LIB glew32)
#target_link_libraries(GIDE PRIVATE "${GLEW_LIB}" "opengl32" "${SFML_NETWORK_LIB}" "${SFML_GRAPHICS_LIB}" "${SFML_SYSTEM_LIB}" "${SFML_WINDOW_LIB}" )
target_link_libraries(GIDE PRIVATE)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
