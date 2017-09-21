#include "Application.h"

using namespace bWidgetsDemo;


int main(int /*argc*/, char** /*argv*/)
{
	Application& app = Application::ensureApplication();

	app.setup();
	app.mainLoop();
	app.exit();

	return 0;
}
