#include "StatsPanel.h"

#include "GitGud.h"

#include <imgui/imgui.h>

namespace GitGud
{
	void StatsPanel::OnImGui()
	{
		ImGui::Begin("Stats");

		auto stats = Renderer2D::GetStatistics();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indicies: %d", stats.GetTotalIndexCount());

		if (ImGui::Button("Begin session"))
		{
			GG_PROFILE_BEGIN_SESSION("TestSession", "testSession.json");
		}

		ImGui::SameLine();

		if (ImGui::Button("End session"))
		{
			GG_PROFILE_END_SESSION();
		}

		ImGui::End();
	}
}