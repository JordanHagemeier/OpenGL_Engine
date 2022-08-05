#include "ShaderManager.h"


std::vector<Shader> ShaderManager::m_Shaders = *InitShaderVector();
std::vector<Shader>* ShaderManager::InitShaderVector() {
	std::vector<Shader>* newVector =  new std::vector<Shader>();
	return newVector;
}


Shader ShaderManager::GetShaderByIdentifier(const std::string& identifier) {
	return shaderPrograms[identifier];
}

Shader ShaderManager::GetShaderByIndex(int i) {
	return ShaderManager::m_Shaders[i];
}


Shader ShaderManager::LoadShaderAs(const std::string& fileName, std::string identifier) {

	std::string vert = ".vert";
	std::string frag = ".frag";

	std::cout << fileName << std::endl;

	std::string vertexFileName = fileName;
	vertexFileName.append(vert);

	std::string fragmentFileName = fileName;
	fragmentFileName.append(frag);

	///////////////////////////////////////////////////////////////////////////////////

	//load shader files, vertex AND fragment 

	//VERTEX SHADER
	std::string fileOutput;
	fileOutput.clear();
	std::ifstream shaderFile;
	shaderFile.clear();

	shaderFile.open(vertexFileName, std::ios::ate);
	std::stringstream fbuffer;
	shaderFile.seekg(0);
	fbuffer << shaderFile.rdbuf();
	shaderFile.close();


	fileOutput = fbuffer.str();
	fbuffer.str("");
	const char* fileOutputPtrVertex = fileOutput.c_str();
	std::cout << fileOutput.c_str() << std::endl;

	///////////////////////////////////////////////////////////////////////////////////

	////FRAGMENT SHADER

	std::string fileOutputFragment;
	shaderFile.open(fragmentFileName, std::ios::ate);
	shaderFile.seekg(0);
	fbuffer << shaderFile.rdbuf();
	shaderFile.close();


	fileOutputFragment = fbuffer.str();
	const char* fileOutputPtrFragment = fileOutputFragment.c_str();
	std::cout << fileOutputFragment.c_str() << std::endl;

	///////////////////////////////////////////////////////////////////////////////////
	////compile shaders

	//vertex Shader
	GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &fileOutputPtrVertex, NULL);
	glCompileShader(vertexShader);

	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	/*shaders.push_back(vertexShader);*/

	///////////////////////////////////////////////////////////////////////////////////

	//fragment shader compilation 
	GLint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fileOutputPtrFragment, NULL);
	glCompileShader(fragShader);

	int  successFrag;
	char infoLogFrag[512];
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &successFrag);

	if (!successFrag)
	{
		glGetShaderInfoLog(fragShader, 512, NULL, infoLogFrag);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLogFrag << std::endl;
	}

	/*shaders.push_back(fragShader);*/


	///////////////////////////////////////////////////////////////////////////////////
	//create shader program
	GLint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);

	int  successProgram;
	char infoLogProgram[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &successProgram);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLogProgram);
		std::cout << "ERROR::SHADER::PROGRAM::LINKINGs_FAILED\n" << infoLogProgram << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);


	Shader shader(shaderProgram, identifier);

	std::cout << "NEW SHADER WITH NAME " << identifier << " AND INDEX OF " << shaderProgram << std::endl;
	shaderPrograms[identifier] = shader;

	m_Shaders.push_back(shader);
	return shader;

}

void ShaderManager::SetBoolInUniform(Shader shader, std::string& uniformName, bool value) {
	glUseProgram(shader.shaderIndex);
	glUniform1i(glGetUniformLocation(shader.shaderIndex, uniformName.c_str()), (int)value);
	glUseProgram(0);
}

void ShaderManager::SetFloatInUniform(Shader shader, const std::string uniformName, float value) {
	glUseProgram(shader.shaderIndex);
	glUniform1f(glGetUniformLocation(shader.shaderIndex, uniformName.c_str()), (float)value);
	glUseProgram(0);
}

void ShaderManager::SetIntInUniform(Shader shader, std::string& uniformName, int value) {
	glUseProgram(shader.shaderIndex);
	glUniform1i(glGetUniformLocation(shader.shaderIndex, uniformName.c_str()), value);
	glUseProgram(0);
}

void ShaderManager::SetMat4InUniform(Shader shader, std::string uniformName, GLint count, glm::mat4x4& value) {
	glUseProgram(shader.shaderIndex);
	GLint location = glGetUniformLocation(shader.shaderIndex, uniformName.c_str());

	glUniformMatrix4fv(location, count, GL_FALSE, glm::value_ptr(value));

	glUseProgram(0);
}

void ShaderManager::SetVec4InUniform(Shader shader, std::string uniformName, glm::vec4 value) {
	glUseProgram(shader.shaderIndex);
	GLint location = glGetUniformLocation(shader.shaderIndex, uniformName.c_str());

	glUniform4f(location, value.x, value.y, value.z, value.w);
	glUseProgram(0);

}

void ShaderManager::SetVec3InUniform(Shader shader, std::string uniformName, glm::vec3 value) {
	glUseProgram(shader.shaderIndex);
	GLint location = glGetUniformLocation(shader.shaderIndex, uniformName.c_str());

	glUniform3f(location, value.x, value.y, value.z);
	glUseProgram(0);
}


void ShaderManager::SetMat4InUniformWithCurrentProgram(Shader shader, std::string uniformName, GLint count, glm::mat4x4& value) {

	glUseProgram(shader.shaderIndex);
	GLint location = glGetUniformLocation(shader.shaderIndex, uniformName.c_str());

	glUniformMatrix4fv(location, count, GL_FALSE, glm::value_ptr(value));
}