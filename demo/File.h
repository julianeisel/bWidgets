#pragma once

#include <string>

namespace bWidgetsDemo {

class File
{
public:
	File(const std::string& path);
	std::string readIntoString();

private:
	std::string path;
};

} // namespace bWidgetsDemo
