cmake_minimum_required(VERSION 2.8)

project(openflap)

# add extra find modules
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

# libraries
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(/usr/local/include/SDL2)
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_TTF_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
include_directories("src/")

# source code
file(GLOB SRC_MAIN src/*.cpp src/*.h)
#aux_source_directory(src SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_MAIN})

target_link_libraries(${CMAKE_PROJECT_NAME}
	${SDL2_LIBRARY}
	${SDL2_TTF_LIBRARIES}
	${SDL2_IMAGE_LIBRARIES}
)
