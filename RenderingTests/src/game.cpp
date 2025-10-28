#include "game.h"
#include "renderer.h"
#include <SDL2/SDL.h>
#include <iostream>



Game::Game()
	: m_myCamera()
	, m_windowX(800.0f)
	, m_windowY(600.0f)
{

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

	m_window = SDL_CreateWindow(
		"Space Invaders",
		100,
		100,
		m_windowX,
		m_windowY,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);

	if (m_window == nullptr)
	{
		std::cout << "Error creating SDL window" << std::endl;
		return false;
	}

	if (!m_renderer.initialize(m_window, getProjectionMatrix()))
	{
		std::cout << "Error initializing renderer" << std::endl;
		return false;
	}


	m_isRunning = true;
	return true;

}

void Game::run()
{
	//Calcualate delta time
	Uint64 performanceFrequency = SDL_GetPerformanceFrequency();
	Uint64 lastTime = SDL_GetPerformanceCounter();

	m_isRunning = true;
	while (m_isRunning)
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

//as for now, player and camera are the same, so pressing directionals makes the camera move as if it was the player
void Game::update(float deltaTime)
{
	if (m_moveForward)
	{
		m_myCamera.processKeyboard(FORWARD, deltaTime);
	}
	if (m_moveBackward)
	{
		m_myCamera.processKeyboard(BACKWARD, deltaTime);
	}
	if (m_moveRight)
	{
		m_myCamera.processKeyboard(RIGHT, deltaTime);
	}
	if (m_moveLeft)
	{
		m_myCamera.processKeyboard(LEFT, deltaTime);
	}

}

glm::mat4 Game::getProjectionMatrix()
{
	return glm::perspective(glm::radians(45.0f), (float)m_windowX / (float)m_windowY, 0.1f, 100.0f);
}

void Game::render()
{
	m_renderer.clearScreen();

	m_renderer.renderScene(m_myCamera.getViewMatrix());

	m_renderer.present();
}

void Game::processInput()
{
	//resetting movement each frame, will be a player function
	m_moveForward = false;
	m_moveBackward = false;
	m_moveLeft = false;
	m_moveRight = false;

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				m_isRunning = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					m_isRunning = false;
				}
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					m_windowX = event.window.data1;
					m_windowY = event.window.data2;
					m_renderer.onWindowResize(m_windowX
						, m_windowY
						, getProjectionMatrix());
				}
				break;
			case SDL_MOUSEMOTION:
				float mouseX = (float)event.motion.xrel;
				float mouseY = (float)event.motion.yrel;
				m_myCamera.processMouse(mouseX, mouseY);
		}

	}

	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	if (keyboardState[SDL_SCANCODE_W])
	{
		m_moveForward = true;
	}
	if (keyboardState[SDL_SCANCODE_S])
	{
		m_moveBackward = true;
	}
	if (keyboardState[SDL_SCANCODE_D])
	{
		m_moveRight = true;
	}
	if (keyboardState[SDL_SCANCODE_A])
	{
		m_moveLeft = true;
	}
}

void Game::shutdown()
{
	if (m_window)
	{
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}
	SDL_Quit();
}