#include "application.hpp"

#define _DEBUG
int main() {
    int init = Application::Run();
    if (init) {
        return -1;
    }

    Application::Shutdown();
    return 0;
}
