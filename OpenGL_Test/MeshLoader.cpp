#include "MeshLoader.h"

Mesh* MeshLoader::GenerateMesh(std::vector<Vertex> Vertices, std::vector<int> Indices) {
	unsigned int current_VAO = 0;
	unsigned int current_VBO = 0;
	unsigned int current_EBO = 0;

	generateVertexObjects(current_VAO, current_VBO, Vertices, Vertices.size(), current_EBO, Indices,  Indices.size());

	Mesh* currentMesh = new Mesh(Vertices, Indices, current_VAO, current_VBO, current_EBO);


	return currentMesh;
}

void MeshLoader::generateVertexObjects(unsigned int& vao, unsigned int& vbo, const float* vert, int length, unsigned int& ebo, int* indices, unsigned int indice_length) {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); 
	glBufferData(GL_ARRAY_BUFFER, length, vert, GL_STATIC_DRAW); //should use std::span<float> instead of float* and length 
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indice_length, indices, GL_STATIC_DRAW);
	setUpVertexAttributes();
	glBindVertexArray(0);

}


void MeshLoader::generateVertexObjects(unsigned int& vao, unsigned int& vbo, std::vector<float> vert, int length, unsigned int& ebo, std::vector<int> ind, int indice_length) {

	generateVertexObjects(vao, vbo, vert.data(), static_cast<int>(length * sizeof(float)), ebo,ind.data(), static_cast<unsigned int>(indice_length *sizeof(int)));


}

void MeshLoader::generateVertexObjects(unsigned int& vao, unsigned int& vbo, std::vector<Vertex> vert, int length, unsigned int& ebo, std::vector<int> ind, int indice_length) {

	generateVertexObjects(vao, vbo, vert[0].data(), static_cast<int>(length * sizeof(Vertex)), ebo, ind.data(), static_cast<unsigned int>(indice_length *sizeof(int)));


}

void MeshLoader::setUpVertexAttributes() {
	//position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	////texture coordinates attributes
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	//normals
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5* sizeof(float)));


	//this is build up like this:
	//1. which vertex attribute to configure (in this case location 0, so 0)
	//2. size of the vertex attribute (vec3 in this case, so 3)
	//3. what kind of data type (float)
	//4. whether to normalize the input or not (no)
	//5. space between the consecutive vertex attributes (in this case 3 * the size of our float values)
	//6. offset for the beginning of our values (no offset, so this weird void pointer)

	//IMPORTANT: 
	//what data is modified with this attribute is determined by the Vertex Buffer Object (VBO) that is CURRENTLY bound!
	//if I want to modify several of these VBOs with attributes, I gotta design a system to do that!
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	//gotta enable the location of the vertex attribute
}


void MeshLoader::createBoundingBox(Mesh* mesh) {
	BoundingBox* currentBoundingBox = new BoundingBox;
	for (Vertex vert : mesh->vertices) {
		if(vert.position.x < currentBoundingBox->xMin){ currentBoundingBox->xMin = vert.position.x;}
		if(vert.position.x > currentBoundingBox->xMax){ currentBoundingBox->xMax = vert.position.x;}
		if(vert.position.y < currentBoundingBox->yMin){ currentBoundingBox->yMin = vert.position.y;}
		if(vert.position.y > currentBoundingBox->yMax){ currentBoundingBox->yMax = vert.position.y;}
		if(vert.position.z < currentBoundingBox->zMin){ currentBoundingBox->zMin = vert.position.z;}
		if(vert.position.z > currentBoundingBox->zMax){ currentBoundingBox->zMax = vert.position.z;}
	}

	mesh->boundingBox = *currentBoundingBox;


	//hardcode to test this theory
	//TODO

	//below
	Vertex BB1;
	BB1.position.x = currentBoundingBox->xMin;
	BB1.position.y = currentBoundingBox->yMin;
	BB1.position.z = currentBoundingBox->zMin;

	Vertex BB2;
	BB2.position.x = currentBoundingBox->xMax;
	BB2.position.y = currentBoundingBox->yMin;
	BB2.position.z = currentBoundingBox->zMin;

	Vertex BB3;
	BB3.position.x = currentBoundingBox->xMax;
	BB3.position.y = currentBoundingBox->yMin;
	BB3.position.z = currentBoundingBox->zMax;

	Vertex BB4;
	BB4.position.x = currentBoundingBox->xMin;
	BB4.position.y = currentBoundingBox->yMin;
	BB4.position.z = currentBoundingBox->zMax;

	//top
	Vertex BB5;
	BB5.position.x = currentBoundingBox->xMin;
	BB5.position.y = currentBoundingBox->yMax;
	BB5.position.z = currentBoundingBox->zMin;

	Vertex BB6;
	BB6.position.x = currentBoundingBox->xMax;
	BB6.position.y = currentBoundingBox->yMax;
	BB6.position.z = currentBoundingBox->zMin;

	Vertex BB7;
	BB7.position.x = currentBoundingBox->xMax;
	BB7.position.y = currentBoundingBox->yMax;
	BB7.position.z = currentBoundingBox->zMax;

	Vertex BB8;
	BB8.position.x = currentBoundingBox->xMin;
	BB8.position.y = currentBoundingBox->yMax;
	BB8.position.z = currentBoundingBox->zMax;


	mesh->boundingBoxVertices.push_back(BB1);
	mesh->boundingBoxVertices.push_back(BB2);
	mesh->boundingBoxVertices.push_back(BB3);
	mesh->boundingBoxVertices.push_back(BB4);
	mesh->boundingBoxVertices.push_back(BB5);
	mesh->boundingBoxVertices.push_back(BB6);
	mesh->boundingBoxVertices.push_back(BB7);
	mesh->boundingBoxVertices.push_back(BB8);

	generateVertexObjects(mesh->boundingBoxVAO, mesh->boundingBoxVBO, mesh->boundingBoxVertices, mesh->boundingBoxVertices.size(), mesh->boundingBoxEBO, mesh->cube_indices, mesh->cube_indices.size());
	
	mesh->boundingBoxDrawing = true;
}