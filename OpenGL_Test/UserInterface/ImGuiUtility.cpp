#include "ImGuiUtility.h"


//should update:
// -> how many gameobjects are there in the gameobject manager
// -> get input on slider, set new transforms
// -> updates whether the gameobject manager should create a new gameobject or not
void ImGuiUtility::UpdateImGuiInterface(GameObjectManager& gameObjectManager) {

	//Object Creation Window
	ImGui::Begin("GameObject Creation");
	if (ImGui::Button("Create New GameObject!")) {
		gameObjectManager.CreateGameObject();
	}
	ImGui::End();

	/*renderIMGUIGameObjectUI();*/

	//Render dear imgui into screen
	/*ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());*/

}