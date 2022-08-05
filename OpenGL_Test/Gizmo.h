#pragma once
#include "GameObject.h"
#include "Vertex.h"
#include "Shader.h"
#include "Mesh.h"
#include "MeshLoader.h"

class Gizmo
{

	public:
		GameObject* ownGameObject;
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 scale = glm::vec3(0.0f, 0.0f, 0.0f);

		glm::vec3 translationVectorX = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 translationVectorY = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 translationVectorZ = glm::vec3(0.0f, 0.0f, 0.0f);

		std::vector<Vertex> gizmoVertices;
		Shader gizmoShader;

		Mesh* gizmoMesh;

		std::vector<int> gizmoIndices{
			0,2,1,
			0,3,2,

			4,5,6,
			4,6,7,

			8,9,10,
			8,10,11
		};




		
	
};

