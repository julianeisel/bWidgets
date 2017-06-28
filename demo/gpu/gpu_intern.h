#pragma once

extern "C" { // Gawain links to this
void gpuBindMatrices(const struct ShaderInterface* shaderface);
bool gpuMatricesDirty(void);
}
