#pragma once

#include <string>
#include <vector>

namespace bWidgetsDemo {

class ShaderProgram
{
public:
	typedef enum {
		ID_UNIFORM_COLOR,
		ID_SMOOTH_COLOR,
		ID_BITMAP_TEXTURE_UNIFORM_COLOR,

		SHADER_PROGRAM_ID_TOT
	} ShaderProgramID;

	ShaderProgram(ShaderProgramID shader_program_id);
	~ShaderProgram();

	unsigned int ProgramID() const;

	ShaderInterface* getInterface() const;

private:
	unsigned int programID;
	ShaderInterface* interface;
};

} // namespace bWidgetsDemo
