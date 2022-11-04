#ifndef APPLICATION_H
#define APPLICATION_H

namespace Nuim {
	class Application {
	public:
		int Run();
	private:
		int initWindow(); 
		int initGL();
		int mainLoop();
		int clearEngine();
	};
	Application* CreateApplication();
};
#endif // APPLICATION_H