#include "renderer.h"
#include <glad/glad.h>
#include <iostream>

int swap = 0;

Renderer::Renderer()
{

}

Renderer::~Renderer()
{
	shutdown();
}

void Renderer::shutdown()
{
	triangleShader_->shutdown();
	delete texture1_;
	delete texture2_;
	delete triangleShader_;
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
	triangleShader_ = new Shader("./shader.vs", "./shader.fs");

	//---------RENDERING ROUTINE FOR TRIANGLE TYPE---------
	//BOX
	float vertices[] = {
		// positions // colors // texture coords
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
	};

	
	//texture setup;
	stbi_set_flip_vertically_on_load(true);
	texture1_ = new Texture("./textureSample.jpg");
	texture2_ = new Texture("./retroTexture.jpg");
	texture1_->bind(0);
	texture2_->bind(1);


	unsigned int indices[] = { 
	0, 1, 2, // first triangle
	0, 2, 3 // second triangle
	};
	
	//Vertex Array Object
	glGenVertexArrays(1, &triangleVAO_);
	glBindVertexArray(triangleVAO_);

	//Vertex Object Buffer setup
	glGenBuffers(1, &triangleVBO_);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Telling how to read position from buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	//Telling how to read color from buffer
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//telling how to read texture from buffer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		(void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	//EBO setup
	glGenBuffers(1, &triangleEBO_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Unbinding
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}

void Renderer::renderTriangle()
{
	//use shader program
	triangleShader_->use();


	//mapping texture channels
	triangleShader_->setInt("texture1", 0);
	triangleShader_->setInt("texture2", 1);

	glBindVertexArray(triangleVAO_);

	glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Renderer::clearScreen()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::present()
{
	SDL_GL_SwapWindow(window_);
}
