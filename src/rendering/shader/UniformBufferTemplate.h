#pragma once

#include <glm/glm.hpp>
#include <tuple>

// Offsets must be a multiple of 16 bytes
// Any alignments below 16 get rounded up to 16
// The last element can be truncated at actual size instead of rounded up. 
// So total sizes might not be multiples of 16


struct UniformBufferTemplate        // Size:                                        // Offset:
{
    glm::mat4 model;                // 4 bytes * 4 floats * 4 rows = 64 bytes       // 0 bytes
    glm::mat4 view;                 // 4 bytes * 4 floats * 4 rows = 64 bytes       // 64 bytes
    glm::mat4 projection;           // 4 bytes * 4 floats * 4 rows = 64 bytes       // 128 bytes
};                                  // Total: 192 bytes


struct UniformBufferTestingExample  // Size:                                        // Offset:
{
    float value;                    // 4 bytes                                      // 0 bytes
    glm::vec3 vector;               // 4 bytes * 3 floats = 16 bytes                // 16 bytes
    glm::mat4 matrix;               // (Row1) 4 bytes * 4 floats = 16 bytes         // 32 bytes
                                    // (Row2) 4 bytes * 4 floats = 16 bytes         // 48 bytes
                                    // (Row3) 4 bytes * 4 floats = 16 bytes         // 64 bytes
                                    // (Row4) 4 bytes * 4 floats = 16 bytes         // 80 bytes
    float values[3];                // [0] 4 bytes = 16 bytes                       // 96 bytes
                                    // [1] 4 bytes = 16 bytes
                                    // [2] 4 bytes = 16 bytes
    bool boolean;                   // 4 bytes                                       // 144 bytes
    int integer;                    // 4 bytes                                       // 148 bytes
};                                  // Total: 152 bytes


// Alignments

// bool boolean;                   // 4 bytes     
// bool boolean_array[n];          // 4 bytes * n                              
// int integer;                    // 4 bytes
// int integer_array[n];           // 4 bytes * n
// float value;                    // 4 bytes
// float values[n];                // 4 bytes * n
// glm::vec2 vector;               // 4 bytes * 2 floats = 8 bytes                      -> 16 bytes
// glm::vec3 vector;               // 4 bytes * 3 floats = 12 bytes                     -> 16 bytes
// glm::vec4 vector;               // 4 bytes * 4 floats = 16 bytes                     -> 16 bytes
// glm::mat2 matrix;               // (Row1) 4 bytes * 2 floats * 2 rows = 16 bytes     -> 16 bytes
// glm::mat3 matrix;               // (Row1) 4 bytes * 3 floats * 3 rows = 36 bytes     -> 48 bytes
// glm::mat4 matrix;               // (Row1) 4 bytes * 4 floats * 4 rows = 64 bytes     -> 64 bytes             


