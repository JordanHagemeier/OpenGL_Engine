#pragma once
#include "IComponent.h"
#include "GameObject.h"
#include <GLFW/glfw3.h>

class CubeControllerComponent : public IComponent {

	public:

		CubeControllerComponent(){};
		~CubeControllerComponent(){ 
			std::cout<< "~CubeControllerComponent" << std::endl; 
		};

	void Update();

	void Serialize(JsonSerializationArchive& archive) override;
	ComponentType getComponentType() const { return ComponentType::CubeController; };

	float testValue = 0.1f;
};