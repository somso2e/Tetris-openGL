#include "application.hpp"
#ifdef _DEBUG
#define MAIN main
#else 
#define MAIN WinMain
#endif

int MAIN() {
	int init = Application::Run();
	if (init) {
		return -1;
	}

	Application::Shutdown();
	return 0;
}

