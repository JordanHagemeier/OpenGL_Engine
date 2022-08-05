#pragma once

#ifndef IOSTREAM
#include <iostream>
#define IOSTREAM
#endif // !IOSTREAM

#ifndef VECTOR
#include <vector>
#define VECTOR
#endif // !VECTOR

#ifndef GLAD
#define GLAD
#include <glad/glad.h>
#endif

#include <map>
#include "Serialization/JsonArchive.h"
struct Texture {
	GLint ID;
	std::string Path; 
	std::string Name;
};

class TextureLoader {
	public:
		static void LoadTextureToTarget(unsigned int& texture, unsigned int& target, std::string& fileName, GLenum textureUnit);
		static Texture LoadImageAsTexture(const std::string& fileName, const std::string& textureName);
		static Texture LoadCubemap(const std::string& filename, const std::string& textureName);
		
		static std::map<std::string,Texture> LoadedTextures;
		static Texture GetTextureByIdentifier(std::string identifier){ return LoadedTextures[identifier]; }
		
		static void CreateJsonMapOfTextures();
		static void LoadTexturesFromJsonFile(const std::string& filePath);
};





