#pragma once

#ifndef GLAD
#define GLAD
#include <glad/glad.h>
#endif

#ifndef GLFW
#define GLFW
#include <GLFW/glfw3.h>
#endif



#include "Vertex.h"
#include "Mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RenderingComponent.h"
#include "GameObject.h"

struct Light {
	glm::vec3 position;
	glm::vec4 color;
	float intensity;
	float radius;
};





Vertex sRVertices[4] = {
	//positions						//uvs						//normals
	glm::vec3(-1.0f, -1.0f, 0.0f),	glm::vec2(0.0f, 0.0f),		glm::vec3(0.0f, 0.0f, -1.0f),	//top left
	glm::vec3(1.0f, -1.0f, 0.0f),	glm::vec2(1.0f, 0.0f),		glm::vec3(0.0f, 0.0f, -1.0f),	//top right
	glm::vec3(1.0f, 1.0f, 0.0f),	glm::vec2(1.0f, 1.0f),		glm::vec3(0.0f, 0.0f, -1.0f),	//bottom right
	glm::vec3(-1.0f, 1.0f, 0.0f),	glm::vec2(0.0f, 1.0f),		glm::vec3(0.0f, 0.0f, -1.0f),	//bottom left
};

std::vector<Vertex> screenRectangleVertices {
	sRVertices[0], sRVertices[1], sRVertices[2], sRVertices[3]
};

std::vector<int> screenRectangleIndices{
	0,1,2,
	0,2,3
};


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

void updateImGUI();
void createIMGUIGameObject();
void renderIMGUIGameObjectUI();
void createMousePositionInfoWindow();
void createCurrentIMGUIWindow(GameObject* currentGO, const char* name, const char* nameID, int id);
void createMenuBar();
void createPrimitiveMesh(int type);
void showCameraDirection();


void SetMVPMatrices(Shader* shader);
void OldSetMVPMatrices(std::string& shaderId );
void setLightingShader(std::string& shaderId, Light pLight);
void SetTextures(Material& material);
void setUpCamera();
void setMouseRotationBool(bool state);
void mouse_Callback(GLFWwindow* window, double xpos, double ypos);
void updateCamera();
void createCameraMovementVector();
void checkForEvents();

void moveCubesAroundPoint(std::string& shaderId, glm::vec3 midPoint, float radius, float offset);
//void loadImageAsTexture(unsigned int& texture, unsigned int& target, std::string& fileName);


void moveLight(float height, glm::vec3 midpoint, float radius);


//debug cube stuff
void setDebugCubePosition(GameObject go);
void checkRayBoundingBoxIntersection(GameObject gameObject);




void getMousePositionInWorldSpace(double mouseX, double mouseY);



//PBO stuff
void createPBOs();
void manipulatePBOData(GLubyte* src, GLubyte* dst);

//FBO stuff
void generateFBOandRBO();
void generateTextureForFBO();
void generateDepthTextureForFBO();
void setTexture(std::string& shaderId, GLint texture, GLenum textureUnit);
bool checkFramebufferStatus(GLuint fbo);
void deleteShaders();

//terrain generation stuff
float RandomFloat(float a, float b);
void generateTerrainPositions(float* posVec);
void addTerrainPositionsToManager(std::vector<glm::vec3>* posVec);

//shader and material setups
void SetUpDefaultMaterialAndShader(Shader& shader);


//Serialization
void SaveGame();
//void ResetGame();
void LoadGame();