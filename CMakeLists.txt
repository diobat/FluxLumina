cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

project(FluxLumina VERSION 0.3.0 LANGUAGES CXX C)

# Set the language for the project
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#Set relevant cmake policies
cmake_policy(SET CMP0135 NEW) #
cmake_policy(SET CMP0074 NEW)
set(Boost_NO_WARN_NEW_VERSIONS 1)

# Configure assets header file
configure_file(src/helpers/RootDir.h.in src/helpers/RootDir.hpp)

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

  # Add header files
file(GLOB_RECURSE HEADER_FILES 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
)

# Define the executable
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})

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
set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
FetchContent_MakeAvailable(glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
install(TARGETS glfw EXPORT ${PROJECT_NAME}Config)

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

target_link_libraries(${PROJECT_NAME} PUBLIC glad)
install(TARGETS glad EXPORT ${PROJECT_NAME}Config)

######################################## STB_IMAGE
FetchContent_Declare(
  stb
  GIT_REPOSITORY 	https://github.com/nothings/stb.git
  GIT_TAG 			master
  CONFIGURE_COMMAND ""
  BUILD_COMMAND 	""
)
FetchContent_MakeAvailable(stb)
target_include_directories(${PROJECT_NAME} PUBLIC 
$<BUILD_INTERFACE:${stb_SOURCE_DIR}>)

######################################## GLM
FetchContent_Declare(
  glm
  GIT_REPOSITORY 	https://github.com/g-truc/glm.git
  GIT_TAG 			0.9.9.0
  CONFIGURE_COMMAND ""
  BUILD_COMMAND 	""
)
FetchContent_MakeAvailable(glm)
target_link_libraries(${PROJECT_NAME} PUBLIC glm)

######################################## ASSIMP
FetchContent_Declare(
  assimp
  GIT_REPOSITORY 	https://github.com/assimp/assimp.git
  GIT_TAG        	v5.3.0
)
option(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "Disable Assimp's export functionality")
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "Disable Assimp's export functionality")
option(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
set(ASSIMP_BUILD_OBJ_IMPORTER ON)
set(ASSIMP_BUILD_GLTF_IMPORTER ON)
FetchContent_MakeAvailable(assimp)
target_link_libraries(${PROJECT_NAME} PUBLIC assimp)

######################################## BOOST
# Boost is different than the others. Due to its size we don't want to build it, we want to download it
# as a pre-compiled library and link it to our project.
message(STATUS "Downloading pre-compiled boost library sources.")
set(FETCHCONTENT_QUIET TRUE) # Sets the output level of the build process to terminal
set(DOWNLOAD_EXTRACT_TIMESTAMP TRUE)

set(BOOST_INCLUDE_LIBRARIES uuid filesystem)
set(BOOST_ENABLE_CMAKE ON)
set(Boost_NO_SYSTEM_PATHS ON)

set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF)

FetchContent_Declare(
    Boost
    URL https://github.com/boostorg/boost/releases/download/boost-1.84.0/boost-1.84.0.7z
    USES_TERMINAL_DOWNLOAD TRUE 
    GIT_PROGRESS TRUE   
    DOWNLOAD_NO_EXTRACT FALSE
)
FetchContent_MakeAvailable(Boost)
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::uuid Boost::filesystem)
install(TARGETS 
boost_uuid boost_assert boost_config boost_container_hash boost_core boost_io boost_move boost_numeric_conversion boost_predef boost_static_assert boost_throw_exception boost_tti boost_type_traits boost_winapi boost_describe boost_mp11 boost_random boost_mpl boost_preprocessor boost_conversion boost_function_types boost_array boost_dynamic_bitset boost_integer boost_range boost_utility 
boost_system boost_smart_ptr boost_detail boost_concept_check boost_iterator boost_optional boost_regex boost_tuple boost_variant2
boost_fusion boost_typeof boost_functional boost_function boost_bind boost_filesystem boost_atomic boost_align
EXPORT ${PROJECT_NAME}Config)

######################################## END OF EXTERNAL DEPENDENCIES
# Restore dev warnings
set(CMAKE_WARN_DEPRECATED ON CACHE BOOL "" FORCE)

# OpenGL
find_package(OpenGL REQUIRED)

# Link the dependency libraries to the target
target_link_libraries(${PROJECT_NAME} PUBLIC OpenGL::GL)

# Define the include DIRs for the consumers of the library
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/project_includes>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src>
    $<INSTALL_INTERFACE:include>
    $<INSTALL_INTERFACE:src>
)

install(DIRECTORY include/ DESTINATION inc)
install(TARGETS ${PROJECT_NAME} DESTINATION lib)