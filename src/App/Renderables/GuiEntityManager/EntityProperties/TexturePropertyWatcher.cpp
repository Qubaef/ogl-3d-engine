#include "TexturePropertyWatcher.h"

#include "imgui.h"

TexturePropertyWatcher::TexturePropertyWatcher(std::string name, unsigned textureId,
	int textureW, int textureH, int displayW, int displayH) :
	GuiProperty(std::move(name)),
	textureId(textureId),
	textureW(textureW),
	textureH(textureH),
	displayW(displayW),
	displayH(displayH)
{
}


void TexturePropertyWatcher::display()
{

	if (ImGui::TreeNode(name.c_str()))
	{
		ImVec2 uv_min = ImVec2(1.0f, 1.0f);                 // Top-left
		ImVec2 uv_max = ImVec2(0.0f, 0.0f);                 // Lower-right
		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
		ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, .5f); // 50% opaque white

		// With imgui, display the GL_TEXTURE_2D
		ImGui::Image(reinterpret_cast<void*>(textureId), ImVec2(displayW, displayH), uv_min, uv_max, tint_col, border_col);

		// ImGui::Image((void*) textureId, ImVec2(textureW, textureH), uv_min, uv_max, tint_col, border_col);

		ImGui::TreePop();
	}
}
