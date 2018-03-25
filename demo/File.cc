#include <assert.h>
#include <fstream>

#include "File.h"

using namespace bWidgetsDemo;



File::File(const std::string& path) :
    path(path)
{
	
}

std::string File::readIntoString()
{
	std::ifstream file_stream(path, std::ios::in);
	std::string string = "";

	if (file_stream.is_open()) {
		std::string line = "";
		while (getline(file_stream, line)) {
			string += line + '\n';
		}
		file_stream.close();
	}
	else {
		assert(0);
	}

	return string;
}
