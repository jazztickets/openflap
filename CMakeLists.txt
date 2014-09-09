cmake_minimum_required(VERSION 2.8)

project(openflap)

# add extra find modules
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

# windows only options
if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
	#include_directories("${PROJECT_SOURCE_DIR}/extlibs/include")
	#link_directories("${PROJECT_SOURCE_DIR}/extlibs/")

	# set output path
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/)
	add_definitions("-D_CRT_SECURE_NO_WARNINGS")
elseif(UNIX)
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -H")
	
	# set default build type
	if(NOT CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE Release)
	endif()
	
	# set output path
	set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
endif()

# libraries
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${SDL2_TTF_INCLUDE_DIRS})
include_directories(${SDL2_MIXER_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
include_directories("src/")

# source code
file(GLOB SRC_MAIN src/*.cpp src/*.h)
add_executable(${PROJECT_NAME} ${SRC_MAIN})

target_link_libraries(${CMAKE_PROJECT_NAME}
	${SDL2_LIBRARY}
	${SDL2_TTF_LIBRARIES}
	${SDL2_IMAGE_LIBRARIES}
	${SDL2_MIXER_LIBRARIES}
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
