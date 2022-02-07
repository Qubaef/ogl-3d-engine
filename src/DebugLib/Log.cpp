#include "Log.h"

#include <Windows.h>

// Global static 
Log LOG;

void Log::setConsoleColor(int c)
{
	const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// Save current console color to be able to restore it
	CONSOLE_SCREEN_BUFFER_INFO Info;
	GetConsoleScreenBufferInfo(hConsole, &Info);
	currentConsoleColor = (char)Info.wAttributes;

	// Set given color c
	SetConsoleTextAttribute(hConsole, c);
}

void Log::resetConsoleColor()
{
	const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, currentConsoleColor);
}