#include <Windows.h>

#include "System.hpp"

struct transformers
{
	int destroyedHumans;
};

struct harrypotter
{
	int eheheeee;
	char expectoPatronum;
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	System system;
	bool result;


	// Create the system object.
	/*system = new System;
	if (!system)
	{
		return 0;
	}*/

	// Initialize and run the system object.
	result = system.Initialize();
	if (result)
	{
		system.Run();
	}

	// Shutdown and release the system object.
	/*delete system;
	system = nullptr;*/

	return 0;
}