#include "model.h"



model::model(GLchar* path)
{
	loadModel(path);
}

model::model() {

}

model::~model()
{
}

void model::loadModel(std::string path) {
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if(!scene || scene->mFlags && AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void model::Draw(GLuint shader) {
	//std::cout << "Num Meshes: " << meshes.size() << std::endl;
	for (GLuint i = 0; i < meshes.size(); i++) {
		meshes[i].Draw(shader);
	}
}

void model::processNode(aiNode* node, const aiScene* scene) {
	
	for(GLuint i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	for(GLuint i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<GLuint> indicies;
	std::vector<Texture> textures;
	
	for(GLuint i = 0; i < mesh->mNumVertices; i++) {	
		Vertex vertex;
		glm::vec3 vector;

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		if (mesh->mTextureCoords[0]) {
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else {
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	// first loop done, now wak through each of the mesh's.....
	
	for (GLuint i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (GLuint j = 0; j < face.mNumIndices; j++) {
			indicies.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	}

	return Mesh(vertices, indicies, textures);
}

std::vector<Texture> model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
	std::vector<Texture> textures;

	for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
		
		aiString str;
		mat->GetTexture(type, i, &str);

		GLboolean skip = false;
		for (GLuint j = 0; j < textures_loaded.size(); j++) {
			if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0) {
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}

GLint model::TextureFromFile(const char* path, std::string directory) {
	//std::cout << "Loading texture from file..." << std::endl;
	std::string filename = std::string(path);
	filename = directory + '/' + filename;
	GLuint textureID;
	glGenTextures(1, &textureID);
	SDL_Surface* img = IMG_Load(filename.c_str());
	if (img == NULL) {
		std::cout << "Model Image load fail!" << std::endl;
		std::cout << "IMG_LOAD: " << IMG_GetError() << std::endl;
		return (GLint)1;
	}

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->w, img->h, 0, GL_RGB, GL_UNSIGNED_BYTE, img->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	SDL_FreeSurface(img);
	return textureID;
}

GLuint model::createShaderProgram() {
	GLuint shaderProgram, vertexShader, fragmentShader;

	const GLchar* vertexShaderSource[] = {
		"#version 440 core															\n"
		"layout (location = 0) in vec3 position;									\n"
		"layout (location = 1) in vec3 normal;										\n"
		"layout (location = 2) in vec2 texCoords;									\n"
		"out vec2 TexCoords;														\n"
		"out vec3 Normal;															\n"
		"out vec3 FragPos;															\n"
		"uniform mat4 model;														\n"
		"uniform mat4 view;															\n"
		"uniform mat4 projection;													\n"
		"void main() {																\n"
		"FragPos = vec3(model * vec4(position, 1.0f));								\n"
		"Normal = mat3(transpose(inverse(model))) * normal;							\n"
		"gl_Position = projection * view * vec4(FragPos, 1.0f);						\n"
		"TexCoords = texCoords;														\n"
		"}\n"
	};
	const GLchar* fragmentShaderSource[] = {
		"#version 440 core																			\n"
		"out vec4 fragColor;																		\n"
		"uniform sampler2D texture_diffuse1;														\n"
		"struct Material {																			\n"
		"	vec3 specular;																			\n"
		"	float shininess;};																		\n"
		"struct Light {																				\n"
		"	vec3 position;																			\n"
		"	vec3 ambient;																			\n"
		"	vec3 diffuse;																			\n"
		"	float constant;																			\n"
		"	float linear;																			\n"
		"	float quadratic;																		\n"
		"	vec3 specular;};																		\n"
		"in vec3 FragPos;																			\n"
		"in vec3 Normal;																			\n"
		"in vec2 TexCoords;																			\n"
		"uniform vec3 viewPos;																		\n"
		"uniform Light light;																		\n"
		"uniform Material material;																	\n"
		"void main() {																				\n"
		"	vec3 ambient = light.ambient * texture(texture_diffuse1, TexCoords).rgb;				\n"
		"	vec3 norm = normalize(Normal);															\n"
		"	vec3 lightDir = normalize(light.position - FragPos);									\n"
		"	float diff = max(dot(norm, lightDir), 0.0);												\n"
		"	vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;			\n"
		"	vec3 viewDir = normalize(viewPos - FragPos);											\n"
		"	vec3 reflectDir = reflect(-lightDir, norm);												\n"
		"	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);				\n"
		"	vec3 specular = light.specular * (spec * material.specular);							\n"
		"	float distance = length(light.position - FragPos);										\n"
		"	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); \n"
		"	ambient *= attenuation;																	\n"
		"	diffuse *= attenuation;																	\n"
		"	specular *= attenuation;																\n"
		"	fragColor = vec4(ambient + diffuse + specular, 1.0f);}									\n\0"
	};
	vertexShader = glCreateShader(GL_VERTEX_SHADER);		//create vertex shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);	//create fragment shader

	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);	//assign GLChar source to shader
	glCompileShader(vertexShader);								//compile shader

	glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Checking to see if shaders created successfully
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "MODEL::VERTEX::SHADER CREATION ERROR: " << infoLog << std::endl;
		return 1;
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "MODEL::FRAGMENT::SHADER CREATION ERROR: " << infoLog << std::endl;
		return 1;
	}

	/* SHADERPROGRAM CREATION */


	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "MODEL::SHADER::PROGRAM CREATION ERROR: " << infoLog << std::endl;
	}
	if (success) {
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	return shaderProgram;	
}
