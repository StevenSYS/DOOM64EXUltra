cmake_minimum_required(VERSION 3.12)
set(PROGRAM_NAME "DOOM 64 EX Ultra")

project(DOOM64EXUltra LANGUAGES C CXX VERSION 1.0)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTiIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

option(BUILD_SHARED_LIBS "Build the libraries as shared libraries" OFF)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunknown-pragmas -fwrapv -MD")
set(CMAKE_C_FLAGS_DEBUG "-g -Wall")
set(CMAKE_C_FLAGS_RELEASE "-s -O2")

add_subdirectory(src/submodules)

file(
	GLOB SOURCE_FILES
	"src/*.c"
	"src/menus/*.c"
)

find_package(SDL3 REQUIRED)
find_package(SDL3_mixer REQUIRED)
find_package(PNG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(ZLIB REQUIRED)
include_directories(
	${libpng_INCLUDE_DIRS}
	${OpenGL_INCLUDE_DIRS}
	"src"
	"src/submodules"
)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/src/progInfo.h.in
	${CMAKE_CURRENT_SOURCE_DIR}/src/progInfo.h
	@ONLY
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set_property(TARGET ${PROJECT_NAME} PROPERTY BUILD_RPATH ".")

target_link_libraries(
	${PROJECT_NAME}
	SDL3::SDL3
	SDL3_mixer::SDL3_mixer
	portmidi
	png
	${OPENGL_LIBRARY}
	z
	m
	cimgui
)