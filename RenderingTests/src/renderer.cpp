#include "renderer.h"
#include <glad/glad.h>
#include <iostream>


glm::vec3 cubePositions[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f, 3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f, 2.0f, -2.5f),
	glm::vec3(1.5f, 0.2f, -1.5f),
	glm::vec3(-1.3f, 1.0f, -1.5f)
};

Renderer::Renderer()
	: triangleShader_(nullptr)
	, triangleVAO_(0)
	, triangleVBO_(0)
	, triangleEBO_(0)
	, rotation_(0.0f)
	, rotationSpeed_(1.5f) // radians per second
	, transformStatic_(1.0f)
	, transformDynamic_(1.0f)
	, texture1_(nullptr)
	, texture2_(nullptr)
	, context_(nullptr)
	, quadVAO_(0)
	, projectionMatrix_(1.0f)
	, window_(nullptr)
{
}

Renderer::~Renderer()
{
	shutdown();
}

void Renderer::shutdown()
{
	// Delete GL resources
	if (triangleEBO_)
	{
		glDeleteBuffers(1, &triangleEBO_);
		triangleEBO_ = 0;
	}
	if (triangleVBO_)
	{
		glDeleteBuffers(1, &triangleVBO_);
		triangleVBO_ = 0;
	}
	if (triangleVAO_)
	{
		glDeleteVertexArrays(1, &triangleVAO_);
		triangleVAO_ = 0;
	}

	// Delete higher level resources
	if (texture1_) { delete texture1_; texture1_ = nullptr; }
	if (texture2_) { delete texture2_; texture2_ = nullptr; }
	if (triangleShader_) { delete triangleShader_; triangleShader_ = nullptr; }

	// Delete GL context
	if (context_)
	{
		SDL_GL_DeleteContext(context_);
		context_ = nullptr;
	}
}

void Renderer::onWindowResize(int newWidth, int newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	//camera?
}

bool Renderer::initialize(SDL_Window* window)
{

	window_ = window;
	//SDL context
	context_ = SDL_GL_CreateContext(window_);

	if (context_ == nullptr)
	{
		std::cout << "Failed to create OpenGL context! SDL Error: " << SDL_GetError() << std::endl;
		return false;
	}
	//init glad
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	int width = 0, height = 0;
	SDL_GetWindowSize(window_, &width, &height);
	glViewport(0, 0, width, height);

	//creating shader
	triangleShader_ = new Shader("./shaders/shader.vs", "./shaders/shader.fs");

	//---------RENDERING ROUTINE FOR TRIANGLE TYPE---------
	//BOX
	float vertices[] = {
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};


	
	//texture setup;
	texture1_ = new Texture("./texture/textureSample.jpg");
	texture2_ = new Texture("./texture/retroTexture.jpg");
	texture1_->bind(0);
	texture2_->bind(1);
	
	//Vertex Array Object
	glGenVertexArrays(1, &triangleVAO_);
	glBindVertexArray(triangleVAO_);

	//Vertex Object Buffer setup
	glGenBuffers(1, &triangleVBO_);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	//Telling how to read position from buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//telling how to read texture from buffer
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
		(void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Unbinding
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//Inizialize cached transforms
	transformStatic_ = glm::mat4(1.0f);
	transformDynamic_ = glm::mat4(1.0f);

	return true;
}


void Renderer::renderScene(const Camera& camera)
{
	//use shader program
	triangleShader_->use();

	//mapping texture channels
	triangleShader_->setInt("texture1", 0);
	triangleShader_->setInt("texture2", 1);

	// Ensure textures are bound to the correct units
	if (texture1_) texture1_->bind(0);
	if (texture2_) texture2_->bind(1);

	glBindVertexArray(triangleVAO_);

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	//projection matrix property of renderer
	triangleShader_->setMat4("projection", projection);
	//view matrix property of camera
	triangleShader_->setMat4("view", camera.cameraView_);

	//box with uniform applied
	glEnable(GL_DEPTH_TEST);
	//object to world for each cube
	for (int i = 0; i < 10; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		triangleShader_->setMat4("model", model);

		// Draw the cube
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	glBindVertexArray(0);
}

void Renderer::clearScreen()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::present()
{
	SDL_GL_SwapWindow(window_);
}
