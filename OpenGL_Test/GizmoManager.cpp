#include "GizmoManager.h"


#include "Gizmo.h"


void GizmoManager::CreateAllGizmos(GameObjectManager gameObjectManager, Shader gizmoShader) {
	for (int i = 0; i < gameObjectManager.getCurrentGameObjectCount(); i++) {
		Gizmo* newGizmo = CreateGizmo(gameObjectManager.getGameObject(i), gizmoShader);


	}
}

Gizmo* GizmoManager::CreateGizmo(GameObject* gameObject, Shader shader) {
	Gizmo* newGizmo			= new Gizmo();
	newGizmo->ownGameObject = gameObject;
	newGizmo->translation	= gameObject->m_Transform;
	newGizmo->rotation		= gameObject->m_Rotation;
	newGizmo->scale			= gameObject->m_Scale;
	newGizmo->gizmoShader	= shader;

	CalculateGizmoArrows(newGizmo);
	Mesh* newGizmoMesh		= MeshLoader::GenerateMesh(newGizmo->gizmoVertices, newGizmo->gizmoIndices);
	newGizmo->gizmoMesh		= newGizmoMesh;

	m_Gizmos.push_back(newGizmo);
	return newGizmo;
}

void GizmoManager::UpdateGizmo(Gizmo* gizmo) {


	//time for some model matrix stuff
	CalculateUpdatedGizmoPosition(gizmo);
}


void GizmoManager::RenderAllGizmos() {
	for (int i = 0; i < m_Gizmos.size(); i++) {
		UpdateGizmo(m_Gizmos[i]);
		glUseProgram(m_Gizmos[i]->gizmoShader.shaderIndex);
		glBindVertexArray(m_Gizmos[i]->gizmoMesh->VAO);

		glDrawElements(GL_TRIANGLES, m_Gizmos[i]->gizmoMesh->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}


}

void GizmoManager::CalculateUpdatedGizmoPosition(Gizmo* gizmo) {

	gizmo->translation			= gizmo->ownGameObject->m_Transform;

	glm::mat4x4 model(1.0f);
	model[3]					= glm::vec4(gizmo->translation,1.0f);

	ShaderManager::SetMat4InUniform(gizmo->gizmoShader, "model", 1, model);


}

void GizmoManager::CalculateGizmoVector(Gizmo* gizmo) {

	//gizmo->translationVectorX = gizmo->translation + glm::vec3(1.0f, 0.0f, 0.0f); //this will be very very depricated once I implement correct rotation,oof
	//gizmo->translationVectorY = gizmo->translation + glm::vec3(0.0f, 1.0f, 0.0f);
	//gizmo->translationVectorZ = gizmo->translation + glm::vec3(0.0f, 0.0f, 1.0f);


	gizmo->translationVectorX = glm::vec3(1.0f, 0.0f, 0.0f); //this will be very very depricated once I implement correct rotation,oof
	gizmo->translationVectorY = glm::vec3(0.0f, 1.0f, 0.0f);
	gizmo->translationVectorZ = glm::vec3(0.0f, 0.0f, 1.0f);
}

void GizmoManager::CalculateGizmoArrows(Gizmo* gizmo) {
	CalculateGizmoVector(gizmo);

	glm::vec3 vecXLeft	(	0.0f,		0.0f,		-0.01f		);
	glm::vec3 vecXRight	(	0.0f,		0.0f,		0.01f		);
	glm::vec3 vecYLeft	(	-0.01f,		0.0f,		0.0f		);
	glm::vec3 vecYRight	(	0.01f,		0.0f,		0.0f		);
	glm::vec3 vecZLeft	(	-0.01f,		0.0f,	    0.0f		);
	glm::vec3 vecZRight	(	0.01f,		0.0f,		0.0f		);

	Vertex origin;
	origin.position		= glm::vec3(0.0f, 0.0f, 0.0f);

	//
	/*	0--------------------1
	,					 ,
	3--------------------2  */

	Vertex xAxis_0;
	xAxis_0.position	= origin.position			+ vecXLeft;
	Vertex xAxis_1;
	xAxis_1.position	= gizmo->translationVectorX + vecXLeft;
	Vertex xAxis_2;
	xAxis_2.position	= gizmo->translationVectorX + vecXRight;
	Vertex xAxis_3;
	xAxis_3.position	= origin.position			+ vecXRight;

	Vertex yAxis_0;
	yAxis_0.position	= origin.position			+ vecYLeft;
	Vertex yAxis_1;										 
	yAxis_1.position	= gizmo->translationVectorY + vecYLeft;
	Vertex yAxis_2;										 
	yAxis_2.position	= gizmo->translationVectorY + vecYRight;
	Vertex yAxis_3;										 
	yAxis_3.position	= origin.position			+ vecYRight;


	Vertex zAxis_0;
	zAxis_0.position	= origin.position			+ vecZLeft;
	Vertex zAxis_1;										 
	zAxis_1.position	= gizmo->translationVectorZ + vecZLeft;
	Vertex zAxis_2;										 
	zAxis_2.position	= gizmo->translationVectorZ + vecZRight;
	Vertex zAxis_3;										 
	zAxis_3.position	= origin.position			+ vecZRight;



	gizmo->gizmoVertices.empty();
	/*gizmo->gizmoVertices.push_back(origin);*/
	gizmo->gizmoVertices.push_back(xAxis_0);
	gizmo->gizmoVertices.push_back(xAxis_1);
	gizmo->gizmoVertices.push_back(xAxis_2);
	gizmo->gizmoVertices.push_back(xAxis_3);

	gizmo->gizmoVertices.push_back(yAxis_0);
	gizmo->gizmoVertices.push_back(yAxis_1);
	gizmo->gizmoVertices.push_back(yAxis_2);
	gizmo->gizmoVertices.push_back(yAxis_3);

	gizmo->gizmoVertices.push_back(zAxis_0);
	gizmo->gizmoVertices.push_back(zAxis_1);
	gizmo->gizmoVertices.push_back(zAxis_2);
	gizmo->gizmoVertices.push_back(zAxis_3);


}
