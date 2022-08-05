#include "RenderingComponent.h"

void RenderingComponent::updateMatrices() {
	bool modelNeedsUpdating = false;

	glm::mat4 modelMat = getModelMatrix();
	if (m_ParentGameObject->transformChanged) {

		m_ParentGameObject->transformChanged = false;
		glm::mat4 newModel = glm::mat4(1.0f);
		modelMat							= glm::translate(newModel, m_ParentGameObject->m_Transform); //maybe this is the way to go? lets try this. otherwise, model = new mat4x4(1.0f)

		modelNeedsUpdating = true;
	}

	if (m_ParentGameObject->rotationChanged) {

		m_ParentGameObject->rotationChanged	= false;
		modelMat						= glm::rotate(modelMat,m_ParentGameObject->m_RotationAngle, m_ParentGameObject->m_Rotation);

		modelNeedsUpdating				= true;
	}

	if (m_ParentGameObject->scaleChanged) {
		m_ParentGameObject->scaleChanged		= false;
		modelMat							= glm::scale(modelMat, m_ParentGameObject->m_Scale);

		modelNeedsUpdating				= true;
	}

	
		
		ShaderManager::SetMat4InUniform(m_Material->shader, "model", 1, modelMat);
		ShaderManager::SetMat4InUniform(m_Material->shader, "view", 1, *m_View);
		ShaderManager::SetMat4InUniform(m_Material->shader, "projection", 1, *m_Projection);
	
}

void RenderingComponent::Initialize(GameObject& gameObject) {
	IComponent::Initialize(gameObject);
	m_View			= gameObject.viewMatrix;
	m_Projection		= gameObject.m_projectionMatrix;
}

void RenderingComponent::Update() {
	updateMatrices();
	SetUpShaderValues(m_Material->shader);
	SetTextures(m_Material->shader.shaderName, m_Material->textures);
	glUseProgram(m_Material->shader.shaderIndex);
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_Mesh->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);

}

void RenderingComponent::SetUpShaderValues(Shader currentShader) {
	std::map<std::string, float>::iterator it;
	for (it = m_Material->shaderValues.begin(); it != m_Material->shaderValues.end(); ++it) {
		ShaderManager::SetFloatInUniform(currentShader, it->first, it->second);
	}
}


void RenderingComponent::SetTextures(const std::string& shaderId, std::vector<Texture> textures) {
	Shader currentShader = ShaderManager::GetShaderByIdentifier(shaderId);

	glUseProgram(currentShader.shaderIndex);
	for (int i = 0; i < textures.size(); i++) {
		if (i < 16) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textures[i].ID);
			
		}
	}
	
	
	glUseProgram(0);
}

void RenderingComponent::Debug() {
	if (boundingBoxDrawing) {
		renderBoundingBox();
		checkRayBoundingBoxIntersection();
	}
}

void RenderingComponent::renderBoundingBox() {
	glUseProgram(m_Material->shader.shaderIndex);
	glBindVertexArray(m_Mesh->boundingBoxVAO);
	glDrawElements(GL_LINES, m_Mesh->cube_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);

}

void RenderingComponent::checkRayBoundingBoxIntersection() {
	//get gameobject for transform matrix
	//get rendering component for mesh
	//translate bounding box vectors with gameobject transform 
	//check in a loop for intersections between ray point and box 

	GameObject* gameObject = getOwnGameObject();
	auto goRender = gameObject->getComponent<RenderingComponent>();
	BoundingBox goBBox = goRender->getMesh()->boundingBox;

	RenderingComponent* testComp = new RenderingComponent;
	if (gameObject->hasComponent<RenderingComponent>(testComp)) {
		goRender = testComp;
	}

	glm::vec4 goTransformVector				= glm::vec4(gameObject->m_Transform, 1.0f);
	glm::vec4 firstBoundingBoxPoint			= glm::vec4(goBBox.xMin, goBBox.yMin, goBBox.zMin, 1.0f);
	glm::vec4 secondBoundingBoxPoint		= glm::vec4(goBBox.xMax, goBBox.yMax, goBBox.zMax, 1.0f);

	firstBoundingBoxPoint	= goTransformVector + firstBoundingBoxPoint;
	secondBoundingBoxPoint	= goTransformVector + secondBoundingBoxPoint;

	//loop for the amount of ray intersection checks we want to make 
	//calculate each time where the ray point would be
	//check for intersection between ray point and bounding box 
	

}

//void setDebugCubePosition(GameObject go) {
//
//	//lol lets spice this up and try out moving the ray intersection check points through space along the direction
//	glm::vec3 currentIntersectionCheckPos = glm::vec3(0.0f);
//	int amountOfRayIntersectionChecksInOneDirection = 30;
//	float rayIntersectionDistanceAddedEachCheck = 3.0f;
//
//	/*for (int i = 0; i < amountOfRayIntersectionChecksInOneDirection; i++) {
//	currentIntersectionCheckPos = Cameras[0].cameraPos + (currentMousePosInWorldSpace * rayIntersectionDistanceAddedEachCheck);
//	currentMousePosInWorldSpace = currentIntersectionCheckPos;
//	}*/
//
//	int intGlfwTime = (int)glfwTime;
//	currentIntersectionCheckPos = Cameras[0].cameraPos + (currentMousePosInWorldSpace * (rayIntersectionDistanceAddedEachCheck * (intGlfwTime % 10)));
//	currentMousePosInWorldSpace = currentIntersectionCheckPos;
//
//
//}
//
//void checkPointBoxIntersection(glm::vec3 point, BoundingBox box) {
//
//}






void RenderingComponent::Serialize(JsonSerializationArchive& archive) 
{
	IComponent::Serialize(archive);

	//serializing mesh type first, since we need it to reconstruct the mesh
	int ownMeshType = static_cast<int>(m_MeshType);
	archive.Serialize(ownMeshType, "MeshType");
	m_MeshType = static_cast<PrimitiveType>(ownMeshType);

	/*Material* material			= nullptr;
	Mesh* mesh					= nullptr;
	GLint VAO					= 0;*/

	JSONObject* nodeBeginPtr		= archive.m_pCurrentJsonObject;
	if (archive.m_pCurrentJsonObject != NULL) {
		if (archive.GetSerializationMode() == SerializationMode::Serialize) {
			(*archive.m_pCurrentJsonObject)["ShaderID"]			= m_Material->shader.shaderIndex;
			(*archive.m_pCurrentJsonObject)["ShaderName"]		= m_Material->shader.shaderName;

			//array for the textures
			auto jsonTextureArray = nlohmann::json::array();

			for (int i = 0; i < m_Material->textures.size(); i++){

				JSONObject textureObject;
				textureObject[std::string("Texture") + std::to_string(i)] = m_Material->textures[i].Name;
				jsonTextureArray.push_back(textureObject);

			}
			(*archive.m_pCurrentJsonObject)["Textures"] = jsonTextureArray;
			
			

			//Serialize primitive Info
			JSONObject primitiveInfoObject;
			archive.m_pCurrentJsonObject = &primitiveInfoObject;
			m_PrimitiveInfo.Serialize(archive);
			archive.m_pCurrentJsonObject = nodeBeginPtr;
			(*archive.m_pCurrentJsonObject)["PrimitiveInformation"] = primitiveInfoObject;
			
		}

		if (archive.GetSerializationMode() == SerializationMode::Deserialize) {
			Material* newMaterial	= new Material();
			newMaterial->shader		= ShaderManager::GetShaderByIdentifier((*archive.m_pCurrentJsonObject)["ShaderName"]);
			

			//getting the textures
			
			auto jsonTextureArray			= (*archive.m_pCurrentJsonObject)["Textures"];
			const size_t size				= jsonTextureArray.size();

			std::vector<Texture> loadedTextureVector;
			loadedTextureVector.reserve(size);

			for (int i = 0; i < size; i++)
			{
				auto jsonEntry					= jsonTextureArray.at(i);
				archive.m_pCurrentJsonObject	= &jsonEntry;
				Texture loadedTexture			= TextureLoader::GetTextureByIdentifier((*archive.m_pCurrentJsonObject)[std::string("Texture") + std::to_string(i)]);
				loadedTextureVector.push_back(loadedTexture);
			}

			archive.m_pCurrentJsonObject		= nodeBeginPtr;
			newMaterial->textures				= loadedTextureVector;
			m_Material							= newMaterial;
			

			//getting the primitive information
			JSONObject primitiveInfoObject;
			primitiveInfoObject = (*archive.m_pCurrentJsonObject)["PrimitiveInformation"];

			archive.m_pCurrentJsonObject = &primitiveInfoObject;
			m_PrimitiveInfo.Serialize(archive);
			archive.m_pCurrentJsonObject = nodeBeginPtr;

			//creating the mesh
			Mesh* newMesh = PrimitiveGenerator::callFunctionByEnum(m_MeshType, m_PrimitiveInfo);
	
			
			
			setMesh(newMesh);

			

		}
	}
	



}


void RenderingComponent::ClearComponent() {
	m_ParentGameObject = nullptr;

	m_Model = glm::mat4(0.0f);
	m_View = nullptr;
	m_Projection = nullptr;

	m_Material = nullptr;
	m_Mesh = nullptr;
	m_MeshType = PrimitiveType::Cube; //using a default mesh type here
	m_PrimitiveInfo;
	m_VAO = 0;



	m_CameraPosition = glm::vec3(0.0f);
	m_MouseRayDirection = glm::vec3(0.0f);

	boundingBoxDrawing = false;
}

//void RenderingComponent::Initialize(GameObject& gameObject) {
//	IComponent::Initialize(gameObject);
//
//	Material newMaterial;
//	newMaterial.shader = ShaderManager::GetShaderByIdentifier((*archive.m_pCurrentJsonObject)["ShaderName"]);
//	for (int i = 0; i < (*archive.m_pCurrentJsonObject)["AmountOfTextures"]; i++) {
//
//		newMaterial.textures.push_back(TextureLoader::GetTextureByIdentifier((*archive.m_pCurrentJsonObject)[std::string("Texture") + std::to_string(i)]));
//	}
//}