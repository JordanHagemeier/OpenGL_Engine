#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <vector>
#include "ComponentTypes.h"
#include "Serialization/JsonArchive.h"


class GameObject;
class IComponent {
public:
	
	virtual ~IComponent()
	{
		std::cout<< "~IComponent" << std::endl; 
	}
	

	GameObject* m_ParentGameObject = nullptr;
	void setOwnGameObject(GameObject* newGameObject)	{ m_ParentGameObject = newGameObject;};
	GameObject* getOwnGameObject( )						{ return m_ParentGameObject; };

	virtual void Update(){};
	virtual void Debug(){};
	virtual ComponentType getComponentType() const = 0;
	virtual void Serialize(JsonSerializationArchive& archive);
	virtual void Initialize(GameObject& gameObject);
	virtual void ClearComponent() ;
};
