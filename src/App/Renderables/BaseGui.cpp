#include "BaseGui.h"

#include "Windows.h"
#include "Psapi.h"

BaseGui::BaseGui(Engine* enginePtr) :
	IProcessable(enginePtr)
{

}

void BaseGui::process()
{
}

void BaseGui::preprocess()
{
	const int plotUpdateFreq = 10;

	//
	// Performance monitor 
	//
	ImGui::SetNextWindowSize(ImVec2(400, 150), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
	ImGui::Begin("Performance monitor", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);                          // Create a window called "Hello, world!" and append into it.

	static int frameTimePlotUpdateFreq = plotUpdateFreq;

	float frameTime = 1000.0f / ImGui::GetIO().Framerate;
	float frameRate = ImGui::GetIO().Framerate;
	frameTimePlotUpdateFreq--;

	if (frameTimePlotUpdateFreq == 0)
	{
		frameTimePlotUpdateFreq = plotUpdateFreq;

		memmove(&frameTimes[0], &frameTimes[1], sizeof(frameTimes) - sizeof(*frameTimes));
		frameTimes[frameTimesLength - 1] = frameTime;
	}

	ImGui::PlotLines("Frame Times", frameTimes, IM_ARRAYSIZE(frameTimes), 0, nullptr);
	ImGui::Text("%.3f ms/frame (%.1f FPS)", frameTime, frameRate);

	ImGui::Separator();

	static int memoryAllocatedPlotUpdateFreq = plotUpdateFreq;

	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	DWORDLONG physMemUsedByMe = pmc.WorkingSetSize;
	float physMemUsedByMeMegabytes = physMemUsedByMe / 1000000;

	memoryAllocatedPlotUpdateFreq--;

	if (memoryAllocatedPlotUpdateFreq == 0)
	{
		memoryAllocatedPlotUpdateFreq = plotUpdateFreq;

		memmove(&allocatedMemory[0], &allocatedMemory[1], sizeof(allocatedMemory) - sizeof(*allocatedMemory));
		allocatedMemory[allocatedMemoryLength - 1] = physMemUsedByMeMegabytes;
	}

	ImGui::PlotLines("Allocated memory", allocatedMemory, IM_ARRAYSIZE(allocatedMemory), 0, nullptr);
	ImGui::Text("Physical memory used (MB): %.0f", physMemUsedByMeMegabytes);

	ImGui::Separator();

	vec3 cameraPos = enginePtr->getCamera()->getPosition();
	ImGui::Text("Camera pos x: %.2f, y: %.2f, z: %.2f", cameraPos[0], cameraPos[1], cameraPos[2]);
	
	ImGui::End();
}

void BaseGui::render()
{
}
