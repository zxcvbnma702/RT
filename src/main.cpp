#include <iostream>
#include "CApp.h"

#ifdef _WIN32
#undef main
#elif defined(_WIN64)
#undef main
#endif // _WIN32

int main(int argc, char* argv[]) {
	CApp app;
	return app.OnExecute();
}