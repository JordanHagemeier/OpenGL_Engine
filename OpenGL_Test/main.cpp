
#ifndef IOSTREAM
#include <iostream>
#define IOSTREAM
#endif // !IOSTREAM

#ifndef VECTOR
#include <vector>
#define VECTOR
#endif // !VECTOR

//for time measurment purposes
#include <chrono>


//rest of it
#include "TextureLoader.h"
#include <imgui.h>
#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_glfw.h>

#include "main_functions.h"
#include "Shader.h"
#include "Camera.h"
#include "Vertex.h"
#include <chrono>
#include "Mesh.h"
#include "CubeController.h"
#include "PrimitiveGenerator.h"
#include "MeshLoader.h"
#include "GameObjectManager.h"
#include "ComponentFactory.h"
#include "ShaderManager.h"
#include "GizmoManager.h"


#include "Serialization/JsonArchive.h"


////TERRAIN GENERATION
////#include "Terrain_Generator/VisualStudio/externals/OpenSimplexNoise/OpenSimplexNoise.hpp"
//#include "Terrain_Generator/VisualStudio/externals/OpenSimplexNoise_2/OpenSimplexNoise-master/OpenSimplexNoise-master/OpenSimplexNoise/OpenSimplexNoise.h"
//#include "Terrain_Generator/VisualStudio/externals/OpenSimplexNoise_2/OpenSimplexNoise-master/OpenSimplexNoise-master/OpenSimplexNoise/OpenSimplexNoise.cpp"



///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

const char* glsl_version			= "#version 430";

GLFWwindow* window;
const int WIDTH						= 800;
const int HEIGHT					= 600;
double cursorXPos;
double cursorYPos;


float mixingValue					= 0.5f;

///////////////////////////////////////////////////////////////////////////////////////////////////////////

GameObjectManager gameObjectManager;
PrimitiveGenerator primitiveGenerator;

std::vector<int> imGUIGameObjectIDs;
std::vector<Gizmo> imGUiGizmos;

PrimitiveInformation defaultPrimitiveInfo; 

GLuint vertexShader;
GLuint fragmentShader_a;
GLuint fragmentShader_b;
GLuint shaderProgram_a;
GLuint shaderProgram_b;




std::map<std::string, Shader> ShaderManager::shaderPrograms; //if I have a static member somewhere, it still needs an actual definition outside of the static class
std::vector<GLuint> ShaderManager::m_ShaderInts; 
std::vector<Camera> Cameras;

TextureLoader textureLoader;

Material* defaultMat;
Mesh* defaultMesh;

Material gizmoMat;

unsigned int null					= 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
std::string DEFAULTSHADERPATH		= "Shader/texturing_shader";
std::string SHADERPATH				= "Shader/vertexShader";
std::string SECONDSHADERPATH		= "Shader/Shader_2";
std::string THIRDSHADERPATH			= "Shader/texturing_shader";
std::string FOURTHSHADERPATH		= "Shader/simpleLightShader";
std::string SKYBOXSHADERPATH		= "Shader/SkyBoxShader";
std::string SBTEST					= "Shader/SBTest";
std::string GIZMOSHADERPATH			= "Shader/GizmoShader";
std::string SCREENEFFECTPATH		= "Shader/ScreenEffect";


std::string DEFAULTSHADERIDENTIFIER = "defaultShaderIdentifier";
///////////////////////////////////////////////////////////////////////////////////////////////////////////

//mouse to world space
glm::vec3 currentMousePosInWorldSpace = glm::vec3(0.0f);
glm::vec4 normalizedDeviceCoordinates = glm::vec4(0.0f);
int moduloMouseX;
int moduloMouseY;
float textureCoordinates[2];
float depthValues[4];

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//camera variables 
float stopStateTimer				= 0.0f;
double glfwTime						= 0;

int mouseX;
int mouseY;

float lastCursorXPosition			= 0.0f;
float lastCursorYPosition			= 0.0f;

float INITIALYAW					= -157.0f;
float INITIALPITCH					= -39.0f;

float cameraPitch					= INITIALPITCH;
float cameraYaw						= INITIALYAW;

bool firstMouseInput				= true;
bool changedMousePos;
bool mouseRotationActive			= true;

bool movementState_1				= false;
bool movementState_2				= false;
bool movementState_3				= false;
bool movementState_4				= false;
bool movementState_5				= false;
bool movementState_6				= false;
bool resetCamera					= false;


bool changeSpeed					= false;
bool endSpeedChange					= false;

bool addVelocity					= false;
float maxVelocity					= 10.0f;
float minVelocity					= 1.5f;
float boosterStrength				= 0.002f;

float currentVelocity				= minVelocity;

long timeSinceApplicationStart		= 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//PBO STUFF

static int index				= 0;
static int nextIndex			= 0;

const int PBO_COUNT				= 2;
GLuint pboIds[PBO_COUNT];           // IDs of PBOs
GLuint copyPBOIDs[PBO_COUNT]; 

const int CHANNEL_COUNT			= 4;
const int DATA_SIZE				= WIDTH * HEIGHT * CHANNEL_COUNT;
const GLenum PIXEL_FORMAT		= GL_RGBA;
GLubyte* colorBuffer			= 0;


///////////////////////////////////////////////////////////////////////////////////////////////////////////
//FBO STUFF

const int FBO_COUNT = 1;
GLuint fboIds[FBO_COUNT];
GLuint fboIds_2[FBO_COUNT];

const int RBO_COUNT = 1;
GLuint rboIds[RBO_COUNT];
GLuint rboIds_2[RBO_COUNT];

unsigned int textureId;
unsigned int textureColorbuffer;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//DEFAULT VALUES CAM

glm::mat4 defaultProjection = glm::mat4(glm::perspective(glm::radians(45.0f),(float) WIDTH /(float) HEIGHT, 0.1f, 1000.0f));
glm::mat4 defaultView		= glm::mat4(1.0f);

///////////////////////////////////////////////////////////////////////////////////////////////////////////

int SEGEMENTS = 20;
int POINTS_PER_DONUT = 10;


///////////////////////////////////////////////////////////////////////////////////////////////////////////
//LIGHTS
Light pointLight{
	glm::vec3(9.0f, 0.50f, 0.0f),		//position
	glm::vec4(0.8f, 0.2f, 0.5f, 1.0f),	//color
	4.0f,	//intensity
	6.0f	//radius
};


//Light Arrays
glm::vec3 lPos[2] = { glm::vec3(0.0f,3.0f, 0.0f), glm::vec3(5.0f, 8.0f, 0.0f)};
glm::vec3 lPosOriginal[2] = {lPos[0], lPos[1]};
glm::vec4 lColors[2] = { glm::vec4(1.0, 0.0, 0.0, 1.0), glm::vec4(0.0, 0.0, 1.0, 1.0)};
float lIntens[2] = { 4.0f, 3.0f};
float radi[2] = {6.0f,6.0f};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//TERRAIN GENERATION

glm::vec3 terrainDimensions = {glm::vec3(20.0f, 20.0f, 20.0f)};
std::vector<glm::vec3> generatedPositions;
const int amountOfGeneratedPositions = 10;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//GIZMOS

std::vector<Gizmo*> m_Gizmos;
GizmoManager m_GizmoManager;



static void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "* GL ERROR *" : ""),
		type, severity, message);
}

void TestOpenGlError()
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

void setMouseRotationBool(bool state) {
	mouseRotationActive = state;
	if (state == false) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		

	}
	if (state == true) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (glfwRawMouseMotionSupported()){
			glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
			}
		glfwGetCursorPos(window, &cursorXPos, &cursorYPos);
		lastCursorXPosition = cursorXPos;
		lastCursorYPosition = cursorYPos;
		
		mouse_Callback(window, cursorXPos, cursorYPos);
	}
}

void mouse_Callback(GLFWwindow* window, double xpos, double ypos) {

	mouseX = xpos;
	mouseY = ypos;

	if(mouseRotationActive){

	
	////get new cursor offset since last mouse position change
	float xoffset		= xpos - lastCursorXPosition;
	float yoffset		= lastCursorYPosition - ypos; // reversed since y-coordinates range from bottom to top
	lastCursorXPosition = xpos; //set last cursor position to current one 
	lastCursorYPosition = ypos;
	
	
	
	//multiply offset by sensitivity
	const float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	
	//update yaw and pitch variables with new values
	cameraYaw	+= xoffset;
	cameraPitch += yoffset;

	//std::cout <<cameraYaw << std::endl;
	//std::cout <<Cameras[0].yaw <<std::endl;

	//check if pitch (how high or low we're looking) is above or below our threshold (so we won't turn the camera weirdly)
	if (cameraPitch > 89.0f) {
		cameraPitch = 89.0f;
	}
	if (cameraPitch < -89.0f) {
		cameraPitch = -89.0f;
	}

	//set bool that indicates that the mouse position was changed
	changedMousePos = true;
	}

	
	
}



void checkForEvents() {

	//checks for booster function and velocity change
	if (addVelocity) {
		if (currentVelocity < maxVelocity) {
			currentVelocity += boosterStrength;
		}

	}

	if (!addVelocity) {
		if (currentVelocity > minVelocity) {
			currentVelocity -= boosterStrength;
		}
	}
	if (changeSpeed) {

		currentVelocity = 20.0f;
	}

	if (endSpeedChange) {

		endSpeedChange = !endSpeedChange;
		currentVelocity = minVelocity;

	}

	//mouse position changed, update camera values
	if (mouseRotationActive) {
		if (changedMousePos) {
			changedMousePos = false;
			Cameras[0].yaw = cameraYaw;
			Cameras[0].pitch = cameraPitch;
		}
	}

	//reset camera to inital position
	if (resetCamera) {
		resetCamera = false;
		Cameras[0].ResetCamera();
	}


	Cameras[0].velocity = currentVelocity;

}

void createCameraMovementVector() {

	glm::vec3 movementVector = glm::vec3(0.0,0.0,0.0);


	if (movementState_1) {
		movementState_1 = !movementState_1;
		movementVector += glm::vec3(0.0,0.0,1.0);
	}

	if (movementState_2) {
		movementState_2 = !movementState_2;
		movementVector += glm::vec3(0.0,0.0,-1.0);
	}

	if (movementState_3) {
		movementState_3 = !movementState_3;
		movementVector += glm::vec3(1.0,0.0,0.0);
	}

	if (movementState_4) {
		movementState_4 = !movementState_4;
		movementVector += glm::vec3(-1.0,0.0,0.0);
	}

	if (movementState_5) {
		movementState_5 = !movementState_5;
		movementVector += glm::vec3(0.0,1.0,0.0);
	}

	if (movementState_6) {
		movementState_6 = !movementState_6;
		movementVector += glm::vec3(0.0,-1.0,0.0);
	}

	Cameras[0].movementVec = movementVector;
}

void updateCamera() {

	checkForEvents();
	createCameraMovementVector();
	
	Cameras[0].UpdateViewMatrix();
	defaultView = Cameras[0].viewMatrix;
}

int main()
{
		
	

	std::cout << "Hello World" << std::endl;

	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	
	window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	setMouseRotationBool(true);

	glfwSetCursorPosCallback(window, mouse_Callback);
	//set initial cursor position
	lastCursorXPosition = WIDTH/2;
	lastCursorYPosition = HEIGHT/2;

	//enables OS specific OpenGL extensions, basically loads them via the glfw process/ the information about what OS we're using 
	//GLAD connects all function pointers of later versions or the version that we're using (of OpenGL) and our OS with the correct function names, so that we can use them
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}    
	
	std::vector<IComponent*> components;
	CubeControllerComponent* cubeController = new CubeControllerComponent();
	RenderingComponent* rendering = new RenderingComponent();
	components.push_back(cubeController);
	components.push_back(rendering);

	for (auto& component : components)
	{
		delete component;
	}
	components.clear();
	/////////////////////////////////////////////////////////////////////
	/*createPBOs();*/
	generateTextureForFBO();
	generateDepthTextureForFBO();
	generateFBOandRBO();
	TestOpenGlError();

	/////////////////////////////////////////////////////////////////////

	// Initialize OpenGL loader

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);


	/////////////////////////////////////////////////////////////////////////////

	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(MessageCallback, 0);
	
	glViewport(0, 0, WIDTH, HEIGHT); //need to set a viewport, window alone is not enough; this way we can actually manipulate how much of our window we would like to see & what
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	TestOpenGlError();

	/////////////////////////////////////////////////////////////////////////////
	//set up shaders and vertex data
	
	TestOpenGlError();
	//circle
	defaultPrimitiveInfo.position		= glm::vec3(0.0f, 0.0f, 0.0f);
	defaultPrimitiveInfo.points			= 20;
	defaultPrimitiveInfo.donutPoints	= 10;
	defaultPrimitiveInfo.radius			= 1.0f;
	defaultPrimitiveInfo.donutRadius	= 1.0f;
	defaultPrimitiveInfo.ringThickness	= 1.0f;


	PrimitiveInformation circleInfo				= PrimitiveGenerator::generatePrimitiveInfo(glm::vec3(0.0f, 0.0f, 0.0f), 2.0f, 0.0f, 10, 0,0.0f);
	Mesh* Circle									= PrimitiveGenerator::callFunctionByEnum(PrimitiveType::Circle, circleInfo);
												  
	//ring										  
	PrimitiveInformation ringInfo				= PrimitiveGenerator::generatePrimitiveInfo(glm::vec3(0.0f, 0.0f, 0.0f), 3.0f, 0.0f, 10, 0, 1.0f);
	Mesh* Ring									= PrimitiveGenerator::callFunctionByEnum(PrimitiveType::Ring, ringInfo);
	
												  
	//donut										  
	PrimitiveInformation donutInfo				= PrimitiveGenerator::generatePrimitiveInfo(glm::vec3(0.0f, 0.0f, 0.0f), 5.0f, 1.0f, SEGEMENTS + 1, POINTS_PER_DONUT, 0.0f);
	Mesh* Donut									= PrimitiveGenerator::callFunctionByEnum(PrimitiveType::Donut, donutInfo);
												  
	//the rotating cubes				
	PrimitiveInformation cubeAInfo				= PrimitiveGenerator::generatePrimitiveInfo(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 0.0f, 00, 0, 0.0f);
	PrimitiveInformation cubeBInfo				= PrimitiveGenerator::generatePrimitiveInfo(glm::vec3(0.0f, 0.0f, 0.0f), 0.9f, 0.0f, 00, 0, 0.0f);
	PrimitiveInformation cubeCInfo				= PrimitiveGenerator::generatePrimitiveInfo(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 0.0f, 00, 0, 0.0f);
	PrimitiveInformation DebugCubeInfo			= PrimitiveGenerator::generatePrimitiveInfo(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 0.0f, 00, 0, 0.0f);

	Mesh* Cube_a									= PrimitiveGenerator::callFunctionByEnum(PrimitiveType::Cube, cubeAInfo);
	Mesh* Cube_b									= PrimitiveGenerator::callFunctionByEnum(PrimitiveType::Cube, cubeBInfo);
	Mesh* Cube_c									= PrimitiveGenerator::callFunctionByEnum(PrimitiveType::Cube, cubeCInfo);

	//Debug Cube
	Mesh* DebugCube								= PrimitiveGenerator::callFunctionByEnum(PrimitiveType::Cube, DebugCubeInfo);

	//SkyBox Cube
	PrimitiveInformation SkyBoxCubeInfo			= PrimitiveGenerator::generatePrimitiveInfo(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 0.0f, 00, 0, 0.0f);
	Mesh* SkyBox									= PrimitiveGenerator::callFunctionByEnum(PrimitiveType::Cube, SkyBoxCubeInfo);

	//ScreenEffect Rectangle
	
	Mesh* ScreenRectangle						= MeshLoader::GenerateMesh(screenRectangleVertices, screenRectangleIndices);

	/*std::vector<Vertex> cubeVertices			= PrimitiveGenerator::createCube(1.0f, std::vector<float>{0.0f,0.0f,0.0f});
	std::vector<Vertex> cubeVertices_b			= PrimitiveGenerator::createCube(0.5f, std::vector<float>{0.0f,0.0f,0.0f});
	std::vector<Vertex> cubeVertices_c			= PrimitiveGenerator::createCube(1.0f, std::vector<float>{0.0f,0.0f,0.0f});*/


	//generate Meshes (also generates the VAO, VBO and EBO for each Mesh)
	/*Mesh Cube_a = MeshLoader::GenerateMesh(cubeVertices, PrimitiveGenerator::cube_indices);
	Mesh Cube_b = MeshLoader::GenerateMesh(cubeVertices_b, PrimitiveGenerator::cube_indices);
	Mesh Cube_c = MeshLoader::GenerateMesh(cubeVertices_c, PrimitiveGenerator::cube_indices);*/
	
	

	

	///////////////////////////////////////////////////////////////////////////////

	setUpCamera();
	defaultView								= Cameras[0].viewMatrix;
	gameObjectManager.viewMatrix			= &defaultView;
	gameObjectManager.m_ProjectionMatrix	= &defaultProjection;
	
	TestOpenGlError();
	///////////////////////////////////////////////////////////////////////////////

	std::string Shader1													= "Shader1";
	ShaderManager::LoadShaderAs(SHADERPATH, Shader1);

	std::string Texturing_Shader										= "Texturing_Shader";
	ShaderManager::LoadShaderAs(THIRDSHADERPATH, Texturing_Shader);

	std::string Light_Shader											= "simpleLightShader";
	ShaderManager::LoadShaderAs(FOURTHSHADERPATH, Light_Shader);

	std::string SkyBoxShader											= "SkyBoxShader";
	ShaderManager::LoadShaderAs(SKYBOXSHADERPATH, SkyBoxShader);

	std::string SBTestname												= "SBTest";
	ShaderManager::LoadShaderAs(SBTEST, SBTestname);

	std::string GizmoShader												= "GizmoShader";
	ShaderManager::LoadShaderAs(GIZMOSHADERPATH, GizmoShader);

	std::string ScreenEffectShader										= "ScreenEffect";
	ShaderManager::LoadShaderAs(SCREENEFFECTPATH, ScreenEffectShader);

	TestOpenGlError();
	

	//set gizmoMaterial
	gizmoMat.shader					= ShaderManager::GetShaderByIdentifier(GizmoShader);
	
	////////////////////////////////////////
	Material SkyBoxMaterial;
	SkyBoxMaterial.shader			= ShaderManager::GetShaderByIdentifier(SkyBoxShader);


	Material SBTestMaterial;
	SBTestMaterial.shader			= ShaderManager::GetShaderByIdentifier(SBTestname);

	Material TexturingShaderMat;
	TexturingShaderMat.shader		= ShaderManager::GetShaderByIdentifier(Texturing_Shader);


	TestOpenGlError();
	///////////////////////////////////////////////////////////////////////////////

	Shader shader			= ShaderManager::GetShaderByIdentifier(Shader1);
	glm::vec4 color			= glm::vec4(1.0, 1.0, 1.0, 1.0);

	glUseProgram(4);
	GLint location			= glGetUniformLocation(4,"ourColor");

	glUniform4f(location, color.x, color.y, color.z, color.w);
	glUseProgram(0);

	ShaderManager::SetFloatInUniform(shader, "radius", 10.0f);

	///////////////////////////////////////////////////////////////////////////////

	Shader SBshader			= ShaderManager::GetShaderByIdentifier(SBTestname);

	glm::mat4 model					= glm::mat4(1.0f);
	glm::mat4 view;
	view							= defaultView;

	glm::mat4 SBprojection;
	SBprojection = defaultProjection;

	ShaderManager::SetMat4InUniform(SBshader, "model", 1, model);
	ShaderManager::SetMat4InUniform(SBshader, "view", 1, view);
	ShaderManager::SetMat4InUniform(SBshader, "projection", 1, SBprojection);

	TestOpenGlError();
	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////////////////////////

	//load textures from images
	Texture texture1;						
	Texture texture2;							
	std::string imagePath								= "images/container.jpg";
	std::string secondImagePath							= "images/awesomeface.png";

	/*TextureLoader::LoadTextureToTarget(texture1, VAO_a, imagePath, GL_TEXTURE0);
	TextureLoader::LoadTextureToTarget(texture2, VAO_a, secondImagePath, GL_TEXTURE1);*/
	texture1 = TextureLoader::LoadImageAsTexture(imagePath, std::string("container"));
	texture2 = textureLoader.LoadImageAsTexture(secondImagePath, std::string("awesomeface"));
	std::string tex1							= "texture1";
	std::string tex2							= "texture2";
	ShaderManager::SetIntInUniform(ShaderManager::GetShaderByIdentifier(Texturing_Shader), tex1 , 0);
	ShaderManager::SetIntInUniform(ShaderManager::GetShaderByIdentifier(Texturing_Shader), tex2 , 1);

	std::string SkyBoxTexName			= "images/skybox/skybox/";
	textureLoader.LoadCubemap(SkyBoxTexName, "skybox");
	std::string skybox =  "skybox";
	ShaderManager::SetIntInUniform(SBTestMaterial.shader, skybox, 0);
	
	std::string tex3							= "depthTexture";
	ShaderManager::SetIntInUniform(ShaderManager::GetShaderByIdentifier(ScreenEffectShader), tex3, 0);

	std::string tex4							= "colorTexture";
	ShaderManager::SetIntInUniform(ShaderManager::GetShaderByIdentifier(ScreenEffectShader), tex4, 1);

	TestOpenGlError();



	//Material Construction
	//

	Material Shader1_Material;
	Shader1_Material.shader			= ShaderManager::GetShaderByIdentifier(Shader1);
	

	

	//texturing Material 
	Material TexturingMaterial;
	TexturingMaterial.shader = ShaderManager::GetShaderByIdentifier(Texturing_Shader);
	TexturingMaterial.textures.push_back(texture1);
	TexturingMaterial.textures.push_back(texture2);
	TexturingMaterial.shaderValues["value"] = mixingValue;


	//set default material
	defaultMat = new Material();
	defaultMat->textures.push_back(texture1);
	defaultMat->textures.push_back(texture2);
	defaultMesh = Cube_b;
	defaultMat->shaderValues["value"] = mixingValue;

	ShaderManager::LoadShaderAs(DEFAULTSHADERPATH, DEFAULTSHADERIDENTIFIER);
	ShaderManager::SetIntInUniform(ShaderManager::GetShaderByIdentifier(DEFAULTSHADERIDENTIFIER), tex1, 0);
	ShaderManager::SetIntInUniform(ShaderManager::GetShaderByIdentifier(DEFAULTSHADERIDENTIFIER), tex2, 1);


	defaultMat->shader = ShaderManager::GetShaderByIdentifier(DEFAULTSHADERIDENTIFIER);
	TestOpenGlError();
	
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	const std::string GO_name			= "CubeB_GO";
	std::vector<IComponent*> untexturedCube_Comps;
		
	RenderingComponent* CubeA_RenderingComponent = new RenderingComponent(defaultView, defaultProjection, defaultMat, Cube_a);

	CubeControllerComponent* CubeA_CubeController = new CubeControllerComponent();

	untexturedCube_Comps.push_back(CubeA_CubeController);
	untexturedCube_Comps.push_back(CubeA_RenderingComponent);


	GameObject* untexturedCube = gameObjectManager.CreateGameObject();
	untexturedCube->setGameObjectName(GO_name);

	for (IComponent* comp : untexturedCube_Comps) {
		untexturedCube->addComponent(comp);
		std::cout <<typeid(comp).name() << " is the type of component" << std::endl;
	}

	TestOpenGlError();

	///////////////////////////////////////////////////////////////////////
	const std::string Donut_Name		= "Donut";
	std::vector<IComponent*> Donut_Comps;

	RenderingComponent* Donut_RenderingComponent = new RenderingComponent(defaultView, defaultProjection, &Shader1_Material, Donut);
	
	Donut_Comps.push_back(Donut_RenderingComponent);

	GameObject* donut = gameObjectManager.CreateGameObject();
	donut->setGameObjectName(Donut_Name);
	for (IComponent* comp : Donut_Comps) {
		donut->addComponent(comp);
		std::cout <<typeid(comp).name() << " is the type of component" << std::endl;
	}

	///////////////////////////////////////////////////////////////////////

	//oh lord
	//THIS IS A CUBE, NOT A RING
	const std::string texturedCubeName			= "TexturedCube";
	
	std::vector<IComponent*> TexturedCube_Comps;

	RenderingComponent* TexturedCube_RenderingComponent = new RenderingComponent(defaultView, defaultProjection, &TexturingMaterial, Cube_b);
	
	TexturedCube_Comps.push_back(TexturedCube_RenderingComponent);

	GameObject* texturedCube = gameObjectManager.CreateGameObject();
	texturedCube->setGameObjectName(texturedCubeName);
	for (IComponent* comp : TexturedCube_Comps) {
		texturedCube->addComponent(comp);
		std::cout <<typeid(comp).name() << " is the type of component" << std::endl;
	}



	/////////////////////////////////////////////////////////////////////////////

	//GIZMO PLAYGROUND
	
	m_GizmoManager.CreateAllGizmos(gameObjectManager, ShaderManager::GetShaderByIdentifier(GizmoShader));


	//////////////////////////////////////////////////////////////////////////////

	////TERRAIN GENERATION 
	//
	//float* pointerToPosArray = new float[amountOfGeneratedPositions];
	//
	//generateTerrainPositions(pointerToPosArray);
	//addTerrainPositionsToManager(&generatedPositions);
	
	///////////////////////////////////////////////////////////////////////////////



	textureLoader.CreateJsonMapOfTextures();

	textureLoader.LoadTexturesFromJsonFile(std::string("TexturesToLoad"));
	while(!glfwWindowShouldClose(window))
	{
		glfwTime = glfwGetTime();
		//auto start = std::chrono::high_resolution_clock::now(); //start time to calculate tick time

		glfwPollEvents();  
		// get em input
		processInput(window);
		
		updateCamera();


		glEnable(GL_MULTISAMPLE);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//render stuff



		TestOpenGlError();	

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		TestOpenGlError();	
		
		////////////////////////////

		

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// oof time to try fbo instead!
		// 1. bind my own fbo and the window fbo
		// 2. blit the window fbo to my own fbo (color & depth for this test)
		// 3. do some cool shabalagalingo with my own fbo, maybe change the color idk
		// 4. put a rectangle in front of my camera that will  always stay in front of the camera rotation wise, translation wise, etc. 
		//		-> (this step needs to be done before everything, gotta create it before the mainloop)
		// 5. put texture shader on rectangle, render own fbo texture to rectangle 

		if (checkFramebufferStatus(fboIds[0]) && checkFramebufferStatus(0)) {

			//setting up the lights
			moveLight(1.0f, lPosOriginal[0], 3.0f);
			setLightingShader(Shader1, pointLight);

			//setting up the gizmo shader, as it does not have an ordinary rendering component I guess 			
			SetMVPMatrices(&ShaderManager::GetShaderByIdentifier(GizmoShader));
			TestOpenGlError();

			//////////////////////////////////////////////////////////////

			glBindFramebuffer(	GL_FRAMEBUFFER, fboIds[0]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);
			glUseProgram(ShaderManager::shaderPrograms[SBTestname].shaderIndex);
			glm::mat4 view = glm::mat4(glm::mat3(defaultView));

			ShaderManager::SetMat4InUniformWithCurrentProgram(ShaderManager::shaderPrograms[SBTestname], "view", 1, view);
			glBindVertexArray(SkyBox->VAO);
			glDrawElements(GL_TRIANGLES, SkyBox->indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			glUseProgram(0);
			glDepthMask(GL_TRUE);

			gameObjectManager.updateGameObjects();



			checkFramebufferStatus(fboIds[0]);
			checkFramebufferStatus(0);
			glBindFramebuffer(	GL_FRAMEBUFFER, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			glBindFramebuffer(	GL_FRAMEBUFFER, fboIds[0]);

			//////TODO!!!! 
			getMousePositionInWorldSpace(mouseX, mouseY);
			//i need to get the mouse position vector via game object manager to update it, maybe variable in each game object??
			
			
			

			//draw debug cube with mouse world pos coordinates
			glUseProgram(ShaderManager::GetShaderByIdentifier(Shader1).shaderIndex);
			glm::mat4 debugModelMat = glm::mat4(1.0f);
			debugModelMat = glm::translate(debugModelMat, currentMousePosInWorldSpace);
			ShaderManager::SetMat4InUniformWithCurrentProgram(ShaderManager::GetShaderByIdentifier(Shader1), "model", 1,debugModelMat);
			glBindVertexArray(DebugCube->VAO);
			glDrawElements(GL_TRIANGLES, DebugCube->indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			glUseProgram(0);

			glBindFramebuffer(	GL_FRAMEBUFFER, 0);

			
		}
		checkFramebufferStatus(fboIds[0]);
		checkFramebufferStatus(0);
		

		TestOpenGlError();


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		glUseProgram(ShaderManager::shaderPrograms[ScreenEffectShader].shaderIndex);
		glBindVertexArray(ScreenRectangle->VAO);
		glDisable(GL_DEPTH_TEST);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		
		glDrawElements(GL_TRIANGLES, ScreenRectangle->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glUseProgram(0);

		TestOpenGlError();

		glClear(GL_DEPTH_BUFFER_BIT);

		TestOpenGlError();
		m_GizmoManager.RenderAllGizmos();

		updateImGUI();
		
		//check for events
		glfwSwapBuffers(window);

		  
	}


	/////////////////////////////////////////////////////////////////////////////
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

	return 0;
}




void framebuffer_size_callback(GLFWwindow* window, int width, int height) //resizing
{
	glViewport(0, 0, width, height);
} 

void processInput(GLFWwindow *window)
{
	stopStateTimer--;
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
		}


	//texture mixing 
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		mixingValue += 0.01f;
		
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		mixingValue -= 0.01f;
		
	}

	//Camera Movement Values and bools

	int w_state = glfwGetKey(window, GLFW_KEY_W);
	int s_state = glfwGetKey(window, GLFW_KEY_S);
	int d_state = glfwGetKey(window, GLFW_KEY_D);
	int a_state = glfwGetKey(window, GLFW_KEY_A);
	int q_state = glfwGetKey(window, GLFW_KEY_Q);
	int e_state = glfwGetKey(window, GLFW_KEY_E);
	int f_state = glfwGetKey(window, GLFW_KEY_F);
	int shift_state = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
	int i_state = glfwGetKey(window, GLFW_KEY_I);


	int p_state = glfwGetKey(window, GLFW_KEY_P);
	int l_state = glfwGetKey(window, GLFW_KEY_L);


	if (w_state == GLFW_PRESS)
	{

		movementState_1 = true;
	}

	if (s_state == GLFW_PRESS)
	{

		movementState_2 = true;
	}

	if (d_state == GLFW_PRESS)
	{

		movementState_3 = true;
	}


	if (a_state == GLFW_PRESS)
	{

		movementState_4 = true;
	}


	if (q_state == GLFW_PRESS)
	{

		movementState_5 = true;
	}


	if (e_state == GLFW_PRESS)
	{

		movementState_6 = true;
	}

	/////////////////////////////////////////////////////////////////////////
	//basically shift but its on press f
	if (f_state == GLFW_PRESS) {
		changeSpeed = true;
	}

	if (changeSpeed == true && f_state == GLFW_RELEASE) {
		endSpeedChange = true;
		changeSpeed = false;
	}

	/////////////////////////////////////////////////////////////////////////

	//Jetpack Joyride 2.0
	if (shift_state == GLFW_PRESS) {
		addVelocity = true;
	}

	if (shift_state == GLFW_RELEASE) {
		addVelocity = false;

	}


	//////////////////////////////////////////////////////////////////////////
	int space_state = glfwGetKey(window, GLFW_KEY_SPACE);
	if (space_state == GLFW_PRESS) {
		resetCamera = true;
	}

	if (i_state == GLFW_PRESS && stopStateTimer < 1.0f)
	{

		stopStateTimer = 100.0f;
		setMouseRotationBool(!mouseRotationActive);
	}

	int moveLightUp = glfwGetKey(window, GLFW_KEY_I);
	if (moveLightUp == GLFW_PRESS) {
		pointLight.position.y += 0.2f;
	}

	int moveLightDown = glfwGetKey(window, GLFW_KEY_K);
	if (moveLightDown == GLFW_PRESS) {
		pointLight.position.y -= 0.2f;
	}

	//SERIALIZATION

	if (p_state == GLFW_PRESS) {
		SaveGame();
	}

	if (l_state == GLFW_PRESS) {
		LoadGame();
	}
}

/////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////
//
//TODO Shader Deletion direkt nach Shader Program Erstellung??
//
//Material & Uniforms Jan fragen
void deleteShaders() { 
	for (GLint shader : ShaderManager::m_ShaderInts) {
		glDeleteShader(shader);
	}
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

//SHADER STUFF

void setUpCamera() {
	Camera cam(glm::vec3(5.5f,5.3f,2.0f),glm::vec3(0.0f,1.0f, 0.0f), glm::vec3(-3, 0, 0));
	cam.yaw		= INITIALYAW;
	cam.pitch	= INITIALPITCH;
	
	Cameras.push_back(cam);
}

void moveLight(float height, glm::vec3 midpoint, float radius) {
	glm::vec3 tempLightPos = glm::vec3(0.0);
	
	float radian					= (2 * glm::pi<float>() * (float)glfwGetTime());

	float cos_rad					= glm::cos(radian);
	float sin_rad					= glm::sin(radian);

	tempLightPos					= glm::vec3(midpoint.x + cos_rad * radius, midpoint.y + height , midpoint.z +  sin_rad * radius);

	
	lPos[0]							= tempLightPos;

}

void moveCubesAroundPoint(std::string& shaderId, glm::vec3 midPoint, float radius, float offset) {
	
	Shader shader					= ShaderManager::GetShaderByIdentifier(shaderId);
	glm::mat4 model					= glm::mat4(1.0f);

	float radian					= (2 * glm::pi<float>() * ((float)glfwGetTime()/10) + glm::radians(offset));

	float cos_rad					= glm::cos(radian);
	float sin_rad					= glm::sin(radian);

	float newX						= (midPoint.x + cos_rad) * radius;
	float newY						= (midPoint.y + sin_rad) * radius;

	
	model							= glm::translate(model, glm::vec3(newX, newY, midPoint.z));

	model							= glm::rotate(model, glm::radians(30.0f) * (float)glfwGetTime(), glm::vec3(0.0f,0.0f,1.0f));

	ShaderManager::SetMat4InUniform(shader, "model", 1, model);

}

void SetMVPMatrices(Shader* shader) {
	
	glm::mat4 model					= glm::mat4(1.0f);
	glm::mat4 view					= defaultView;
	glm::mat4 projection			= defaultProjection;

	ShaderManager::SetMat4InUniform(*shader, "model", 1, model);
	ShaderManager::SetMat4InUniform(*shader, "view", 1, view);
	ShaderManager::SetMat4InUniform(*shader, "projection", 1, projection);

	TestOpenGlError();
}





void setLightingShader(std::string& shaderId, Light pLight){
	Shader shader = ShaderManager::GetShaderByIdentifier(shaderId);
	ShaderManager::SetVec3InUniform(shader, "lightPos", pLight.position);
	TestOpenGlError();
	ShaderManager::SetVec4InUniform(shader, "lightColor", pLight.color);
	TestOpenGlError();
	ShaderManager::SetFloatInUniform(shader, "lightIntensity",pLight.intensity);
	TestOpenGlError();
	ShaderManager::SetFloatInUniform(shader, "radius", pLight.radius);

	glUseProgram(shader.shaderIndex);
	GLint location_lPos				= glGetUniformLocation(shader.shaderIndex, "lightPositions");
	GLint location_radi				= glGetUniformLocation(shader.shaderIndex, "radiuses");
	GLint location_col				= glGetUniformLocation(shader.shaderIndex, "lightColors");
	GLint location_intens = glGetUniformLocation(shader.shaderIndex, "lightIntensities");
	glUniform3fv(location_lPos, 2, glm::value_ptr(lPos[0]));
	glUniform4fv(location_col, 2, glm::value_ptr(lColors[0]));
	glUniform1fv(location_radi, 2, &radi[0]);
	glUniform1fv(location_intens, 2, &lIntens[0]);
	glUseProgram(0);
	TestOpenGlError();


}


void setTexture(std::string& shaderId, GLint texture, GLenum textureUnit) {

	Shader shader	= ShaderManager::GetShaderByIdentifier(shaderId);
	glUseProgram(shader.shaderIndex);
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUseProgram(0);

	TestOpenGlError();
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

//IMGUI 

void updateImGUI() {
	
	// render your GUI
	ImGui::Begin("Demo window");
	/*ImGui::Text("Tick duration is %i", duration.count());*/
	if (ImGui::Button("Create New GameObject!")) {
		createIMGUIGameObject();
	}

	ImGui::End();

	renderIMGUIGameObjectUI();

	// Render dear imgui into screen
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

void createIMGUIGameObject() {
	RenderingComponent* defaultRenderingComponent = static_cast<RenderingComponent*>(ComponentFactory::CreateComponent(ComponentType::RenderingComponent));
	defaultRenderingComponent->setMaterial(defaultMat);
	defaultRenderingComponent->setMesh(defaultMesh);

	

	GameObject* newGameObject	= gameObjectManager.CreateGameObject();
	newGameObject->addComponent(defaultRenderingComponent);
	
	//create gizmo for game object
	Gizmo* currentGizmo			= m_GizmoManager.CreateGizmo(newGameObject, gizmoMat.shader);

	imGUiGizmos.push_back(*currentGizmo);
	


}

void renderIMGUIGameObjectUI() {

	
	for (int i = 0; i < gameObjectManager.getCurrentGameObjectCount(); i++) {
		if (gameObjectManager.getGameObject(i) != nullptr) {
			/*int id = imGUIGameObjectIDs.at(i);*/
			GameObject* currentGO = gameObjectManager.getGameObject(i);
			std::string name_inString = currentGO->getGameObjectName();
			/*std::string nameWithID			= name_inString.append(std::to_string(id));*/
			const char* name = &name_inString[0];
			const char* nameID = /*&nameWithID[0];*/ name;

			createCurrentIMGUIWindow(currentGO, name, nameID, i);
		}
		
	};

	ImGui::Begin("Gizmos");
	for (int j = 0; j < imGUiGizmos.size(); j++) {
		ImGui::Text("GameObject: %s", imGUiGizmos.at(j).ownGameObject->getGameObjectName());
		ImGui::Text("Position: %f, %f, %f", imGUiGizmos.at(j).translation.x, imGUiGizmos.at(j).translation.y, imGUiGizmos.at(j).translation.z);
		ImGui::Text("GO Position: %f, %f, %f", imGUiGizmos.at(j).ownGameObject->m_Transform.x,imGUiGizmos.at(j).ownGameObject->m_Transform.y,imGUiGizmos.at(j).ownGameObject->m_Transform.z );
	}
	ImGui::End();

	createMousePositionInfoWindow();
	createMenuBar();
	showCameraDirection();

	ImGui::Begin("GameObjectManager");
	ImGui::Text("GOs in Vector: %i", gameObjectManager.getCurrentGOSize());
	for (int i = 0; i < gameObjectManager.getCurrentGOSize(); i++) {
		if (gameObjectManager.getGameObject(i) != nullptr) {
		GameObject* currentGO = gameObjectManager.getGameObject(i);
		ImGui::Text("GameObject is %s, ID: %i", currentGO->getGameObjectName().c_str(), currentGO->getIdentifier());
		ImGui::Text("Position is: %f = x, %f = y, %f = z", currentGO->m_Transform.x, currentGO->m_Transform.y, currentGO->m_Transform.z);
		}
	}
	ImGui::End();
}



void createCurrentIMGUIWindow(GameObject* currentGO, const char* name, const char* nameID, int id) {

	ImGui::Begin(nameID);
	ImGui::PushID(id);
	ImGui::Text("I am a new Game Object with this name: %s \n", name);

	float position[3]				= {currentGO->m_Transform.x, currentGO->m_Transform.y, currentGO->m_Transform.z};
	ImGui::SliderFloat3("Position", position, -10.0f, 10.0f);
	currentGO->setTransform(glm::vec3(position[0], position[1], position[2]));

	
	float size[3]					= {currentGO->m_Scale.x, currentGO->m_Scale.y, currentGO->m_Scale.z};
	ImGui::SliderFloat3("Size", size, -10.0f, 10.0f);
	currentGO->setScale(glm::vec3(size[0], size[1], size[2]));

	

	ImGui::PopID();
	ImGui::End();
}

void createMousePositionInfoWindow() {
	ImGui::Begin("Mouse Position Info");
	ImGui::Text("Time since Application Start: %f", glfwTime);
	ImGui::Text(" Screen position: %d = x, %d = y", mouseX, mouseY);
	ImGui::Text(" modulod Mouse Pos: %i = x, %i = y", moduloMouseX, moduloMouseY);
	ImGui::Text(" Normalized Device Coordinates:\n %f = x, %f = y, \n %f = z, %f = w", normalizedDeviceCoordinates.x, normalizedDeviceCoordinates.y, normalizedDeviceCoordinates.z, normalizedDeviceCoordinates.w);
	/*ImGui::Text(" Texture Coords: %f = x, %f = y", textureCoordinates[0], textureCoordinates[1]);
	ImGui::Text(" Depth Values read:\n %f = x, %f = y,\n %f = z, %f = w", depthValues[0], depthValues[1], depthValues[2], depthValues[3]);*/
	ImGui::Text(" Camera Transform: %f = x, %f = y, %f = z", Cameras[0].cameraPos.x, Cameras[0].cameraPos.y,  Cameras[0].cameraPos.z );
	ImGui::Text(" Mouse Position in Space: %f = x, %f = y, %f = z",currentMousePosInWorldSpace.x, currentMousePosInWorldSpace.y, currentMousePosInWorldSpace.z );
	ImGui::End();
}

void createPrimitiveMesh(int type) {
	Mesh* newPrimitive								=  PrimitiveGenerator::callFunctionByEnum((PrimitiveType)type, defaultPrimitiveInfo);
	MeshLoader::createBoundingBox(newPrimitive);

	
	RenderingComponent* defaultRenderingComponent	= new RenderingComponent;
	defaultRenderingComponent->setMaterial(defaultMat);
	defaultRenderingComponent->setMesh(newPrimitive);
	


	GameObject* newGameObject						= gameObjectManager.CreateGameObject();
	newGameObject->addComponent(defaultRenderingComponent);
	newGameObject->setGameObjectName(PrimitiveGenerator::primitiveNames[type]);

	
	//create gizmo for game object
	Gizmo* currentGizmo = m_GizmoManager.CreateGizmo(newGameObject, gizmoMat.shader);
	TestOpenGlError();
	imGUiGizmos.push_back(*currentGizmo);
}


void createMenuBar() {
	const char* ItemList[4] = { "Circle", "Ring", "Donut", "Cube"};
	if(ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if(ImGui::BeginMenu("Create Primitve"))
			{
				for (int i = 0; i < sizeof(ItemList) / sizeof(ItemList[0]); i++) {
					if (ImGui::MenuItem(ItemList[i])) {
						createPrimitiveMesh(i);
					}
				}
				//Do something
			ImGui::EndMenu();
			}

		ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
	
}

void showCameraDirection() {

	ImGui::Begin("Camera Debug");
	float yawPitch[2]	= {Cameras[0].yaw, Cameras[0].pitch};
	ImGui::SliderFloat2("Modify Yaw & Pitch",yawPitch, -180.0f, 180.0f);

	Cameras[0].yaw		= yawPitch[0];
	Cameras[0].pitch	= yawPitch[1];
	cameraYaw			= yawPitch[0];
	cameraPitch			= yawPitch[1];
	Cameras[0].UpdateViewMatrix();

	ImGui::End();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////

//TIME TO TRY OUT GIZMOS MY GUYS



///////////////////////////////////////////////////////////////7

void getMousePositionInWorldSpace(double mouseX, double mouseY) {

	glm::mat4 inverseViewProjection = glm::inverse( defaultView * defaultProjection); // in the shader we're multiplying projection with view, so to get the inverse, we gotta do this the other way around?
	glm::vec4 inverseVector;

	//calculate normalized device coordinates
	moduloMouseX				= (int)mouseX % WIDTH;
	moduloMouseY				= (int)mouseY % HEIGHT;
	if (moduloMouseX < 0) {
		moduloMouseX *= -1.0f;
	}
	if (moduloMouseY < 0){
		moduloMouseY *= -1.0f;
	}

	//normalized device coordinates
	inverseVector.x					= ((2.0f*(float)mouseX)/WIDTH) -1.0f;
	inverseVector.y					= 1.0f-((2.0f*(float)mouseY)/(HEIGHT));
	inverseVector.z					= 1.0f;
	inverseVector.w					= 1.0f;

	normalizedDeviceCoordinates = inverseVector;



	//calculate texture coordinates for framebuffer texture object(read the correct pixels corresponding to the mouse position)
	
	textureCoordinates[0]			= ((2.0f * (float)moduloMouseX)/WIDTH)/2.0f;
	textureCoordinates[1]			= (((-1.0f)/HEIGHT)* (float)moduloMouseY) + 1.0f;

	//read depth value from framebuffer via texture coords
	
	/*glReadPixels(textureCoordinates[0],textureCoordinates[1], 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depthValues);*/
	



	//homogeneous clip coordinates
	inverseVector.z					= -1.0f;


	/*glm::vec4 positionInWorldSpace = inverseVector * inverseViewProjection;*/


	//homogeneous clip coordinates to camera space (eye space)
	glm::vec4 homoToCameraSpace = glm::vec4(0.0f);
	homoToCameraSpace = glm::inverse(defaultProjection) * inverseVector;

	homoToCameraSpace = glm::vec4(homoToCameraSpace.x, homoToCameraSpace.y, -1.0f, 0.0f); //z = -1.0f so that we have a direction for the ray, rather than a point in space


	//camera space to world space 
	glm::vec3 cameraToWorldSpace = glm::vec3((glm::inverse(defaultView) * homoToCameraSpace));
	cameraToWorldSpace = glm::normalize(cameraToWorldSpace);


	//get current camera point just to try out
	glm::vec3 cameraMousePosInWorldSpace = Cameras[0].cameraPos + cameraToWorldSpace;


	

	//TODO WON'T NEED THIS ANYMORE
	glm::vec4 positionInWorldSpace = glm::vec4(0.0f);
	positionInWorldSpace = glm::inverse(defaultProjection) * inverseVector;
	positionInWorldSpace.w			= 1.0f/positionInWorldSpace.w;
	positionInWorldSpace.x			*= positionInWorldSpace.w;
	positionInWorldSpace.y			*= positionInWorldSpace.w;
	positionInWorldSpace.z			*= positionInWorldSpace.w;


	currentMousePosInWorldSpace = glm::vec3(cameraToWorldSpace);

	/*std::cout << positionInWorldSpace.x << ": x, " << positionInWorldSpace.y << ": y, " << positionInWorldSpace.z << ": z" << std::endl;*/
	//separate depth buffer? I really need to get this performance wise sorted, because readPixel aint gonna cut it


	/*std::cout << "Mouse Position: " << mouseX << " x," << mouseY << " y" << std::endl;*/
	/*std::cout << "Texture Position: " << textureCoordinates[0] << " x," << textureCoordinates[1] << " y" << std::endl;*/
	/*std::cout << "Mouse Position in Space: " << currentMousePosInWorldSpace.x << " x," << currentMousePosInWorldSpace.y << " y" << currentMousePosInWorldSpace.z << " z" << std::endl;*/
	/*std::cout<<" Normalized Device Coordinates: " << inverseVector.x << " x, " << inverseVector.y << " y" << std::endl;*/
	/*std::cout << "Depth: " << inverseVector.z <<std::endl;*/

}


void setDebugCubePosition(GameObject go) {
	
	//lol lets spice this up and try out moving the ray intersection check points through space along the direction
	glm::vec3 currentIntersectionCheckPos = glm::vec3(0.0f);
	int amountOfRayIntersectionChecksInOneDirection = 30;
	float rayIntersectionDistanceAddedEachCheck = 3.0f;

	/*for (int i = 0; i < amountOfRayIntersectionChecksInOneDirection; i++) {
		currentIntersectionCheckPos = Cameras[0].cameraPos + (currentMousePosInWorldSpace * rayIntersectionDistanceAddedEachCheck);
		currentMousePosInWorldSpace = currentIntersectionCheckPos;
	}*/

	int intGlfwTime = (int)glfwTime;
	currentIntersectionCheckPos = Cameras[0].cameraPos + (currentMousePosInWorldSpace * (rayIntersectionDistanceAddedEachCheck * (intGlfwTime % 10)));
	currentMousePosInWorldSpace = currentIntersectionCheckPos;

	
}


//void checkRayBoundingBoxIntersection(GameObject gameObject) {
//	//get gameobject for transform matrix
//	//get rendering component for mesh
//	//multiply bounding box from mesh with transform matrix 
//	//check in a loop for intersections between ray point and box 
//	
//
//	auto goRender = gameObject.getComponent<RenderingComponent>();
//	BoundingBox goBBox = goRender->getMesh()->boundingBox;
//
//
//	glm::vec3 goTransformVector = gameObject.transform;
//	glm::mat4x4 goTransformMat = glm::mat4x4(0.0f);
//	goTransformMat = glm::translate(goTransformMat,goTransformVector);
//
//	glm::vec4 firstBoundingBoxPoint = glm::vec4(goBBox.xMin, goBBox.yMin, goBBox.zMin, 1.0f);
//	glm::vec4 secondBoundingBoxPoint = glm::vec4(goBBox.xMax, goBBox.yMax, goBBox.zMax, 1.0f);
//
//	firstBoundingBoxPoint = goTransformMat * firstBoundingBoxPoint;
//	secondBoundingBoxPoint = goTransformMat * secondBoundingBoxPoint;
//
//
//}

void checkPointBoxIntersection(glm::vec3 point, BoundingBox box) {

}
///////////////////////////////////////////////
//PBO Functions

void createPBOs() {
	// create 2 pixel buffer objects, you need to delete them when program exits.
	// glBufferData() with NULL pointer reserves only memory space.
	glGenBuffers(PBO_COUNT, pboIds);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[0]);
	glBufferData(GL_PIXEL_PACK_BUFFER, DATA_SIZE, 0, GL_STREAM_READ);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, pboIds[1]);
	glBufferData(GL_PIXEL_PACK_BUFFER, DATA_SIZE, 0, GL_STREAM_READ);

	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);



	//create 2 pixel buffer objects that are used for manipulated data and which will upload to a texture

	glGenBuffers(PBO_COUNT, copyPBOIDs);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, copyPBOIDs[0]);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, DATA_SIZE, 0, GL_STREAM_DRAW);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, copyPBOIDs[1]);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, DATA_SIZE, 0, GL_STREAM_DRAW);

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	TestOpenGlError();

	colorBuffer = new GLubyte[DATA_SIZE];
	memset(colorBuffer, 255, DATA_SIZE);

	TestOpenGlError();
}


void manipulatePBOData(GLubyte* src, GLubyte* dst) {
	int count = WIDTH * HEIGHT;
	for (int i = 0; i < count; i++) {
		*dst = (GLubyte) 255;
		++dst;
		*dst = (GLubyte) 100;
		++dst;
		*dst = (GLubyte) 50;
		++dst;
		*dst = (GLubyte) 255;
		++dst;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////

void generateFBOandRBO() {
	glGenFramebuffers(FBO_COUNT, fboIds);
	glBindFramebuffer(GL_FRAMEBUFFER, fboIds[0]);
	
	glGenRenderbuffers(RBO_COUNT, rboIds);
	glBindRenderbuffer(GL_RENDERBUFFER, rboIds[0]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboIds[0]);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureId, 0);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	TestOpenGlError();
}

void generateTextureForFBO() {
	//// create a texture object
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);*/
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_2D, 0);

	/*glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	TestOpenGlError();*/

	
}

void generateDepthTextureForFBO() {
	//// create a texture object
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WIDTH, HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);

}



bool checkFramebufferStatus(GLuint fbo)
{
	// check FBO status
	glBindFramebuffer(GL_FRAMEBUFFER, fbo); // bind
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch(status)
	{
	case GL_FRAMEBUFFER_COMPLETE:
		/*std::cout << "Framebuffer complete." << std::endl;*/
		return true;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		std::cout << "[ERROR] Framebuffer incomplete: Attachment is NOT complete." << std::endl;
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		std::cout << "[ERROR] Framebuffer incomplete: No image is attached to FBO." << std::endl;
		return false;
		/*
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
		std::cout << "[ERROR] Framebuffer incomplete: Attached images have different dimensions." << std::endl;
		return false;

		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS:
		std::cout << "[ERROR] Framebuffer incomplete: Color attached images have different internal formats." << std::endl;
		return false;
		*/
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		std::cout << "[ERROR] Framebuffer incomplete: Draw buffer." << std::endl;
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		std::cout << "[ERROR] Framebuffer incomplete: Read buffer." << std::endl;
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		std::cout << "[ERROR] Framebuffer incomplete: Multisample." << std::endl;
		return false;

	case GL_FRAMEBUFFER_UNSUPPORTED:
		std::cout << "[ERROR] Framebuffer incomplete: Unsupported by FBO implementation." << std::endl;
		return false;

	default:
		std::cout << "[ERROR] Framebuffer incomplete: Unknown error." << std::endl;
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);   // unbind
}



//TERRAIN GENERATION 

//NOISE TEXTURE 2D GENERATION
//steps:
//0. setup  width & height for TG-Texture (Terrain Generation Texture)
// -> create vector of width and height dimensions x 4 to fill with color later
//1. generate texture (needs no data at this point) of width and height dimensions of TG Tex
//2. generate TG-Texture data via noise 
// -> glTexSubImage2D -> fill entire texture with new texture data from vector
//3. bind texture to 


//NOISE 3D POSITION GENERATION

//steps:
//0. setup xyz dimensions which the noise will use as boundaries
//1. create 3 dimensional vector for the positions
//2. set how many points should be generated
//3. generate positions
//4. take pos vector and create a cube for every pos, set the pos, render it

//void generateTerrainPositions(float* posVec) {
//	
//	int k = 0;
//	int j = 0;
//	int n = 0;
//	/*OpenSimplexNoise OpenSimpleNoiseInstance;
//	for (int i = 1; i <= amountOfGeneratedPositions; i++) {
//				k++;
//				j++;
//				glm::vec3 newPos;
//				newPos.x = OpenSimpleNoiseInstance.Evaluate(terrainDimensions.x /(amountOfGeneratedPositions * (i * 10)), terrainDimensions.y /(amountOfGeneratedPositions * k),terrainDimensions.z /(amountOfGeneratedPositions * j), glfwTime * rand()) * 10.0f;
//				newPos.y = OpenSimpleNoiseInstance.Evaluate(terrainDimensions.x /(amountOfGeneratedPositions * i), terrainDimensions.y /(amountOfGeneratedPositions * k),terrainDimensions.z /(amountOfGeneratedPositions * j)) * 10.0f;
//				newPos.z = OpenSimpleNoiseInstance.Evaluate(terrainDimensions.x /(amountOfGeneratedPositions * i), terrainDimensions.y /(amountOfGeneratedPositions * k),terrainDimensions.z /(amountOfGeneratedPositions * j), glfwTime * rand()) * 10.0f;
//
//				posVec->push_back(newPos);
//			
//	}*/
//
//
//	OpenSimplexNoise::Noise noise;
//	int GeneratedPositionsDividedByThree = amountOfGeneratedPositions/3;
//	for (int i = 0; i <= (GeneratedPositionsDividedByThree / 3); i + 3) {
//		k++;
//		j++;
//		n++;
//		glm::vec3 newPos;
//		newPos.x = noise.eval(terrainDimensions.x /(amountOfGeneratedPositions * n), terrainDimensions.y /(amountOfGeneratedPositions * k),terrainDimensions.z /(amountOfGeneratedPositions * j), RandomFloat(-terrainDimensions.x, terrainDimensions.x)) * terrainDimensions.x;
//		newPos.y = noise.eval(terrainDimensions.x /(amountOfGeneratedPositions * n), terrainDimensions.y /(amountOfGeneratedPositions * k),terrainDimensions.z /(amountOfGeneratedPositions * j), RandomFloat(-terrainDimensions.y, terrainDimensions.y)) * terrainDimensions.y;
//		newPos.z = noise.eval(terrainDimensions.x /(amountOfGeneratedPositions * n), terrainDimensions.y /(amountOfGeneratedPositions * k),terrainDimensions.z /(amountOfGeneratedPositions * j), RandomFloat(-terrainDimensions.z, terrainDimensions.z)) * terrainDimensions.z;
//
//		posVec[i]	= newPos.x;
//		posVec[i +1] = newPos.y;
//		posVec[i +2] = newPos.z;
//
//	}
//
//
//}

float RandomFloat(float a, float b) {
	float random = ((float) rand()) / (float) RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

void addTerrainPositionsToManager(std::vector<glm::vec3>* posVec) {

	for (int i = 0; i < amountOfGeneratedPositions; i++) {
		int thisID = gameObjectManager.getCurrentGameObjectCount();
		createIMGUIGameObject();
		gameObjectManager.getGameObject(thisID)->setTransform(posVec->at(i));
	}
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Setup default material and shader

void SetUpDefaultMaterialAndShader(Shader& shader) {
	//defaultMat->shader = ShaderManager::LoadShaderAs(DEFAULTSHADERPATH, DEFAULTSHADERIDENTIFIER);
	defaultMat->shader = shader;
	
}






////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Serialization

 void SaveGame() {

	JsonSerializationArchive archive =  JsonSerializationArchive(SerializationMode::Serialize);
	gameObjectManager.Serialize(archive);
	archive.Save("testSave");
}



//load game

 void LoadGame() {
	 JsonSerializationArchive archive = JsonSerializationArchive(SerializationMode::Deserialize);
	 archive.Load("testSave");
	/* archive.Serialize(gameObjectManager); das hier muss noch gebaut werden*/
	gameObjectManager.ClearGameObjects();
	imGUiGizmos.clear();
	m_Gizmos.clear();


	gameObjectManager.Serialize(archive);
	gameObjectManager.Initialize();
	m_GizmoManager.CreateAllGizmos(gameObjectManager, ShaderManager::GetShaderByIdentifier("GizmoShader"));


	 // For each manager: Serialize
	 // For each manager: InitializeAfterSerialize
	 
}