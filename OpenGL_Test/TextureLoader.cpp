#include "TextureLoader.h"


#ifndef STB_IMAGE
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE
#endif // !STB_IMAGE


std::map<std::string, Texture> InitMap() {
	std::map<std::string, Texture> newMap;
	return newMap;
}

std::map<std::string, Texture> TextureLoader::LoadedTextures = InitMap();

std::vector<std::string> faces =
{
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
};

void TestOpenGlErrorLolz()
{
	GLenum errorCode = glGetError();

	while (errorCode != GL_NO_ERROR) 
	{
		//std::string fileString(file);
		std::string error = "unknown error";

		// clang-format off
		switch (errorCode) {
		case GL_INVALID_ENUM:      error = "GL_INVALID_ENUM"; break;
		case GL_INVALID_VALUE:     error = "GL_INVALID_VALUE"; break;
		case GL_INVALID_OPERATION: error = "GL_INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:    error = "GL_STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:   error = "GL_STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:     error = "GL_OUT_OF_MEMORY"; break;
		}
		// clang-format on

		std::cerr << "OpenglError " << error << std::endl;
		errorCode = glGetError();
	}
}


void TextureLoader::LoadTextureToTarget(unsigned int& texture, unsigned int& target, std::string& fileName, GLenum textureUnit) {
	/*unsigned int texture;*/
	glActiveTexture(textureUnit);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(target);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	stbi_set_flip_vertically_on_load(true);  

	const char* fN							= fileName.c_str();
	int width, height, nrChannels;
	unsigned char *data						= stbi_load(fN, &width, &height, &nrChannels, 0); 


	
	if (data)
	{
		if (fileName.substr(fileName.find_last_of(".") + 1) == "png") {
		
			
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		if(fileName.substr(fileName.find_last_of(".") + 1) == "jpg"){
			
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}


	stbi_image_free(data);
	glBindVertexArray(0);
	
}


Texture TextureLoader::LoadImageAsTexture(const std::string& fileName, const std::string& textureName) {
	
	unsigned int texture					= 0;
	/*unsigned int texture;*/
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	stbi_set_flip_vertically_on_load(true);  

	const char* fN							= fileName.c_str();
	int width, height, nrChannels;
	unsigned char *data = stbi_load(fN, &width, &height, &nrChannels, 0); 

	if (data)
	{
		if (fileName.substr(fileName.find_last_of(".") + 1) == "png") {

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		if(fileName.substr(fileName.find_last_of(".") + 1) == "jpg"){
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}


	stbi_image_free(data);
	
	Texture newTexture;
	newTexture.ID = texture;
	newTexture.Path = fileName;
	newTexture.Name = textureName;

	LoadedTextures[textureName] = newTexture;
	return newTexture;
}

Texture TextureLoader::LoadCubemap(const std::string& filename, const std::string& textureName)
{
	unsigned int textureID;
	
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;

	stbi_set_flip_vertically_on_load(false); 
	//add filename to faces
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		std::string currentFileName = filename;
		std::string fileNamePlusFace = faces[i];
		if (!filename.empty()) {
			 fileNamePlusFace = fileNamePlusFace.replace(0,10, currentFileName.append(faces[i]));
		}
		
		TestOpenGlErrorLolz();
		unsigned char *data = stbi_load(fileNamePlusFace.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}

	TestOpenGlErrorLolz();
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	TestOpenGlErrorLolz();
	Texture newTexture;
	newTexture.ID = textureID;
	newTexture.Path = filename;
	newTexture.Name = textureName;

	LoadedTextures[textureName] = newTexture;
	return newTexture;
}  


void TextureLoader::CreateJsonMapOfTextures() {
	JsonSerializationArchive archive = JsonSerializationArchive(SerializationMode::Serialize);

	// Saving
	auto texturesArray = nlohmann::json::array();

	for (const auto& entry : LoadedTextures)
	{
		JSONObject textureDetailsObject;
		JSONObject textureObject;

		const std::string& textureIdentifier	= entry.first;
		const std::string& texturePath			= entry.second.Path;
		 
		textureDetailsObject["Name"]			= textureIdentifier;
		textureDetailsObject["Path"]			= texturePath;
		textureObject[textureIdentifier]		= textureDetailsObject;

		texturesArray.push_back(textureObject);
	}
	(*archive.m_pCurrentJsonObject)["LoadedTextures"] = texturesArray;



	archive.Save("TexturesToLoad");
	
}

void TextureLoader::LoadTexturesFromJsonFile(const std::string& filePath) {
	JsonSerializationArchive archive = JsonSerializationArchive(SerializationMode::Deserialize);
	archive.Load(filePath);

	LoadedTextures.clear();
	auto texturesToLoad = nlohmann::json::array();
	texturesToLoad = (*archive.m_pCurrentJsonObject)["LoadedTextures"];

	std::cout << "\n";
	for ( auto it:texturesToLoad.items() )
	{
		const auto& textureDetailObject = (*it.value().begin()); 
		std::cout << it.key() << " | " << it.value() << " | " << textureDetailObject["Path"] << "\n";
		LoadImageAsTexture(textureDetailObject["Path"], textureDetailObject["Name"]);
	}

	//still needs a check for skybox or other texture types
}




