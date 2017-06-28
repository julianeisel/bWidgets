#include <cassert>
#include <iostream>

extern "C" {
#include "../../extern/gawain/gawain/immediate.h"
}

#include "GPU.h"


static bool initialized = false;

/**
 * \return true if successful.
 */
void GPU_init(void)
{
	if (initialized) {
		assert(0);
		return;
	}

	GLenum glew_ret = glewInit();
	if (glew_ret != GLEW_OK) {
		std::cout << "Error: " << glewGetErrorString(glew_ret) << std::endl;
		assert(0);
	}
	immInit();
}

void GPU_exit(void)
{
	immDestroy();
}
