cmake_minimum_required(VERSION 2.8)

project(openflap)

# add extra find modules
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

# libraries
find_package(SDL2 REQUIRED)
include_directories(/usr/local/include/SDL2)
include_directories(${SDL2_INCLUDE_DIR})

# source code
aux_source_directory(src SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${CMAKE_PROJECT_NAME}
	${SDL2_LIBRARY}
	/usr/lib/x86_64-linux-gnu/libSDL2_image.so
	/usr/local/lib/libSDL2_ttf-2.0.so.0
)
