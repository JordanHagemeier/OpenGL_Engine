#include "PrimitiveGenerator.h"

std::map<PrimitiveType, FunctionPointer> PrimitiveGenerator::mappedFunctions = PrimitiveGenerator::InitPrimitiveFunctions();
FunctionPointer PrimitiveGenerator::functionPointer = NULL;

const char* PrimitiveGenerator::primitiveNames[4] = {"Circle", "Ring", "Donut", "Cube"};

std::map<PrimitiveType, FunctionPointer> PrimitiveGenerator::InitPrimitiveFunctions(){
	std::map<PrimitiveType, FunctionPointer>* newMap = new std::map<PrimitiveType, FunctionPointer>; 

	(*newMap)[PrimitiveType::Circle]		= &PrimitiveGenerator::createNGon;
	(*newMap)[PrimitiveType::Ring]			= &PrimitiveGenerator::createRingNGon;
	(*newMap)[PrimitiveType::Donut]			= &PrimitiveGenerator::createDonutNGon;
	(*newMap)[PrimitiveType::Cube]			= &PrimitiveGenerator::createCube;

	return *newMap;
}		 

Mesh* PrimitiveGenerator::callFunctionByEnum(PrimitiveType type, PrimitiveInformation pInfo) {
	if (static_cast<int>(type) < 0 || type > PrimitiveType::Count) {
		std::cout << "Type was not of Primitive Type";
		
	}
	FunctionPointer currentFunction = mappedFunctions[type];
	//functionPointer = pGenerator->mappedFunctions[type];

	Mesh* currentMesh = std::invoke(currentFunction, pInfo);
	return currentMesh;
}

Vertex PrimitiveGenerator::cubeVertices[72] = {
	//positions							//uvs						//normals
	//top quad
	glm::vec3(1.0f, 1.0f, 1.0f),		glm::vec2(1.0f, 0.0f),		glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, -1.0f),		glm::vec2(1.0f, 1.0f),		glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(-1.0f, 1.0f, -1.0f),		glm::vec2(0.0f, 1.0f),		glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),		glm::vec2(0.0f, 0.0f),		glm::vec3(0.0f, 1.0f, 0.0f),

	//right quad
	glm::vec3(1.0f, 1.0f, 1.0f),		glm::vec2(0.0f, 1.0f),		glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, -1.0f),		glm::vec2(1.0f, 1.0f),		glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, -1.0f, 1.0f),		glm::vec2(0.0f, 0.0f),		glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, -1.0f, -1.0f),		glm::vec2(1.0f, 0.0f),		glm::vec3(1.0f, 0.0f, 0.0f),

	//behind quad
	glm::vec3(1.0f, 1.0f, -1.0f),		glm::vec2(0.0f, 1.0f),		glm::vec3(0.0f, 0.0f, -1.0f),
	glm::vec3(-1.0f, 1.0f, -1.0f),		glm::vec2(1.0f, 1.0f),		glm::vec3(0.0f, 0.0f, -1.0f),
	glm::vec3(1.0f, -1.0f, -1.0f),		glm::vec2(0.0f, 0.0f),		glm::vec3(0.0f, 0.0f, -1.0f),
	glm::vec3(-1.0f, -1.0f, -1.0f),		glm::vec2(1.0f, 0.0f),		glm::vec3(0.0f, 0.0f, -1.0f),

	//left quad
	glm::vec3(-1.0f, 1.0f, -1.0f),		glm::vec2(0.0f, 1.0f),		glm::vec3(-1.0f, 0.0f, 0.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),		glm::vec2(1.0f, 1.0f),		glm::vec3(-1.0f, 0.0f, 0.0f),
	glm::vec3(-1.0f, -1.0f, -1.0f),		glm::vec2(0.0f, 0.0f),		glm::vec3(-1.0f, 0.0f, 0.0f),
	glm::vec3(-1.0f, -1.0f, 1.0f),		glm::vec2(1.0f, 0.0f),		glm::vec3(-1.0f, 0.0f, 0.0f),

	//front quad
	glm::vec3(-1.0f, 1.0f, 1.0f),		glm::vec2(0.0f, 1.0f),		glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),		glm::vec2(1.0f, 1.0f),		glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(-1.0f, -1.0f, 1.0f),		glm::vec2(0.0f, 0.0f),		glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(1.0f, -1.0f, 1.0f),		glm::vec2(1.0f, 0.0f),		glm::vec3(0.0f, 0.0f, 1.0f),

	//bottom quad
	glm::vec3(1.0f, -1.0f, 1.0f),		glm::vec2(1.0f, 0.0f),		glm::vec3(0.0f, -1.0f, 0.0f),
	glm::vec3(1.0f, -1.0f, -1.0f),		glm::vec2(1.0f, 1.0f),		glm::vec3(0.0f, -1.0f, 0.0f),
	glm::vec3(-1.0f, -1.0f, -1.0f),		glm::vec2(0.0f, 1.0f),		glm::vec3(0.0f, -1.0f, 0.0f),
	glm::vec3(-1.0f, -1.0f, 1.0f),		glm::vec2(0.0f, 0.0f),		glm::vec3(0.0f, -1.0f, 0.0f),

};

PrimitiveInformation PrimitiveGenerator::generatePrimitiveInfo(glm::vec3 pos, float rad, float donutrad, int points, int donutpoints, float ringthickness) {
	PrimitiveInformation* pInfo = new PrimitiveInformation;
	pInfo->position				= pos;
	pInfo->radius				= rad;
	pInfo->donutRadius			= donutrad;
	pInfo->points				= points;
	pInfo->donutPoints			= donutpoints;
	pInfo->ringThickness		= ringthickness;

	return *pInfo;
}

Mesh* PrimitiveGenerator::createNGon(PrimitiveInformation info) {

	//creation of vertices
	std::vector<Vertex>  positions;

	Vertex originalPosition;
	originalPosition.position	= glm::vec3(info.position[0],info.position[1],info.position[2] );
	originalPosition.uvs		= glm::vec2(0.0f, 0.0f);
	originalPosition.normals	= glm::vec3(0.0f, 1.0f, 0.0f);

	positions.push_back(originalPosition);



	//ring points
	for (int i = 0; i < info.points; i++) {

		float radian			= (2 * glm::pi<float>() / info.points) * i;

		float cos_rad			= glm::cos(radian);
		float sin_rad			= glm::sin(radian);

		Vertex currentVertex;
		currentVertex.position	= glm::vec3((info.position[0] + cos_rad) * info.radius,info.position[1], ( info.position[2] + sin_rad) * info.radius);
		currentVertex.uvs		= glm::vec2(0.0f, 0.0f);
		currentVertex.normals	= glm::vec3(0.0f, 1.0f, 0.0f);

		positions.push_back(currentVertex);


	}

	//creation of indices

	std::vector<int> indices = createCircleIndices(info);

	Mesh* newNGon = MeshLoader::GenerateMesh(positions, indices);
	newNGon->ownType = PrimitiveType::Circle;
	newNGon->ownInformation = info;
	return newNGon;
}


std::vector<int> PrimitiveGenerator::createCircleIndices(PrimitiveInformation info) {
	std::vector<int> circle_indices;
	for (int i = 0; i < info.points; i++) {
		circle_indices.push_back(0);
		circle_indices.push_back((i+1)% (info.points + 1));
		if (((i + 1) % (info.points +1)) >= info.points) {
			circle_indices.push_back(1);
		}
		else {
			circle_indices.push_back((i+2)% (info.points + 1));
		}

	}
	return circle_indices;
}

Mesh* PrimitiveGenerator::createRingNGon(PrimitiveInformation info) {

	std::vector<Vertex> ring_positions;

	for (int i = 0; i < info.points; i++) {

		float radian = (2 * glm::pi<float>() / info.points) * i;

		float cos_rad = glm::cos(radian);
		float sin_rad = glm::sin(radian);

		////outer ring vertex
		Vertex currentVertex;

		currentVertex.position = glm::vec3((info.position[0] + cos_rad) * info.radius, info.position[1], ( info.position[2] + sin_rad) * info.radius);
		currentVertex.uvs = glm::vec2(i%2,1.0f);
		currentVertex.normals = glm::vec3(0.0f, 1.0f, 0.0f);

		ring_positions.push_back(currentVertex);


		//inner ring vertex
		Vertex currentSecondVertex;

		currentSecondVertex.position = glm::vec3((info.position[0] + cos_rad) * (info.radius - info.ringThickness), info.position[1], (info.position[2] + sin_rad) * (info.radius - info.ringThickness));
		currentSecondVertex.uvs = glm::vec2(i%2,0.0f);
		currentSecondVertex.normals = glm::vec3(0.0f, 1.0f, 0.0f);

		ring_positions.push_back(currentSecondVertex);

	}

	std::vector<int> indices = createRingIndices(info);

	Mesh* newRing = MeshLoader::GenerateMesh(ring_positions, indices);
	newRing->ownType = PrimitiveType::Ring;
	newRing->ownInformation = info;
	return newRing;

}

std::vector<int> PrimitiveGenerator::createRingIndices(PrimitiveInformation info) {
	std::vector<int> ringIndices;

	for (int i = 0; i < (info.points * 2); i += 2) {
		//first triangle segment
		ringIndices.push_back(i);
		ringIndices.push_back(i+1);
		ringIndices.push_back((i+3)%(info.points*2));

		//second triangle segment
		ringIndices.push_back(i);
		ringIndices.push_back((i+3)%(info.points * 2));
		ringIndices.push_back((i+2)%(info.points * 2));

	}

	return ringIndices;
}

Mesh* PrimitiveGenerator::createDonutNGon(PrimitiveInformation info) {
	glm::vec3 donutPositions;
	glm::vec2 donutUVs;
	glm::vec3 donutNormals;

	std::vector<Vertex> donutVertices;


	glm::vec3 baseCircleMid = glm::vec3(info.position[0], info.position[1], info.position[2]);

	for (int i = 0; i < info.points; i++) {

		float radian = (2 * glm::pi<float>() / info.points) * i;

		float cos_rad_circle = glm::cos(radian);
		float sin_rad_circle = glm::sin(radian);

		glm::vec3 donutCircleMid = glm::vec3((info.position[0] + cos_rad_circle * info.radius), info.position[1], (info.position[2] + sin_rad_circle * info.radius));
		glm::vec3 baseToDonutCircleN = glm::normalize(donutCircleMid - baseCircleMid);

		for (int k = 0; k < info.donutPoints; k++) {

			float radian = (2 * glm::pi<float>() / info.donutPoints) * k;

			float cos_rad_donut = glm::cos(radian);
			float sin_rad_donut = glm::sin(radian);

			float donutHeight = 1.0f;
			float donutThickness = 1.2f;

			glm::vec3 donutCircleHeight = glm::vec3(0,sin_rad_donut * info.donutRadius * donutHeight,0);
			glm::vec3 donutCircleOffset = baseToDonutCircleN * cos_rad_donut * info.donutRadius * donutThickness;

			donutPositions = glm::vec3((donutCircleMid.x + donutCircleOffset.x + donutCircleHeight.x),
				(donutCircleMid.y + donutCircleOffset.y + donutCircleHeight.y),
				(donutCircleMid.z + donutCircleOffset.z + donutCircleHeight.z));

			//Texture Coords
			donutUVs = glm::vec2((i%2),
				(1.0f));

			//Normals
			glm::vec3 DonutToBaseCircleDirection = glm::normalize(baseCircleMid - donutCircleMid);

			//create Vector to normalize
			glm::vec3 normal = glm::vec3(baseToDonutCircleN.x + donutCircleOffset.x + donutCircleHeight.x,
				baseToDonutCircleN.y + donutCircleOffset.y + donutCircleHeight.y, 
				baseToDonutCircleN.z + donutCircleOffset.z + donutCircleHeight.z);
			normal = glm::normalize(normal);

			donutNormals = normal;

			Vertex currentDonutVertex;

			currentDonutVertex.position = donutPositions;
			currentDonutVertex.uvs = donutUVs;
			currentDonutVertex.normals = donutNormals;

			donutVertices.push_back(currentDonutVertex);

		}


	}

	info.points = info.points - 1;
	std::vector<int> indices = createDonutIndices(info);

	Mesh* newDonut = MeshLoader::GenerateMesh(donutVertices, indices);
	newDonut->ownType = PrimitiveType::Donut;
	newDonut->ownInformation = info;
	return newDonut;
}

std::vector<int> PrimitiveGenerator::createDonutIndices(PrimitiveInformation info) {
	std::vector<int> indices;

	int segments				= info.points;
	int points_per_donutring	= info.donutPoints;

	int triangleCounter = segments + 1;
	for (int i = 0; i < segments + 1; i++) {

		std::cout <<"Segement: " << i <<std::endl;
		for (int k = 0; k <= points_per_donutring - 1; k++) {

			//first triangle
			int triangle_1_1 = (i * points_per_donutring) + k;
			int triangle_1_2 = (((i+1) * points_per_donutring) + k)%(triangleCounter*points_per_donutring);
			int triangle_1_3 = (((i+1) * points_per_donutring) + (k +1)%points_per_donutring)%(triangleCounter*points_per_donutring);


			indices.push_back(triangle_1_1);
			indices.push_back(triangle_1_2);
			indices.push_back(triangle_1_3);

			//print triangle
			std::cout<< "triangle 1: "<< triangle_1_1 <<", " <<triangle_1_2 << ", " << triangle_1_3 <<std::endl;


			//second triangle
			int triangle_2_1 = (i * points_per_donutring) + k;
			int triangle_2_2 = (((i+1) * points_per_donutring) + (k +1)%points_per_donutring)%(triangleCounter*points_per_donutring);
			int triangle_2_3 = ((i * points_per_donutring) + (k +1)%points_per_donutring)%(triangleCounter*points_per_donutring);


			indices.push_back(triangle_2_1);
			indices.push_back(triangle_2_2);
			indices.push_back(triangle_2_3);

			std::cout<< "triangle 2: "<< triangle_2_1 <<", " <<triangle_2_2 << ", " << triangle_2_3 <<std::endl;
		}

		std::cout <<triangleCounter <<std::endl;
	}

	return indices;
}

Mesh* PrimitiveGenerator::createCube(PrimitiveInformation info) {

	std::vector<Vertex> changedCubeVertices;

	float radius = info.radius;
	glm::vec3 midpoint = info.position;

	for (int i = 0; i < sizeof(PrimitiveGenerator::cubeVertices)/sizeof(PrimitiveGenerator::cubeVertices[0]); i++) {
		Vertex currentVertex = cubeVertices[i];
		currentVertex.position = glm::vec3((currentVertex.position.x * radius) + midpoint[0],(currentVertex.position.y * radius) + midpoint[1], (currentVertex.position.z * radius) + midpoint[2] );
		changedCubeVertices.push_back(currentVertex);
	}


	Mesh* newCube = MeshLoader::GenerateMesh(changedCubeVertices, cube_indices);
	newCube->ownType = PrimitiveType::Cube;
	newCube->ownInformation = info;
	return newCube;
	
	/*Vertex* vertexArray = new Vertex[changedCubeVertices.size()];
	for (int vertexInVector = 0; vertexInVector < changedCubeVertices.size(); vertexInVector++) {
	vertexArray[vertexInVector] = changedCubeVertices[vertexInVector];
	}*/

	//for (int cubeVertex = 1; cubeVertex <= 8; cubeVertex +=2) {
	//	float radian = (2 * glm::pi<float>() / 8) * cubeVertex; //we need to divide 2Pi by 8 to get 45 degrees big cake slices, then we add +2 to cubeVertex so we add 90 degrees instead of just 45

	//	float cos_rad_circle = glm::cos(radian);
	//	float sin_rad_circle = glm::sin(radian);

	//	std::cout << radian << "is radian" << std::endl;


	//	Vertex newVertex;
	//	newVertex.position.x = (cos_rad_circle * radius) + midpoint[0];
	//	newVertex.position.y = (cos_rad_circle * radius) + midpoint[1];
	//	newVertex.position.z = (cos_rad_circle * radius) + midpoint[2];

	//	newVertex.uvs.x = cubeVertex%2;
	//	newVertex.uvs.y = (cubeVertex + 1)%2;

	//	newVertex.normals.x = 0.0f;
	//	newVertex.normals.y = 0.0f;
	//	newVertex.normals.z = 0.0f;

	//	Vertex secondVertex;
	//	secondVertex.position.x = (cos_rad_circle * radius) + midpoint[0];
	//	secondVertex.position.y = (cos_rad_circle * radius) + midpoint[1];
	//	secondVertex.position.z = ((cos_rad_circle * radius) + midpoint[2]) * (-1.0f);

	//	secondVertex.uvs.x = (cubeVertex + 1)%2;
	//	secondVertex.uvs.y = 1.0f;

	//	secondVertex.normals.x = 0.0f;
	//	secondVertex.normals.y = 0.0f;
	//	secondVertex.normals.z = 0.0f;

	//	cubeVertices.push_back(newVertex);
	//	cubeVertices.push_back(secondVertex);

	//}
	
}


 std::vector<int> PrimitiveGenerator::cube_indices
{	0,	1,	2,
	0,	2,	3,
	4,	6,	7,
	4,	7,	5,
	8,	10,	11,
	8,	11,	9,
	12,	14,	15,
	12,	15,	13,
	16,	18,	19,
	16,	19,	17,
	20,	21,	22,
	20,	22,	23
};
