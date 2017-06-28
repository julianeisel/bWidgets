#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"

typedef enum GPUShaderID {
	GPU_SHADER_2D_UNIFORM_COLOR,
} GPUShaderID;

void GPU_init(void);
void GPU_exit(void);

void gpuOrtho(
        float left, float right,
        float bottom, float top);
