cmake_minimum_required(VERSION 2.8)

# constants
add_definitions(-DGAME_VERSION="1.0.1")

# project name
project(openflap)

# add extra find modules
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

# add custom target to generate version.h
if(NOT NOVERSION)
	add_custom_target(
		version
		${CMAKE_COMMAND}
		-D SRC=${PROJECT_SOURCE_DIR}/cmake/version.h.in
		-D DST=${PROJECT_SOURCE_DIR}/src/version.h
		-P ${PROJECT_SOURCE_DIR}/cmake/version.cmake
	)
endif()

# mingw
if(WIN32)
	set(EXTRA_LIBS ${EXTRA_LIBS} winmm ws2_32)
endif()

# set default build type
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

# compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-result -Wno-unused-parameter -std=c++11 -msse -mfpmath=sse")

# set output path
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

# find libraries
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_TTF_INCLUDE_DIRS})
include_directories(${SDL2_MIXER_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
include_directories("src/")

# build binary
file(GLOB SRC_ALL src/*.cpp src/*.h)
add_executable(${PROJECT_NAME} ${SRC_ALL} src/resource.rc)

if(NOT NOVERSION)
add_dependencies(${CMAKE_PROJECT_NAME} version)
endif()

# link libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
	${SDL2_LIBRARY}
	${SDL2_TTF_LIBRARIES}
	${SDL2_IMAGE_LIBRARIES}
	${SDL2_MIXER_LIBRARIES}
	${EXTRA_LIBS}
)

# install
if(WIN32)
else()

	# linux installation
	install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION share/games/${CMAKE_PROJECT_NAME})
	install(DIRECTORY ${PROJECT_SOURCE_DIR}/working/audio DESTINATION share/games/${CMAKE_PROJECT_NAME})
	install(DIRECTORY ${PROJECT_SOURCE_DIR}/working/font DESTINATION share/games/${CMAKE_PROJECT_NAME})
	install(DIRECTORY ${PROJECT_SOURCE_DIR}/working/image DESTINATION share/games/${CMAKE_PROJECT_NAME})
	install(FILES ${PROJECT_SOURCE_DIR}/deployment/${CMAKE_PROJECT_NAME}.png DESTINATION share/pixmaps)
	install(FILES ${PROJECT_SOURCE_DIR}/deployment/${CMAKE_PROJECT_NAME}.desktop DESTINATION share/applications)
	install(FILES ${PROJECT_SOURCE_DIR}/LICENSE DESTINATION share/doc/${CMAKE_PROJECT_NAME})
	install(FILES ${PROJECT_SOURCE_DIR}/README DESTINATION share/doc/${CMAKE_PROJECT_NAME})

	# generate the script to launch the program
	configure_file(${PROJECT_SOURCE_DIR}/deployment/${CMAKE_PROJECT_NAME} ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles)
	install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${CMAKE_PROJECT_NAME} DESTINATION bin)
endif()
