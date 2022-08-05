#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Vertex.h"
#include "MeshLoader.h"
#include <map>
#include "PrimitiveTypes.h"
#include "Serialization/JsonArchive.h"

#include "PrimitiveInformation.h"


typedef Mesh*(*FunctionPointer)(PrimitiveInformation info);

class PrimitiveGenerator {

	
	static std::map<PrimitiveType, FunctionPointer> mappedFunctions;
	
	public:

		
		/*PrimitiveGenerator() {
			mappedFunctions[PrimitiveType::Circle]		= &PrimitiveGenerator::createNGon;
			mappedFunctions[PrimitiveType::Ring]		= &PrimitiveGenerator::createRingNGon;
			mappedFunctions[PrimitiveType::Donut]		= &PrimitiveGenerator::createDonutNGon;
			mappedFunctions[PrimitiveType::Cube]		= &PrimitiveGenerator::createCube;
		}*/

		static FunctionPointer functionPointer;

		static std::map<PrimitiveType, FunctionPointer> InitPrimitiveFunctions();
		static const char* primitiveNames[];
	 
		static Mesh* callFunctionByEnum(PrimitiveType type, PrimitiveInformation pInfo);
		static PrimitiveInformation generatePrimitiveInfo(glm::vec3 pos, float rad, float donutrad, int points, int donutpoints, float ringThickness);

		static Mesh* createNGon(PrimitiveInformation info);
		static std::vector<int> createCircleIndices(PrimitiveInformation info);
	
		static Mesh* createRingNGon(PrimitiveInformation pInfo);
		static std::vector<int> createRingIndices(PrimitiveInformation pInfo);
		
		static Mesh* createDonutNGon(PrimitiveInformation pInfo);
		static std::vector<int> createDonutIndices(PrimitiveInformation pInfo);
		
		static Mesh* createCube(PrimitiveInformation info);


		static std::vector<int> cube_indices;

		static Vertex cubeVertices[];

};