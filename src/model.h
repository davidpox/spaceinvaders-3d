#pragma once

#include <iostream>
#include <string>
#include <GL/glew.h>
#include "SDL.h"
#include "SDL_image.h"
#include <vector>
#include "Mesh.h"

// // NOTE: GLSL uses radians, so will do the same, for consistency
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//ASSIMP
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class model
{
public:
	model();
	model(GLchar* path);
	~model();

	bool isDead;
	int currentAnimState;
	
	glm::vec3 position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 size{ 0.0f, 0.0f, 0.0f };
	std::string objName;

	void Draw(GLuint shader);
	glm::mat4 _transRotate, _transTranslate, _transScale;
	GLuint createShaderProgram();
	
private:
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	GLint TextureFromFile(const char* path, std::string directory);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

