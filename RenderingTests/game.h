#pragma once
#include <vector>


struct SDL_Window;
class Renderer;

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
		Renderer* renderer_;
		bool isRunning_;
};