#pragma once
#include "Shader.h"
#include "TextureLoader.h"
#include "ShaderManager.h"


class Material
{
public:
	Shader shader;
	std::vector<Texture> textures;
	std::map<std::string, float> shaderValues;

	Material()
	{
	 /*shader = ShaderManager::m_DefaultShader;*/
	}
	
};

