#include "Background.h"



Background::Background()
{

}


Background::~Background()
{
}

GLuint Background::createShaderProgram() {
	GLuint shaderProgram, vertexShader, fragmentShader;

	const GLchar* vertexShaderSource[] = {
		"#version 440 core															\n"
		"layout (location = 0) in vec3 position;									\n"
		"out vec3 TexCoords;															\n"
		"uniform mat4 projection;													\n"
		"uniform mat4 view;															\n"
		"uniform mat4 model;														\n"
		"void main()																\n"
		"{																			\n"
		"TexCoords = position;														\n"
		"vec4 pos = projection * view * vec4(position, 1.0);						\n"
		"gl_Position = pos.xyww;}												\n"
	};
	const GLchar* fragmentShaderSource[] = {
		"#version 440 core															\n"
		"out vec4 FragColor;														\n"
		"in vec3 TexCoords;															\n"
		"uniform samplerCube skybox;												\n"
		"void main()																\n"
		"{																			\n"
		"FragColor = texture(skybox, TexCoords);									\n"
		"}																			\n\0"
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
		std::cout << "PLAYERSHIP::VERTEX::SHADER CREATION ERROR: " << infoLog << std::endl;
		return 1;
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "PLAYERSHIP::FRAGMENT::SHADER CREATION ERROR: " << infoLog << std::endl;
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
		std::cout << "SHADER::PROGRAM CREATION ERROR: " << infoLog << std::endl;
	}
	if (success) {
		program = shaderProgram;
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}




	return shaderProgram;
}


GLuint Background::createCube() {
	GLfloat verts[] = {
		-100.0f,  100.0f, -100.0f,
		-100.0f, -100.0f, -100.0f,
		100.0f, -100.0f, -100.0f,
		100.0f, -100.0f, -100.0f,
		100.0f,  100.0f, -100.0f,
		-100.0f,  100.0f, -100.0f,

		-100.0f, -100.0f,  100.0f,
		-100.0f, -100.0f, -100.0f,
		-100.0f,  100.0f, -100.0f,
		-100.0f,  100.0f, -100.0f,
		-100.0f,  100.0f,  100.0f,
		-100.0f, -100.0f,  100.0f,

		100.0f, -100.0f, -100.0f,
		100.0f, -100.0f,  100.0f,
		100.0f,  100.0f,  100.0f,
		100.0f,  100.0f,  100.0f,
		100.0f,  100.0f, -100.0f,
		100.0f, -100.0f, -100.0f,

		-100.0f, -100.0f,  100.0f,
		-100.0f,  100.0f,  100.0f,
		100.0f,  100.0f,  100.0f,
		100.0f,  100.0f,  100.0f,
		100.0f, -100.0f,  100.0f,
		-100.0f, -100.0f,  100.0f,

		-100.0f,  100.0f, -100.0f,
		100.0f,  100.0f, -100.0f,
		100.0f,  100.0f,  100.0f,
		100.0f,  100.0f,  100.0f,
		-100.0f,  100.0f,  100.0f,
		-100.0f,  100.0f, -100.0f,

		-100.0f, -100.0f, -100.0f,
		-100.0f, -100.0f,  100.0f,
		100.0f, -100.0f, -100.0f,
		100.0f, -100.0f, -100.0f,
		-100.0f, -100.0f,  100.0f,
		100.0f, -100.0f,  100.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), &verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	return 1;
}

GLuint Background::makeCubeMap() {
	GLuint textureID;
	glGenTextures(1, &textureID);

	std::vector<const char*> faces;

	faces.push_back("bin/assets/skybox/right.png");
	faces.push_back("bin/assets/skybox/left.png");
	faces.push_back("bin/assets/skybox/top.png");
	faces.push_back("bin/assets/skybox/down.png");
	faces.push_back("bin/assets/skybox/front.png");
	faces.push_back("bin/assets/skybox/back.png");

	SDL_Surface* img;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (int i = 0; i < faces.size(); i++) {
		img = IMG_Load(faces[i]);
		if (img == NULL) {
			std::cout << "Failed on skybox image load: \n\t" << IMG_GetError() << std::endl;
			return 0;
		}
		else {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);
			SDL_FreeSurface(img);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);


	cubemapTex = textureID;
	return textureID;
}

void Background::Draw() {
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(program, "skybox"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTex);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}



