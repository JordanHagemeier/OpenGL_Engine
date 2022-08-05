#pragma once
#include "Mesh.h"
#include <glad/glad.h>

class MeshLoader {


	public:
		

		MeshLoader(){};

		static Mesh* GenerateMesh(std::vector<Vertex> Vertices, std::vector<int> Indices);
		static void generateVertexObjects(unsigned int& vao, unsigned int& vbo, const float* vert, int length, unsigned int& ebo, int* indices, unsigned int indice_length);
		static void generateVertexObjects(unsigned int& vao, unsigned int& vbo, std::vector<float> vert, int length, unsigned int& ebo, std::vector<int> ind, int indice_length);
		static void generateVertexObjects(unsigned int& vao, unsigned int& vbo, std::vector<Vertex> vert, int length, unsigned int& ebo, std::vector<int> ind, int indice_length);
		static void setUpVertexAttributes();
		static void createBoundingBox(Mesh* mesh);
};

