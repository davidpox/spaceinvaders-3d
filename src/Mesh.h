#pragma once

#include <iostream>
#include <string>
#include <GL/glew.h>
#include "SDL.h"
#include "SDL_image.h"
#include <sstream>
#include <vector>

// // NOTE: GLSL uses radians, so will do the same, for consistency
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture
{
	GLuint id;
	std::string type;
	aiString path;
};

class Mesh
{
public:
	std::vector<Vertex> verticies;
	std::vector<GLuint> indicies;
	std::vector<Texture> textures;
	
	void Draw(GLuint shader);

	Mesh(std::vector<Vertex> verticies, std::vector<GLuint> indicies, std::vector<Texture> textures);
	~Mesh();
	GLuint VAO, VBO, EBO;
	void setupMesh();
};

