#include "bx/core/application.hpp"

int main(int argc, char** argv)
{
	AppConfig config;
	config.argc = argc;
	config.argv = argv;
	return Application::Launch(config);
}