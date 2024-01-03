# FluxLumina

FluxLumina is an OpenGL-based rendering engine designed for high-performance graphics applications. It offers a range of features to facilitate the creation of visually stunning and efficient 3D renderings. 

## Features

- **Forward Rendering**: Efficient rendering for scenarios with fewer lights and simpler shading models, supports shadows
- **Deferred Rendering**: Advanced rendering technique for handling complex lighting scenarios.
- **Physically Based Rendering (PBR)**: Realistic material and lighting models for enhanced visual fidelity.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development purposes.

### Prerequisites

- C++ Compiler (GCC, Clang, MSVC, etc.)
- CMake (version 3.23 or higher)
- OpenGL (version 4.3 or higher)

### Installing

To install FluxLumina, clone the GitHub repository and build the project using CMake.

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

## Usage

To use FluxLumina in your project, include it as a dependency in your CMake project and link against it.

```cmake
# Add FluxLumina as a subdirectory
add_subdirectory(path/to/FluxLumina)

# Link FluxLumina to your project
target_link_libraries(your_project PRIVATE FluxLumina)
```

And then on your C++ code simply instantiate the engine

```c++
#include <FluxLumina.hpp>

int main()
{
    FluxLumina graphicalEngine(E_RenderStrategy::ForwardShading);    
    return 0
}
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
Acknowledgments

    Mention any other contributors or sources that helped create this project.
    Any dependencies or third-party resources used.