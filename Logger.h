#pragma once
#include <cstdio>
#include <ctime>

// Previous solution (not used, because we are in C++ and it's apparently better to use templates)
// #define LOG(...) Logger::LogMessage(("[LOG]: " __VA_ARGS__))


class Logger
{
private:
	// Log file details
	const char* logFilename = "engineLOG.txt";
	FILE* logFile = nullptr;

	// Log behaviour flags
	bool fileLogging = false;
	bool consoleLogging = false;
	bool logEnabled = false;
	bool debugEnabled = false;
	bool performanceEnabled = false;

	// Log default messages
	const char* logHeader = "Homemade Engine Log";
	const char* authorInfo = "by Jakub Fedorowicz";

	
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
			auto time = std::time(nullptr);
			fprintf(logFile, "%s - %s%s\n", logHeader, ctime(&time), authorInfo);

			// Close the file
			fclose(logFile);
		}
	}

	template<typename... Args> void LOG(const char* f, Args... args) {
		if (logEnabled)
		{
			defaultPrint("LOG", f, args...);
		}
	}

	template<typename... Args> void DEBUG(const char* f, Args... args) {
		if (debugEnabled)
		{
			defaultPrint("DEBUG", f, args...);
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
		if (true)
		{
			defaultPrint("ERROR", f, args...);
		}
	}

	template<typename... Args> void WARNING(const char* f, Args... args) {
		if (true)
		{
			defaultPrint("WARNING", f, args...);
		}
	}
};
