#pragma once
#include "../externals/imgui/imgui.h"
#include "../GameObjectManager.h"
class ImGuiUtility
{
public: 
	ImGuiUtility() {}

	void UpdateImGuiInterface(GameObjectManager& gameObjectManager);
	//should update:
	// -> how many gameobjects are there in the gameobject manager
	// -> get input on slider, set new transforms
	// -> updates whether the gameobject manager should create a new gameobject or not
	
	void UpdateGizmos();
	// -> updates position for each gizmo

	void RenderImGuiInterface();
	// -> renders all windows with gameobjects
	

	void RenderGizmos();
	
	
};

