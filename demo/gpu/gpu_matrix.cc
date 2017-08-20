#include <iostream>
#include <cstring>

extern "C" {
#include "../../extern/gawain/gawain/shader_interface.h"
}

#include "gpu_intern.h"
#include "GPU.h"


/* Based on Blender's gpu_matrix.c */

#define DEBUG_MATRIX_BIND 0

#define MATRIX_STACK_DEPTH 32

namespace bWidgetsDemo {

typedef struct MatrixStack {
	glm::mat4 stack[MATRIX_STACK_DEPTH];
	unsigned int top;
} MatrixStack;

typedef struct {
	MatrixStack model_view_stack;
	MatrixStack projection_stack;

	bool dirty;

	/* TODO: cache of derived matrices (Normal, MVP, inverse MVP, etc)
	 * generate as needed for shaders, invalidate when original matrices change
	 *
	 * TODO: separate Model from View transform? Batches/objects have model,
	 * camera/eye has view & projection
	 */
} MatrixState;

#define MATRIX_4X4_IDENTITY {{1.0f, 0.0f, 0.0f, 0.0f}, \
                             {0.0f, 1.0f, 0.0f, 0.0f}, \
                             {0.0f, 0.0f, 1.0f, 0.0f}, \
                             {0.0f, 0.0f, 0.0f, 1.0f}}

static MatrixState state = {
	.model_view_stack = {{MATRIX_4X4_IDENTITY}, 0},
	.projection_stack = {{MATRIX_4X4_IDENTITY}, 0},
	.dirty = true
};

} // namespace bWidgetsDemo

#undef MATRIX_4X4_IDENTITY

#define ModelViewStack state.model_view_stack
#define ModelView ModelViewStack.stack[ModelViewStack.top]

#define ProjectionStack state.projection_stack
#define Projection ProjectionStack.stack[ProjectionStack.top]

using namespace bWidgetsDemo;


static glm::mat4 gpuGetModelViewMatrix()
{
	return ModelView;
}

static glm::mat4 gpuGetProjectionMatrix()
{
	return Projection;
}

static glm::mat4 gpuGetModelViewProjectionMatrix()
{
	return Projection * ModelView;
}

static glm::mat4 gpuGetNormalMatrix()
{
	glm::mat4 normal_mat = gpuGetModelViewMatrix();

	glm::inverse(normal_mat);
	glm::transpose(normal_mat);

	return normal_mat;
}

void gpuBindMatrices(const ShaderInterface* shaderface)
{
	/* set uniform values to matrix stack values
	 * call this before a draw call if desired matrices are dirty
	 * call glUseProgram before this, as glUniform expects program to be bound
	 */

	const ShaderInput *MV = ShaderInterface_builtin_uniform(shaderface, UNIFORM_MODELVIEW);
	const ShaderInput *P = ShaderInterface_builtin_uniform(shaderface, UNIFORM_PROJECTION);
	const ShaderInput *MVP = ShaderInterface_builtin_uniform(shaderface, UNIFORM_MVP);

	const ShaderInput *N = ShaderInterface_builtin_uniform(shaderface, UNIFORM_NORMAL);
	const ShaderInput *MV_inv = ShaderInterface_builtin_uniform(shaderface, UNIFORM_MODELVIEW_INV);
	const ShaderInput *P_inv = ShaderInterface_builtin_uniform(shaderface, UNIFORM_PROJECTION_INV);

	if (MV) {
		#if DEBUG_MATRIX_BIND
		puts("setting MV matrix");
		#endif

		glUniformMatrix4fv(MV->location, 1, GL_FALSE, &gpuGetModelViewMatrix()[0][0]);
	}

	if (P) {
		#if DEBUG_MATRIX_BIND
		puts("setting P matrix");
		#endif

		glUniformMatrix4fv(P->location, 1, GL_FALSE, &gpuGetProjectionMatrix()[0][0]);
	}

	if (MVP) {
		#if DEBUG_MATRIX_BIND
		puts("setting MVP matrix");
		#endif

		glUniformMatrix4fv(MVP->location, 1, GL_FALSE, &gpuGetModelViewProjectionMatrix()[0][0]);
	}

	if (N) {
		#if DEBUG_MATRIX_BIND
		puts("setting normal matrix");
		#endif

		glUniformMatrix3fv(N->location, 1, GL_FALSE, &gpuGetNormalMatrix()[0][0]);
	}

	if (MV_inv) {
		glm::mat4 m = gpuGetModelViewMatrix();
		glm::inverse(m);
		glUniformMatrix4fv(MV_inv->location, 1, GL_FALSE, &m[0][0]);
	}

	if (P_inv) {
		glm::mat4 m = gpuGetProjectionMatrix();
		glm::inverse(m);
		glUniformMatrix4fv(P_inv->location, 1, GL_FALSE, &m[0][0]);
	}

	state.dirty = false;
}


bool gpuMatricesDirty(void)
{
	return state.dirty;
}

static void gpuMultMatrix(const glm::mat4& m)
{
	ModelView *= m;
	state.dirty = true;
}

void gpuTranslate2f(const float x, const float y)
{
	glm::mat4 m;

	m[3][0] = x;
	m[3][1] = y;

	gpuMultMatrix(m);
}

void gpuTranslate2f(const float vec[2])
{
	gpuTranslate2f(vec[0], vec[1]);
}

static void mat4_ortho_set(
        glm::mat4& m,
        float left, float right,
        float bottom, float top,
        float near, float far)
{
	m[0][0] = 2.0f / (right - left);
	m[1][0] = 0.0f;
	m[2][0] = 0.0f;
	m[3][0] = -(right + left) / (right - left);

	m[0][1] = 0.0f;
	m[1][1] = 2.0f / (top - bottom);
	m[2][1] = 0.0f;
	m[3][1] = -(top + bottom) / (top - bottom);

	m[0][2] = 0.0f;
	m[1][2] = 0.0f;
	m[2][2] = -2.0f / (far - near);
	m[3][2] = -(far + near) / (far - near);

	m[0][3] = 0.0f;
	m[1][3] = 0.0f;
	m[2][3] = 0.0f;
	m[3][3] = 1.0f;

	state.dirty = true;
}

void gpuOrtho(
        float left, float right,
        float bottom, float top)
{
	mat4_ortho_set(Projection, left, right, bottom, top, -100.0f, 100.0f);
}
