#include "GameObjectManager.h"

GameObject* GameObjectManager::CreateGameObject() 
{
	GameObject* newGameObject;
	int identifier						= getNextGameObjectCount();
	newGameObject						= new GameObject(identifier);
	newGameObject->viewMatrix			= viewMatrix;
	newGameObject->m_projectionMatrix	= m_ProjectionMatrix;
	currentGameObjects[identifier]		= newGameObject;

	m_GameObjects.push_back(newGameObject);


	return newGameObject;
}

GameObject* GameObjectManager::CreateGameObject(IComponent* RenderingComponent) 
{
	GameObject* newGameObject;
	int identifier						= getNextGameObjectCount();
	newGameObject						= new GameObject(identifier);
	newGameObject->viewMatrix			= viewMatrix;
	newGameObject->m_projectionMatrix	= m_ProjectionMatrix;
	currentGameObjects[identifier]		= newGameObject;

	m_GameObjects.push_back(newGameObject);

	//give it a rendering component and a mesh and material 


	return newGameObject;
}

GameObject* GameObjectManager::CreateDefaultGameObject() 
{
	GameObject* newGameObject;
	int identifier						= getNextGameObjectCount();
	newGameObject						= new GameObject(identifier);
	newGameObject->viewMatrix			= viewMatrix;
	newGameObject->m_projectionMatrix	= m_ProjectionMatrix;
	currentGameObjects[identifier]		= newGameObject;

	m_GameObjects.push_back(newGameObject);

	//give it a rendering component and a mesh and material 
	RenderingComponent* defaultRenderingComponent = new RenderingComponent();

	return newGameObject;
}


void GameObjectManager::Serialize(JsonSerializationArchive& archive) 
{
	/*archive.Serialize(m_GameObjectCount, "gameObjectCount");*/
	archive.Serialize(m_GameObjects, "gameObjectsInManager");
	
}

void GameObjectManager::Initialize() {
	for (int i = 0; i < m_GameObjects.size(); i++) {
		m_GameObjects[i]->viewMatrix = viewMatrix;
		m_GameObjects[i]->m_projectionMatrix = m_ProjectionMatrix;
		m_GameObjects[i]->Initialize();
		m_GameObjects[i]->SetIdentifier(getNextGameObjectCount());
		currentGameObjects[m_GameObjects[i]->getIdentifier()]	= m_GameObjects[i]; 
	}
}

void GameObjectManager::ClearGameObjects() {
	for (int i = 0; i < m_GameObjects.size(); i++) {
		m_GameObjects[i]->ClearComponents();
		delete m_GameObjects[i];
		
	}
	m_GameObjects.clear();
	m_GameObjectCount = 0;
	currentGameObjects.clear();
}

//void GameObjectManager::createGameObject(int identifier, IComponent* component) {
//	GameObject* newGameObject;
//	newGameObject = new GameObject(identifier);
//	newGameObject->setNewComponent(component); //TODO IDENTIFIER FOR COMPONENT!! INT? STRING? SHOULD IT GET CREATED HERE OR SHOULD IT BE SOMETHING THAT YOU CAN CHOOSE?????
//	setNewGameObject(newGameObject);
//
//}
//
//void GameObjectManager::createGameObject(std::string name, int identifier) {
//	GameObject* newGameObject;
//	newGameObject = new GameObject(name, identifier);
//
//	setNewGameObject(newGameObject);
//
//}
//void GameObjectManager::createGameObject(std::string name, int identifier, std::vector<IComponent*> components) {
//	GameObject* newGameObject;
//	newGameObject = new GameObject(name, identifier);
//
//	for (int i = 0; i < components.size(); i++) {
//		newGameObject->setNewComponent(components.at(i));
//	}
//	setNewGameObject(newGameObject);
//
//}
//void GameObjectManager::createGameObject(std::string name, int identifier, IComponent* component) {
//	GameObject* newGameObject;
//	newGameObject = new GameObject(name, identifier);
//
//	newGameObject->setNewComponent(component);
//	setNewGameObject(newGameObject);
//
//}