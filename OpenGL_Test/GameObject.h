#pragma once
#include <glm/glm.hpp>
#include "IComponent.h"
#include <vector>
#include <string>
#include <map>
#include "Serialization/JsonArchive.h"



class GameObject {

	
	std::map<ComponentType, IComponent*> Components;
	std::vector<IComponent*> m_Components;
	
	int identifier = 0;
	

	public:
	glm::mat4* viewMatrix = nullptr;
	glm::mat4* m_projectionMatrix = nullptr;
		
	GameObject(){};
	GameObject(int id) {
		m_Name = std::string( "GameObject" + std::to_string(id));
		identifier = id;
	}
	GameObject(std::string name, int id) {
		m_Name = name;
		identifier = id;
	}

	std::string m_Name			= "NotInitializedGameObject";
	glm::vec3 m_Transform		= glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_Rotation		= glm::vec3(0.0f, 1.0f, 0.0f);
	float m_RotationAngle		= 0.0f;
	glm::vec3 m_Scale			= glm::vec3(1.0f, 1.0f, 1.0f);

	bool transformChanged		= true;
	bool rotationChanged		= true;
	bool scaleChanged			= true;

	
	int getIdentifier()														{ return identifier;};
	void SetIdentifier(int id)												{identifier = id;}
	

	void setGameObjectName(std::string name)								{ m_Name = name.c_str();};
	std::string getGameObjectName()											{ return m_Name;};

	
	void setTransform(glm::vec3 newTransform)								{ m_Transform = newTransform;										transformChanged = true;};
	void setRotation(glm::vec3 rotationAxis, float newRotationAngle)		{ m_Rotation = rotationAxis; m_RotationAngle = newRotationAngle;	rotationChanged = true;};
	void setScale(glm::vec3 newScale)										{ m_Scale = newScale;												scaleChanged = true;};


	void Update();

	void Serialize(JsonSerializationArchive& archive);
	void Initialize();
	void ClearComponents();

	/*void setNewComponent(IComponent* newComponent, ComponentType type)		{ ownComponentPointers.push_back(newComponent); newComponent->setOwnGameObject(this); Components[type] = newComponent; } ;*/
	/*IComponent* getComponentByType(ComponentType type)						{ return Components[type]; };*/

	void addComponent(IComponent* newComponent)								{ m_Components.push_back(newComponent); newComponent->Initialize(*this);}
	
	template <class T>
	T* getComponent(){
		for (IComponent* comp : m_Components) {
			T* result = dynamic_cast<T*>(comp);
			if (result != nullptr) {
				return result;
			}
		}
		return nullptr;
		
	}

	template<class M>
	bool hasComponent(IComponent* component) {
		for (IComponent* comp : m_Components) {
			M* result = dynamic_cast<M*>(comp);
			if (result != nullptr) {
				component = result;
				return true;
			}
		}
		return false;
	}
	
};
