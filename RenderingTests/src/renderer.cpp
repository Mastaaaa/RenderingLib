#include "renderer.h"
#include <glad/glad.h>
#include <iostream>

//I may leak some memory with current shader using, maybe stop using pointers

Renderer::Renderer()
	: triangleShader_(nullptr)
	, m_lightShader(nullptr)
	, m_cubeVAO(0)
	, m_cubeVBO(0)
	, rotation_(0.0f)
	, rotationSpeed_(1.5f) // radians per second
	, transformStatic_(1.0f)
	, transformDynamic_(1.0f)
	, texture1_(nullptr)
	, texture2_(nullptr)
	, context_(nullptr)
	, quadVAO_(0)
	, m_projectionMatrix(1.0f)
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
	if (m_cubeVBO)
	{
		glDeleteBuffers(1, &m_cubeVBO);
		m_cubeVBO = 0;
	}
	if (m_cubeVAO)
	{
		glDeleteVertexArrays(1, &m_cubeVAO);
		m_cubeVAO = 0;
	}
	if (m_planeVBO)
	{
		glDeleteBuffers(1, &m_planeVBO);
		m_planeVBO = 0;
	}
	if (m_planeVAO)
	{
		glDeleteVertexArrays(1, &m_planeVAO);
		m_planeVAO = 0;
	}

	// Delete higher level resources
	if (texture1_) { delete texture1_; texture1_ = nullptr; }
	if (texture2_) { delete texture2_; texture2_ = nullptr; }
	if (triangleShader_) { delete triangleShader_; triangleShader_ = nullptr; }
	if (m_lightShader) { delete m_lightShader; m_lightShader = nullptr; }

	// Delete GL context
	if (context_)
	{
		SDL_GL_DeleteContext(context_);
		context_ = nullptr;
	}
}

void Renderer::onWindowResize(int newWidth, int newHeight, const glm::mat4& projectionMatrix)
{
	glViewport(0, 0, newWidth, newHeight);
	//setting new projectionMatrix
	m_projectionMatrix = projectionMatrix;
	triangleShader_->setMat4("projection", projectionMatrix);
}

bool Renderer::initialize(SDL_Window* window, const glm::mat4& projectionMatrix)
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

	//Cube and terrain shaders
	triangleShader_ = new Shader("./shaders/shader.vs", "./shaders/shader.fs");
	m_projectionMatrix = projectionMatrix;
	triangleShader_->use();
	triangleShader_->setMat4("projection", projectionMatrix);
	triangleShader_->unuse();

	//Light shaders
	m_lightShader = new Shader("./shaders/light_shader.vs", "./shaders/light_shader.fs");
	m_projectionMatrix = projectionMatrix;
	m_lightShader->use();
	m_lightShader->setMat4("projection", projectionMatrix);
	m_lightShader->unuse();


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

	//LIGHT SOURCE VERTICES
	float light_vertices[] = {
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f, 
	0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f, 
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f, 
	0.5f, -0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f, 
	-0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, -0.5f, 
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f, 
	-0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, -0.5f, -0.5f, 
	0.5f, -0.5f, -0.5f, 
	0.5f, -0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f, 
	0.5f, -0.5f, 0.5f, 
	0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f, 
	-0.5f, -0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f, 
	0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, -0.5f, 
	};

	//PLANE

	float vertices_piano[] = {                
		-1000.0f, 0.0f,  1000.0f,    0.0f,   1000.0f,  
		 1000.0f, 0.0f,  1000.0f,    1000.0f, 1000.0f, 
		-1000.0f, 0.0f, -1000.0f,    0.0f,   0.0f,    

		-1000.0f, 0.0f, -1000.0f,    0.0f,   0.0f,    
		 1000.0f, 0.0f,  1000.0f,    1000.0f, 1000.0f,  
		 1000.0f, 0.0f, -1000.0f,    1000.0f, 0.0f     
	};

	
	//texture setup;
	texture1_ = new Texture("./texture/textureSample.jpg");
	texture2_ = new Texture("./texture/grassTexture.jpg");
	texture1_->bind(0);

//------------------------CUBE TYPE--------------------
	
	//Vertex Array Object
	glGenVertexArrays(1, &m_cubeVAO);
	glBindVertexArray(m_cubeVAO);

	//Vertex Object Buffer setup
	glGenBuffers(1, &m_cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_cubeVBO);
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

//------------------------PLANE TYPE------------------------
	//Vertex Array Object
	glGenVertexArrays(1, &m_planeVAO);
	glBindVertexArray(m_planeVAO);

	//Vertex Object Buffer setup
	glGenBuffers(1, &m_planeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_piano), vertices_piano, GL_STATIC_DRAW);

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


//------------------------LIGHT SOURCE------------------------
	//Vertex Array Object
	glGenVertexArrays(1, &m_lightVAO);
	glBindVertexArray(m_lightVAO);

	//Vertex Object Buffer setup
	glGenBuffers(1, &m_lightVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(light_vertices), light_vertices, GL_STATIC_DRAW);

	//Telling how to read position from buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Unbinding
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Inizialize cached transforms
	transformStatic_ = glm::mat4(1.0f);
	transformDynamic_ = glm::mat4(1.0f);

	return true;
}


void Renderer::renderScene(const glm::mat4& viewMatrix)
{
	//use cube and plane shader
	triangleShader_->use();

//---------------MATRIX SCENE SETUP FOR CUBE AND PLANE-------------
	triangleShader_->setMat4("view", viewMatrix);
	triangleShader_->setMat4("projection", m_projectionMatrix);
	glEnable(GL_DEPTH_TEST);

//---------------TEST CUBE RENDERING-------------------;
	glBindVertexArray(m_cubeVAO);

	//color setup instead of shaders for the plane
	triangleShader_->setBool("u_useTexture", false);
	//object color hardcoded
	triangleShader_->setVec3("u_objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
	//white light hardcoded
	triangleShader_->setVec3("u_lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

	//model matrix setup for the cube
	glm::mat4 modelCube = (1.0f);
	modelCube = glm::translate(modelCube, glm::vec3(0.0f, -0.5f, 0.0f));
	triangleShader_->setMat4("model", modelCube);


	glDrawArrays(GL_TRIANGLES, 0, 36);

//--------------PLANE RENDERING--------------------
	glBindVertexArray(m_planeVAO);

	//texture setup for the plane
	triangleShader_->setInt("texture1", 0);
	triangleShader_->setBool("u_useTexture", true);
	texture2_->bind(0);

	//model matrix setup for the plane
	glm::mat4 modelPlane = glm::mat4(1.0f);
	modelPlane = glm::translate(modelPlane, glm::vec3(0.0f, -1.f, 0.0f));
	triangleShader_->setMat4("model", modelPlane);

	//drawing and resetting texture
	glDrawArrays(GL_TRIANGLES, 0, 6);
	triangleShader_->setBool("u_useTexture", false);

//---------------MATRIX SCENE SETUP LIGHT SHADER-------------
	//use lighting shader
	m_lightShader->use();
	m_lightShader->setMat4("view", viewMatrix);
	m_lightShader->setMat4("projection", m_projectionMatrix);

//----------------LIGHT SOURCE RENDERING-------------
	glBindVertexArray(m_lightVAO);

	//model matrix setup for lightSource
	glm::mat4 modelLight = glm::mat4(1.0f);
	modelLight = glm::translate(modelLight, glm::vec3(5.0f, 5.0f, -5.0f));
	modelLight = glm::scale(modelLight, glm::vec3(0.2f));
	m_lightShader->setMat4("model", modelLight);

	//drawing
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
}

void Renderer::clearScreen()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::present()
{
	SDL_GL_SwapWindow(window_);
}
