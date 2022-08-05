#pragma once

#ifndef IOSTREAM
#include <iostream>
#define IOSTREAM
#endif // !IOSTREAM


#ifndef FSTREAM
#include <fstream>
#define FSTREAM
#endif // !FSTREAM


//TODO INCLUDE GUARDS
#include <sstream>
#include <map>

#ifndef GLAD
#define GLAD
#include <glad/glad.h>
#endif

#ifndef VECTOR
#include <vector>
#define VECTOR
#endif // !VECTOR

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class Shader {
public:
	GLint shaderIndex;
	std::string shaderName;

	Shader(){}
	Shader(int id,  std::string name) {
		shaderIndex = id;
		shaderName = name;
	}

	
};

