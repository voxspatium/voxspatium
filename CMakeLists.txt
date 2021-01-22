cmake_minimum_required(VERSION 3.5)
project(voxspatium)

# Modules
#SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules ${CMAKE_MODULE_PATH})

# Enable compiler warning.
string(APPEND CMAKE_CXX_FLAGS " -Wall")
string(APPEND CMAKE_CXX_FLAGS " -Werror")
string(APPEND CMAKE_CXX_FLAGS " -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-reorder")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-parameter")

# Sources and headers
include_directories(${PROJECT_SOURCE_DIR}/src)
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*)

# Executable output
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# System-specific options
# TODO: this
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(VOXSPATIUM_EXECUTABLE "voxspatium")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(VOXSPATIUM_EXECUTABLE "voxspatium")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(VOXSPATIUM_EXECUTABLE "voxspatium.exe")
endif()

add_executable(${VOXSPATIUM_EXECUTABLE} ${SOURCES})

# Include GL
set(OpenGL_GL_PREFERENCE "LEGACY")
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(${VOXSPATIUM_EXECUTABLE} ${OPENGL_LIBRARIES})

# Include GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_PATH})
target_link_libraries(${VOXSPATIUM_EXECUTABLE} ${GLEW_LIBRARIES})

# Include SDL
INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
target_link_libraries(${VOXSPATIUM_EXECUTABLE} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
