#pragma once

namespace bWidgetsDemo {

/**
 * Singleton class for general data of the bWidget Demo Application.
 */
class Application
{
public:
	static Application& CreateApplication();

	void setup();
	void mainLoop();
	void exit();

private:
	Application() {}
	// Ensure uniqueness
	Application(Application const&) = delete;
	void operator=(Application const&) = delete;

	class WindowManager* wm;
};

} // namespace bWidgetsDemo
