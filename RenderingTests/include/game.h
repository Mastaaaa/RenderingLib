#pragma once
#include <vector>
#include "renderer.h"
#include <SDL2/SDL.h>


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


	private:
		void processInput();
		void update(float delta);
		void render();

		//Variables
		SDL_Window* window_;
		Renderer renderer_;
		Camera myCamera_;
		bool isRunning_;
};