cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

project(FluxLumina VERSION 0.3.0 LANGUAGES CXX)

# Set the language for the project
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#Set relevant cmake policies
cmake_policy(SET CMP0135 NEW) #
cmake_policy(SET CMP0074 NEW)
set(Boost_NO_WARN_NEW_VERSIONS 1)

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp
	${CMAKE_SOURCE_DIR}/res/shaders/*)

# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp)

# Configure assets header file
configure_file(src/helpers/RootDir.h.in src/helpers/RootDir.h)
include_directories(
	${CMAKE_BINARY_DIR}/src
)

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# Fetch dependencies remotely
include(FetchContent)

# Supress dev warnings on subprojects
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

######################################## GLFW
FetchContent_Declare(
  glfw
  GIT_REPOSITORY 	https://github.com/glfw/glfw.git
  GIT_TAG        	3.3.2
)
FetchContent_MakeAvailable(glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
message(STATUS "GLFW -> ${glfw_SOURCE_DIR}")

######################################## GLAD
FetchContent_Declare(
  glad
  GIT_REPOSITORY 	https://github.com/Dav1dde/glad.git
  GIT_TAG 			v0.1.34
)
set(GLAD_PROFILE 		    "core"	    CACHE STRING "OpenGL profile")
set(GLAD_API            "gl=4.6"    CACHE STRING "API type/version pair")
set(GLAD_GENERATOR 		  "c"	        CACHE STRING "Language to generate the binding for")

FetchContent_MakeAvailable(glad)

target_link_libraries(${PROJECT_NAME} PRIVATE glad)
message(STATUS "GLAD -> ${glad_SOURCE_DIR}")

######################################## STB_IMAGE
FetchContent_Declare(
  stb
  GIT_REPOSITORY 	https://github.com/nothings/stb.git
  GIT_TAG 			master
  CONFIGURE_COMMAND ""
  BUILD_COMMAND 	""
)
FetchContent_MakeAvailable(stb)
target_include_directories(${PROJECT_NAME} PRIVATE "${stb_SOURCE_DIR}")
message(STATUS "STB -> ${stb_SOURCE_DIR}")

######################################## GLM
FetchContent_Declare(
  glm
  GIT_REPOSITORY 	https://github.com/g-truc/glm.git
  GIT_TAG 			0.9.9.0
  CONFIGURE_COMMAND ""
  BUILD_COMMAND 	""
)
FetchContent_MakeAvailable(glm)
target_include_directories(${PROJECT_NAME} PRIVATE "${glm_SOURCE_DIR}")
message(STATUS "GLM -> ${glm_SOURCE_DIR}")

######################################## ASSIMP
FetchContent_Declare(
  assimp
  GIT_REPOSITORY 	https://github.com/assimp/assimp.git
  GIT_TAG        	v5.3.0
)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "Disable Assimp's export functionality")
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "Disable Assimp's export functionality")
FetchContent_MakeAvailable(assimp)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp)
message(STATUS "ASSIMP -> ${assimp_SOURCE_DIR}")

######################################## BOOST
# Boost is different than the others, we need to build it
message(STATUS "Downloading and building boost library sources. This will take some time...")
set(FETCHCONTENT_QUIET FALSE) # Prints the output of the build process to terminal
set(DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_Declare(
    Boost
    URL https://github.com/boostorg/boost/releases/download/boost-1.84.0/boost-1.84.0.7z # downloading a zip release speeds up the download
    USES_TERMINAL_DOWNLOAD TRUE 
    GIT_PROGRESS TRUE   
    DOWNLOAD_NO_EXTRACT FALSE
)
set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
set(BOOST_INCLUDE_LIBRARIES log date_time filesystem uuid)
set(BOOST_ENABLE_CMAKE ON)
set(BOOST_ROOT ${boost_BUILD_DIR})
set(Boost_NO_SYSTEM_PATHS TRUE)
FetchContent_MakeAvailable(Boost)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::filesystem Boost::log Boost::log_setup Boost::date_time Boost::uuid)

######################################## END OF EXTERNAL DEPENDENCIES

# Restore dev warnings
set(CMAKE_WARN_DEPRECATED ON CACHE BOOL "" FORCE)

# OpenGL
find_package(OpenGL REQUIRED)

# Define the include DIRs
include_directories(
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/include"
)

# Link the dependency libraries to the target
target_link_libraries(${PROJECT_NAME} PRIVATE opengl32)

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${HEADER_FILES} ${SOURCE_FILES})
