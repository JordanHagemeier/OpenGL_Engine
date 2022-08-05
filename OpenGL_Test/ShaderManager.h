#pragma once
#include "Shader.h"
#include "GLFW/glfw3.h"
class ShaderManager {

public:

	

	static std::map<std::string, Shader> shaderPrograms;
	static std::vector<GLuint> m_ShaderInts;
	static std::vector<Shader> m_Shaders;

	static std::vector<Shader>* InitShaderVector();
	static Shader GetShaderByIdentifier(const std::string& identifier);
	static Shader GetShaderByIndex(int i);
	static Shader LoadShaderAs(const std::string& fileName, std::string identifier);

	static void SetBoolInUniform(Shader shader, std::string& uniformName, bool value);
	static void SetFloatInUniform(Shader shader, const std::string uniformName, float value);
	static void SetIntInUniform(Shader shader, std::string& uniformName, int value);
	static void SetMat4InUniform(Shader shader, std::string uniformName, GLint count, glm::mat4x4& value);
	static void SetVec4InUniform(Shader shader, std::string uniformName, glm::vec4 value);
	static void SetVec3InUniform(Shader shader, std::string uniformName, glm::vec3 value);
	static void SetMat4InUniformWithCurrentProgram(Shader shader, std::string uniformName, GLint count, glm::mat4x4& value);

	
};


