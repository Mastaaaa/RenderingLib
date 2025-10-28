#pragma once
#include <vector>
#include "renderer.h"
#include "camera.h"
#include <SDL2/SDL.h>
#include <glm/gtc/matrix_transform.hpp>


struct SDL_Window;
class Renderer;
class Camera;

class Game
{
	public:
		Game();
		~Game();
		bool initialize();
		void run();
		void shutdown();

		glm::mat4 getProjectionMatrix();


	private:
		void processInput();
		void update(float delta);
		void render();

		//to move later into the player object, now is tied to the camera as if it was the player
		bool m_moveForward;
		bool m_moveBackward;
		bool m_moveLeft;
		bool m_moveRight;

		//Variables
		SDL_Window* m_window;
		Renderer m_renderer;
		Camera m_myCamera;
		float m_windowX;
		float m_windowY;
		bool m_isRunning;
};