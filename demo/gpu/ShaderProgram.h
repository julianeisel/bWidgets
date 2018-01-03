#pragma once

#include <string>
#include <array>

namespace bWidgetsDemo {

typedef enum ShaderTypeID {
	SHADER_TYPE_VERTEX,
	SHADER_TYPE_FRAGMENT,

	SHADER_TYPE_TOT
} ShaderTypeID;

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
	std::array<unsigned int, SHADER_TYPE_TOT> shader_ids;
	unsigned int programID;
	ShaderInterface* interface;
};

} // namespace bWidgetsDemo
