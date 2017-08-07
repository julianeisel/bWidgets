#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"

typedef enum GPUShaderID {
	GPU_SHADER_2D_UNIFORM_COLOR,
} GPUShaderID;

void GPU_init(void);
void GPU_exit(void);

void gpuTranslate2f(const float x, const float y);
void gpuTranslate2f(const float vec[2]);

void gpuOrtho(
        float left, float right,
        float bottom, float top);
