#pragma once
#include "Vertex.h"
#include <vector>
#include "PrimitiveTypes.h"
#include "PrimitiveInformation.h"


struct BoundingBox {
	float xMin = 0.0f, xMax = 0.0f;
	float yMin = 0.0f, yMax = 0.0f;
	float zMin = 0.0f, zMax = 0.0f;

};
class Mesh {
	public:
		PrimitiveType ownType; 
		PrimitiveInformation ownInformation;
		std::vector<Vertex> vertices;
		std::vector<int> indices;
		unsigned int VAO = 0;
		unsigned int VBO = 0;
		unsigned int EBO = 0;

		bool boundingBoxDrawing = false;
		BoundingBox boundingBox;
		std::vector<Vertex> boundingBoxVertices;
		
		unsigned int boundingBoxVAO;
		unsigned int boundingBoxVBO;
		unsigned int boundingBoxEBO;

		void Serialize(JsonSerializationArchive& archive);

		Mesh(){};
		Mesh(std::vector<Vertex> newVertices, unsigned int vao, unsigned int vbo, unsigned int ebo) {
			vertices = newVertices;
			VAO = vao;
			VBO = vbo;
			EBO = ebo;
		}

		Mesh(std::vector<Vertex> newVertices, unsigned int vao, unsigned int vbo, unsigned int ebo, PrimitiveType type) {
			vertices = newVertices;
			VAO = vao;
			VBO = vbo;
			EBO = ebo;
			ownType = type;
		}

		Mesh(std::vector<Vertex> newVertices, std::vector<int> newIndices, unsigned int vao, unsigned int vbo, unsigned int ebo) {
			vertices = newVertices;
			indices = newIndices;
			VAO = vao;
			VBO = vbo;
			EBO = ebo;
		}

		Mesh(std::vector<Vertex> newVertices, std::vector<int> newIndices, unsigned int vao, unsigned int vbo, unsigned int ebo, PrimitiveType type) {
			vertices = newVertices;
			indices = newIndices;
			VAO = vao;
			VBO = vbo;
			EBO = ebo;
			ownType = type;
		}
		

		std::vector<int> cube_indices
		{		0,1,
				1,2,
				2,3,
				3,0,
				0,4,
				1,5,
				2,6,
				3,7,
				4,5,
				5,6,
				6,7,
				7,4
				
		};

		Vertex cubeVertices[24] = { 
			//positions							//uvs						//normals
			glm::vec3(1.0f, 1.0f, 1.0f),		glm::vec2(1.0f, 0.0f),		glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, -1.0f),		glm::vec2(1.0f, 1.0f),		glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(-1.0f, 1.0f, -1.0f),		glm::vec2(0.0f, 1.0f),		glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(-1.0f, 1.0f, 1.0f),		glm::vec2(0.0f, 0.0f),		glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(1.0f, -1.0f, 1.0f),		glm::vec2(0.0f, 0.0f),		glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, -1.0f, -1.0f),		glm::vec2(1.0f, 0.0f),		glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, -1.0f, -1.0f),		glm::vec2(1.0f, 0.0f),		glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(-1.0f, -1.0f, 1.0f),		glm::vec2(1.0f, 0.0f),		glm::vec3(0.0f, 0.0f, -1.0f),

		};

};


