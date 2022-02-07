#pragma once
#include <cstdio>
#include <ctime>

class Log
{
private:
	// Log file details
	const char* logFilename = "engine-log.txt";
	FILE* logFile = nullptr;

	// Log behaviour flags
	bool fileLogging = false;
	bool consoleLogging = false;
	bool logEnabled = false;
	bool debugEnabled = false;
	bool performanceEnabled = false;

	const char FG_BLUE		= 0x0001; // text color contains blue
	const char FG_GREEN		= 0x0002; // text color contains green
	const char FG_RED		= 0x0004; // text color contains red
	const char FG_INTENSITY	= 0x0008; // text color is intensified
	const char BG_BLUE		= 0x0010; // background color contains blue
	const char BG_GREEN		= 0x0020; // background color contains green
	const char BG_RED		= 0x0040; // background color contains red
	const char BG_INTENSITY	= 0x0080; // background color is intensified

	// Log default messages
	const char* logHeader = "Homemade Engine";
	const char* authorInfo = "Jakub Fedorowicz, qubaefg@gmail.com";

	// Console color to which resetConsoleColor function will refer
	char currentConsoleColor;

	void setConsoleColor(int c);
	void resetConsoleColor();

	template<typename... Args> void defaultPrint(const char* tag, const char* f, Args... args) {
		if (consoleLogging)
		{
			printf("[%s]: ", tag);
			printf(f, args...);
		}
		if (fileLogging)
		{
			logFile = fopen(logFilename, "a+");
			fprintf(logFile, "[%s]: ", tag);
			fprintf(logFile, f, args...);
			fclose(logFile);
		}
	}

public:
	void init(bool fileLogging, bool consoleLogging, bool logEnabled, bool debugEnabled, bool performanceEnabled)
	{
		this->fileLogging = fileLogging;
		this->consoleLogging = consoleLogging;
		this->logEnabled = logEnabled;
		this->debugEnabled = debugEnabled;
		this->performanceEnabled = performanceEnabled;

		if (fileLogging)
		{
			// Open the log file and clear previous data
			logFile = fopen(logFilename, "w");

			// Put log header and time info
			const auto time = std::time(nullptr);
			fprintf(logFile, "%s - %s by %s\n", logHeader, ctime(&time), authorInfo);

			// Close the file
			fclose(logFile);
		}
	}

	template<typename... Args> void INFO(const char* f, Args... args) {
		if (logEnabled)
		{
			defaultPrint("INFO", f, args...);
		}
	}

	template<typename... Args> void DEBUG(const char* f, Args... args) {
		if (debugEnabled)
		{
			setConsoleColor(FG_BLUE | FG_INTENSITY);

			defaultPrint("DEBUG", f, args...);

			resetConsoleColor();
		}
	}

	template<typename... Args> void PERFORMANCE(const char* f, Args... args) {
		if (performanceEnabled)
		{
			if (consoleLogging)
			{
				printf("[PERFORMANCE]: ");
				printf(f, args...);
			}
		}
	}

	template<typename... Args> void ERROR(const char* f, Args... args) {
		setConsoleColor(FG_RED);

		defaultPrint("ERROR", f, args...);

		resetConsoleColor();
	}

	template<typename... Args> void WARNING(const char* f, Args... args) {
		setConsoleColor(FG_RED | FG_GREEN);

		defaultPrint("WARNING", f, args...);

		resetConsoleColor();
	}
};

// Create publicly available Log
extern Log LOG;