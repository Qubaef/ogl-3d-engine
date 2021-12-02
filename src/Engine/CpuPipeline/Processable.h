#pragma once
#ifndef PROCESSABLE_H
#define PROCESSABLE_H

#include "../Include/Common.h"

class Engine;

class Processable
{
private:
	int implementationStatus = 0;

protected:
	const int preprocessFlag = 1 << 0;
	const int processFlag = 1 << 1;
	const int renderFlag = 1 << 2;
	// const int postprocessFlag = 1 << 3;

	Engine* enginePtr;

public:
	Processable(Engine* enginePtr);

	// Operation characteristics:
	// * performed in main thread
	// * performed before all the actual processing (recommended for e.g. Model Matrix Calculation)
	// * NOT PARALLEL, so it should be well optimized and contain only the key operations
	virtual void preprocess() = 0;
	void definePreprocess();
	bool ifDefinedPreprocess();

	// Operation characteristics:
	// * performed in one of the worker threads
	// * executed in processing phase, so should be independent of other Processable components
	// * PARALLEL, so it should contain heaviest of the calculations (e.g. physics calculation)
	virtual void process() = 0;
	void defineProcess();
	bool ifDefinedProcess();

	// Operation characteristics:
	// * performed in main thread
	// * performed after processing phase, when all updates are finished
	// * PARALLEL, so it should contain heaviest of the calculations (e.g. physics calculation)
	virtual void render() = 0;
	void defineRender();
	bool ifDefinedRender();

	// Operation characteristics:
	// * performed in one of the worker threads
	// * performed while main process is busy rendering - proper for time-independent operations
	// * PARALLEL, but it shouldn't be too long, because if it exceeds rendering time, it will affect performance
	// virtual void postprocess();
	// bool ifDefinedPostprocess();

	// To provide that Processable is abstract class
	virtual ~Processable() = default;
};

#endif
