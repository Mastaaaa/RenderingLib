#include "game.h"
#include "renderer.h"
#include <SDL2/SDL.h>
#include <iostream>


Game::Game()
{
	window_ = nullptr;
	renderer_ = nullptr;
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

	window_ = SDL_CreateWindow(
		"Space Invaders",
		100,
		100,
		800,
		800,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);

	if (window_ == nullptr)
	{
		std::cout << "Error creating SDL window" << std::endl;
		return false;
	}

	renderer_ = new Renderer();
	if (!renderer_->initialize(window_))
	{
		std::cout << "Error initializing Renderer" << std::endl;
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

}

void Game::render()
{
	if (renderer_)
	{
		renderer_->clearScreen();

		renderer_->renderTriangle();
		
		renderer_->present();
	}
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
					renderer_->onWindowResize(event.window.data1, event.window.data2);
				}
				break;
		}

	}
}

void Game::shutdown()
{
	if (renderer_)
	{
		delete renderer_;
		renderer_ = nullptr;
	}

	if (window_)
	{
		SDL_DestroyWindow(window_);
		window_ = nullptr;
	}

	SDL_Quit();
}