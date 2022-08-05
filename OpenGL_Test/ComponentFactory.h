#pragma once

#include "IComponent.h"

//might need to forward declare these
#include "RenderingComponent.h"
#include "CubeController.h"

class ComponentFactory {
	
	public: 
	static IComponent* CreateComponent(ComponentType type) {
		switch (type) {
			case ComponentType::RenderingComponent:
				return new RenderingComponent();
				break;

			case ComponentType::CubeController:
				return new CubeControllerComponent();
				break;
		}

		return nullptr;
	}

};