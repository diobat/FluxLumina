cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(PlusBetterGooder)

set(CMAKE_CXX_STANDARD 17)

#Set relevant cmake policies
cmake_policy(SET CMP0074 NEW)
set(Boost_NO_WARN_NEW_VERSIONS 1)

#Add other CMakeLists

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp
	${CMAKE_SOURCE_DIR}/res/shaders/*)

# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp)

# Add .lib files
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Configure assets header file
configure_file(src/helpers/RootDir.h.in src/helpers/RootDir.h)
include_directories(
	${CMAKE_BINARY_DIR}/src
)

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# We need a CMAKE_DIR with some code to find external dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# OpenGL
find_package(OpenGL REQUIRED)

# GLM
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")

# GLFW
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

# ASSIMP
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")

# STB_IMAGE
add_library(STB_IMAGE "thirdparty/stb_image.cpp")

# GLAD
add_library(GLAD "thirdparty/glad.c")

#Boost
set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 

# Set Boost root folder as BOOST_ROOT environment variable or this will fail
find_package(Boost 1.82.0 REQUIRED COMPONENTS log date_time filesystem)
message(STATUS "Found BOOST in ${Boost_INCLUDE_DIR}")
#message(STATUS "Boost libraries: ${Boost_LIBRARIES}")

# Put all libraries into a variable
set(LIBS glfw3 opengl32 assimp STB_IMAGE GLAD ${Boost_LIBRARIES})

# Define the include DIRs
include_directories(
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/include"
	${Boost_INCLUDE_DIRS}
)

# Define the link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS} ${Boost_LIBRARIES})

#set_property(GLOBAL PROPERTY USE_FOLDERS ON)
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${HEADER_FILES} ${SOURCE_FILES})

# Copy dlls
if(WIN32)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${PROJECT_SOURCE_DIR}/dlls"
		$<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()
