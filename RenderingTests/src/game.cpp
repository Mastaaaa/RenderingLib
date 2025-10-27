#include "game.h"
#include "renderer.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



Game::Game()
	: myCamera_()
{
	window_ = nullptr;
	isRunning_ = false;
}

Game::~Game()
{
	shutdown();
}

bool Game::initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "Error initializing SDL!" << std::endl;
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	window_ = SDL_CreateWindow(
		"Space Invaders",
		100,
		100,
		800,
		600,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);

	if (window_ == nullptr)
	{
		std::cout << "Error creating SDL window" << std::endl;
		return false;
	}

	if (!renderer_.initialize(window_))
	{
		std::cout << "Error initializing renderer" << std::endl;
		return false;
	}


	isRunning_ = true;
	return true;

}

void Game::run()
{
	//Calcualate delta time
	Uint64 performanceFrequency = SDL_GetPerformanceFrequency();
	Uint64 lastTime = SDL_GetPerformanceCounter();

	isRunning_ = true;
	while (isRunning_)
	{
		Uint64 currentTime = SDL_GetPerformanceCounter();
		Uint64 elapsedTicks = currentTime - lastTime;
		float deltaTime = (float)elapsedTicks / (float)performanceFrequency;

		lastTime = currentTime;

		processInput();
		update(deltaTime);
		render();

	}

}

void Game::update(float deltaTime)
{
	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	if (keyboardState[SDL_SCANCODE_W])
	{
		glm::vec3 flatDirection = myCamera_.cameraDirection_;
		flatDirection.y = 0.0f;
		flatDirection = glm::normalize(flatDirection);
		myCamera_.cameraPosition_.x += 5.0f * deltaTime * flatDirection.x;
		myCamera_.cameraPosition_.z += 5.0f * deltaTime * flatDirection.z;
	}
	//hardcoded speed, must be..idk player speed?
	if (keyboardState[SDL_SCANCODE_S])
	{
		glm::vec3 flatDirection = myCamera_.cameraDirection_;
		flatDirection.y = 0.0f;
		flatDirection = glm::normalize(flatDirection);
		myCamera_.cameraPosition_.x -= 5.0f * deltaTime * flatDirection.x;
		myCamera_.cameraPosition_.z -= 5.0f * deltaTime * flatDirection.z;
	}
	if (keyboardState[SDL_SCANCODE_D])
	{
		myCamera_.cameraPosition_ += 10.0f * deltaTime * glm::normalize(glm::cross(myCamera_.cameraDirection_, myCamera_.upVector_));
	}
	if (keyboardState[SDL_SCANCODE_A])
	{
		myCamera_.cameraPosition_ -= 10.0f * deltaTime * glm::normalize(glm::cross(myCamera_.cameraDirection_, myCamera_.upVector_));
	}
	if (keyboardState[SDL_SCANCODE_SPACE])
	{
		myCamera_.cameraPosition_ += glm::vec3(0.0f, 10.f * deltaTime, 0.0f);
	}
	if (keyboardState[SDL_SCANCODE_LCTRL])
	{
		myCamera_.cameraPosition_ -= glm::vec3(0.0f, 10.f * deltaTime, 0.0f);
		
	}

	myCamera_.updateCameraView();


}

void Game::render()
{
	renderer_.clearScreen();

	renderer_.renderScene(myCamera_);

	renderer_.present();
}

void Game::processInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				isRunning_ = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					isRunning_ = false;
				}
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					renderer_.onWindowResize(event.window.data1, event.window.data2);
				}
				break;
			case SDL_MOUSEMOTION:
				float mouseX = (float)event.motion.xrel;
				float mouseY = (float)event.motion.yrel;
				myCamera_.processMouse(mouseX, mouseY);
		}

	}
}

void Game::shutdown()
{
	if (window_)
	{
		SDL_DestroyWindow(window_);
		window_ = nullptr;
	}
	SDL_Quit();
}