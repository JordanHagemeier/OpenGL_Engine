#include "GameObject.h"
#include "ComponentFactory.h"
#include "../../json/json.hpp"

void GameObject::Update() {
	for (auto& component : m_Components) {
		component->Update();
		component->Debug();
		
	};
};

void GameObject::ClearComponents() {
	

	for (auto* component : m_Components) 
	{
		
		delete component;
	}
	m_Components.clear();
}


IComponent* FactoryFunction(const nlohmann::ordered_json& jsonEntry)
{
	const int componentTypeInt = jsonEntry["componentType"]; // #componentTypeDeserialization
	ComponentType componentType = static_cast<ComponentType>(componentTypeInt);
	return ComponentFactory::CreateComponent(componentType);
}

void GameObject::Serialize(JsonSerializationArchive& archive) 
{
	archive.Serialize(m_Name, "GameObjectName");
	
	archive.Serialize(m_Transform.x, "transform.x");
	archive.Serialize(m_Transform.y, "transform.y");
	archive.Serialize(m_Transform.z, "transform.z");

	archive.Serialize(m_Rotation.x, "rotation.x");
	archive.Serialize(m_Rotation.y, "rotation.y");
	archive.Serialize(m_Rotation.z, "rotation.z");

	archive.Serialize(m_RotationAngle, "rotationAngle");

	archive.Serialize(m_Scale.x, "scale.x");
	archive.Serialize(m_Scale.y, "scale.y");
	archive.Serialize(m_Scale.z, "scale.z");
	
	auto factoryLambda = [](const nlohmann::ordered_json& jsonEntry) {
		const int componentTypeInt = jsonEntry["componentType"]; // #componentTypeDeserialization
		ComponentType componentType = static_cast<ComponentType>(componentTypeInt);
		return ComponentFactory::CreateComponent(componentType);
	};
	std::function<IComponent* (const nlohmann::ordered_json&)> factoryFunction = factoryLambda;

	archive.Serialize(m_Components, "components", factoryFunction);

	

}

void GameObject::Initialize() {
	GameObject* currentGameObject = this;
	for (int i = 0; i < m_Components.size(); i++) {
		
		m_Components[i]->Initialize(*currentGameObject);
	}
}



//std::map<ComponentType, IComponent*> Components;
//std::vector<IComponent*> ownComponentPointers;
//


// Old Serialization made by hand:
/*


auto componentsInObject = nlohmann::json::array();
if (archive.GetSerializationMode() == SerializationMode::Serialize) {



for (auto& component : ownComponentPointers) {

if (archive.GetSerializationMode() == SerializationMode::Serialize) {
JSONObject* oldJson = archive.m_pCurrentJsonObject;

JSONObject componentJson;
archive.m_pCurrentJsonObject = &componentJson;

component->Serialize(archive);

componentsInObject.push_back(componentJson);
archive.m_pCurrentJsonObject = oldJson;
			}
		}

		(*archive.m_pCurrentJsonObject)["components"] = componentsInObject;

	}
	else {

	//	int thisGameObjectsId = getNextGameObjectCount();
	//	createGameObject(thisGameObjectsId);
	//	GameObject* newGameObject = getGameObject(thisGameObjectsId);
	//	newGameObject->Serialize(archive);
	}

*/