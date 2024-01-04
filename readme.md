# FluxLumina

FluxLumina is an OpenGL-based rendering engine, provided as a static library, designed for high-performance graphics applications. It offers a range of features to facilitate the creation of visually stunning and efficient 3D renderings. 

## Features

- **Forward Rendering**: Efficient rendering for scenarios with fewer lights and simpler shading models, supports shadows
- **Deferred Rendering**: Advanced rendering technique for handling complex lighting scenarios.
- **Physically Based Rendering (PBR)**: Realistic material and lighting models for enhanced visual fidelity.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine

### Prerequisites

- C++ Compiler (GCC, Clang, MSVC, etc.)
- CMake (version 3.23 or higher)
- OpenGL (version 4.3 or higher)

### Standalone working example

Check out our [template](https://github.com/diobat/FluxLumina-template) for a working setup of FluxLumina

### Including into your project

To bring FluxLumina into your project, include it as a dependency in your CMake project and link against it. CMake will fetch the source and build the target for you.

```cmake
# Fetch dependencies remotely
include(FetchContent)

FetchContent_Declare(
  FluxLumina
  GIT_REPOSITORY 	https://github.com/diobat/FluxLumina.git
  GIT_TAG        	Release
)
FetchContent_MakeAvailable(FluxLumina)
target_link_libraries(${PROJECT_NAME} PRIVATE FluxLumina)
```

or, alternatively you can get the prebuilt targets from our [releases page](https://github.com/diobat/FluxLumina/releases) and place them on your lib and include folders.

Afterwards, on your C++ code, simply instantiate the engine and you are good to go

```c++
#include <FluxLumina.hpp>

int main()
{
    FluxLumina graphicalEngine(E_RenderStrategy::ForwardShading);    
    return 0
}
```

### Building the target from source

To build the files from source, clone the GitHub repository and use CMake.

```bash
# Clone the repository
git clone https://github.com/diobat/FluxLumina.git

# Go to the project directory
cd FluxLumina

# Create a build directory
mkdir build

# Configure the build
cmake -S . -B build

# Build debug binaries
cmake --build build --config Debug

# Build release binaries
cmake --build build --config Release
```


## Dependencies

  - glfw 
  - glad
  - glm
  - boost::uuid & boost::filesystem
  - stb_image
  - assimp

## License

This project is licensed under the MIT License - see the LICENSE.md file for details.