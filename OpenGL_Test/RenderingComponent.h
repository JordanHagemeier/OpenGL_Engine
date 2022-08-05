#pragma once

#include "Shader.h"
#include "Mesh.h"
#include "GameObject.h"
#include "IComponent.h"
#include "TextureLoader.h"
#include <map>
#include "PrimitiveGenerator.h"
#include "Material.h"
#include "ShaderManager.h"



class RenderingComponent : public IComponent {

	glm::mat4 m_Model						= glm::mat4(1.0f);
	glm::mat4* m_View						= nullptr;
	glm::mat4* m_Projection					= nullptr;

	Material* m_Material					= nullptr;
	Mesh* m_Mesh							= nullptr;
	PrimitiveType m_MeshType				= PrimitiveType::Cube; //using a default mesh type here
	PrimitiveInformation m_PrimitiveInfo	= {glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 0.0f, 00, 0, 0.0f};
	GLint m_VAO								= 0;


	public:

	Material* m_DefaultMaterial;
	Mesh* m_DefaultMesh;

	glm::vec3 m_CameraPosition	= glm::vec3(0.0f);
	glm::vec3 m_MouseRayDirection = glm::vec3(0.0f);

	bool boundingBoxDrawing = false;
	
	RenderingComponent()
	{
		
		m_DefaultMesh = PrimitiveGenerator::callFunctionByEnum(m_MeshType, m_PrimitiveInfo);
	};

	~RenderingComponent() 
	{
		std::cout << "~RenderingComponent" << std::endl;
	}
		//ownGameObject = nullptr;

		// model = glm::mat4(0.0f);
		// view = glm::mat4(0.0f);
		// projection = glm::mat4(0.0f);

		//material = nullptr;
		//mesh = nullptr;
		//meshType = PrimitiveType::Cube; //using a default mesh type here
		//primitiveInfo;
		//VAO = 0;



		//cameraPosition = glm::vec3(0.0f);
		//mouseRayDirection = glm::vec3(0.0f);

		//boundingBoxDrawing = false;
	//}

	RenderingComponent(glm::mat4& viewMatrix, glm::mat4& projectionMatrix, Material* newMaterial, Mesh* newMesh) {
		m_Model				= glm::mat4(1.0f);
		m_View				= &viewMatrix;
		m_Projection			= &projectionMatrix;
		m_Material			= newMaterial;
		m_Mesh				= newMesh;
		m_MeshType			= newMesh->ownType;
		m_VAO					= newMesh->VAO;
		boundingBoxDrawing	= newMesh->boundingBoxDrawing;
		m_PrimitiveInfo		= newMesh->ownInformation;

		ShaderManager::SetMat4InUniform(m_Material->shader, "model", 1, m_Model);
		ShaderManager::SetMat4InUniform(m_Material->shader, "view", 1, *m_View);
		ShaderManager::SetMat4InUniform(m_Material->shader, "projection", 1, *m_Projection);

	}

	void Update();
	void Debug();
	void Serialize(JsonSerializationArchive& archive) override;
	void Initialize(GameObject& gameObject) override;
	void ClearComponent() override;
	/*void Initialize(GameObject& gameObject) override;*/

	void updateMatrices();
	void setMaterial(Material* newMaterial)				{	m_Material				= newMaterial; };
	void setMesh(Mesh* newMesh)							{	m_Mesh					= newMesh; 
															m_VAO					= m_Mesh->VAO; 
															boundingBoxDrawing		= newMesh->boundingBoxDrawing; 
															m_MeshType				= newMesh->ownType; 
															m_PrimitiveInfo			= newMesh->ownInformation;
														};

	Mesh* getMesh()										{ return m_Mesh; }
	void renderBoundingBox();

	void setModelMatrix(glm::mat4 newModel){ m_Model = newModel;};
	glm::mat4 getModelMatrix(){ return m_Model;};

	ComponentType getComponentType() const				{ return ComponentType::RenderingComponent; };
	void checkRayBoundingBoxIntersection();

	void SetUpShaderValues(Shader currentShader);
	void SetTextures(const std::string& shaderId, std::vector<Texture> textures);
};