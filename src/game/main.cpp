#include <windows.h>
#include "application.hpp"

#ifdef _DEBUG
#define MAIN main()
#else 
#define MAIN WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
#endif

int MAIN{
	int init = Application::Run();
	if (init) {
		return -1;
	}

	Application::Shutdown();
	return 0;
}


