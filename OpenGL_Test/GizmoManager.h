#pragma once
#include "Gizmo.h"
#include "GameObjectManager.h"
class GizmoManager
{

	std::vector<Gizmo*> m_Gizmos;

public:
	void CreateAllGizmos(GameObjectManager gameObjectManager, Shader gizmoShader);
	Gizmo* CreateGizmo(GameObject* gameObject, Shader shader);
	void UpdateGizmo(Gizmo* gizmo);
	void RenderAllGizmos();
	void CalculateUpdatedGizmoPosition(Gizmo* gizmo);
	void CalculateGizmoVector(Gizmo* gizmo);
	void CalculateGizmoArrows(Gizmo* gizmo);
};

