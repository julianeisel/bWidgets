#pragma once

#include <string>
#include <vector>


namespace bWidgets {

class ShaderProgram
{
public:
	typedef enum {
		SHADER_PROGRAM_ID_UNIFORM_COLOR,

		SHADER_PROGRAM_ID_TOT
	} ShaderProgramID;

	ShaderProgram(ShaderProgramID shader_program_id);
	~ShaderProgram();

	unsigned int ProgramID() const;

	ShaderInterface* getInterface() const;

private:
	unsigned int programID;
	ShaderInterface* interface;

	std::string parseShader(const std::string&);
	unsigned int compileShader(const std::string&, const struct ShaderType&);
	unsigned int linkProgram(const std::vector<unsigned int>&);
	void cleanupAfterCreation(const std::vector<unsigned int>&);
};

} // namespace bWidgets
