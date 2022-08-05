#include "IComponent.h"
#include "Serialization/JsonArchive.h"

void IComponent::Serialize(JsonSerializationArchive& archive)
{
	if (archive.GetSerializationMode() == SerializationMode::Serialize)
	{
		int componentType = static_cast<int>(getComponentType());
		archive.Serialize(componentType, "componentType"); //#componentTypeDeserialization
	}
}

void IComponent::Initialize(GameObject& gameObject) {
	m_ParentGameObject = &gameObject;
}

void IComponent::ClearComponent() {
	m_ParentGameObject = nullptr;
}