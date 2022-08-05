#pragma once
#include "GameObject.h"
#include "IComponent.h"
#include <vector>
#include <map>
#include "Serialization/JsonArchive.h"
#include "Mesh.h"
#include "RenderingComponent.h"


class GameObjectManager {

	int m_GameObjectCount;
	std::map<int, GameObject*> currentGameObjects;
	std::vector<GameObject*> m_GameObjects;


	public:

		glm::mat4* viewMatrix;
		glm::mat4* m_ProjectionMatrix;

		GameObjectManager(){
		m_GameObjectCount = 0;
		
		};

		int getCurrentGOSize()												{ return (int) currentGameObjects.size(); };
		int getCurrentGameObjectCount()										{ return (int) m_GameObjectCount;}
		GameObjectManager* getGameObjectManager()							{ return this;}
		
		GameObject* getGameObject(int id)									{ return currentGameObjects[id]; }
		int getNextGameObjectCount()										{ int currentNumber = m_GameObjectCount; m_GameObjectCount++; return currentNumber;};

		void updateGameObjects() {
			for (auto& gameObject : m_GameObjects) {
				gameObject->Update();
			};
		}

		GameObject* CreateGameObject();
		GameObject* CreateGameObject(IComponent* renderingComponent);
		GameObject* CreateDefaultGameObject();

		void Serialize(JsonSerializationArchive& archive);
		void Initialize();
		void ClearGameObjects();
		/*void createGameObject(int identifier, IComponent* component);
		void createGameObject(std::string name, int identifier);
		void createGameObject(std::string name, int identifier, std::vector<IComponent*> components);
		void createGameObject(std::string name, int identifier, IComponent* component);*/
};
