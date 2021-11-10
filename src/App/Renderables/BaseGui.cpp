#include "BaseGui.h"

BaseGui::BaseGui(Engine* enginePtr) :
	Processable(enginePtr)
{

}

void BaseGui::process()
{
}

void BaseGui::preprocess()
{
	// static bool show_demo_window = true;
	// static bool show_another_window = true;
	// static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	// static float f = 0.0f;
	// static int counter = 0;

	// int w, h;
	// glfwGetWindowSize(enginePtr->getGlWindow(), &w, &h);

	ImVec2 size(400, 100);
	// ImVec2 pos(w - size[0], h - size[1]);
	ImVec2 pos(0, 0);

	ImGui::SetNextWindowSize(size, ImGuiCond_Once);
	ImGui::SetNextWindowPos(pos, ImGuiCond_Once);
	ImGui::Begin("BaseGui", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);                          // Create a window called "Hello, world!" and append into it.

	// ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	// ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
	// ImGui::Checkbox("Another Window", &show_another_window);
	// 
	// ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	// ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
	// 
	// if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	// 	counter++;
	// ImGui::SameLine();
	// ImGui::Text("counter = %d", counter);

	static int plotUpdateFreq = 1;

	float frameTime = 1000.0f / ImGui::GetIO().Framerate;
	float frameRate = ImGui::GetIO().Framerate;
	plotUpdateFreq--;

	if (plotUpdateFreq == 0)
	{
		plotUpdateFreq = 1;

		memmove(&frameTimes[0], &frameTimes[1], sizeof(frameTimes) - sizeof(*frameTimes));
		frameTimes[frameTimesLength - 1] = frameTime;
	}

	ImGui::PlotLines("Frame Times", frameTimes, IM_ARRAYSIZE(frameTimes), 0, nullptr);
	ImGui::Text("%.3f ms/frame (%.1f FPS)", frameTime, frameRate);

	vec3 cameraPos = enginePtr->getCamera()->getPosition();
	
	ImGui::Text("Camera pos x: %.2f, y: %.2f, z: %.2f", cameraPos[0], cameraPos[1], cameraPos[2]);
	ImGui::End();
}

void BaseGui::render()
{
}
