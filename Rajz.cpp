#define SDL_MAIN_HANDLED
// SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

// standard
#include <vector>
#include <iostream>
#include <sstream>

struct MouseEvent {
	bool pressed;
	float x;
	float y;
	int button;

	MouseEvent() : pressed(false), x(0), y(0), button(0) {}
};

void handle_events(bool& running, MouseEvent &mouseEvent);

int main(int argc, char* argv[])
{
	MouseEvent mouseEventHandler;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	bool running = true;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Rajz", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);

	if (window == NULL) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return -1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == NULL) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return -1;
	}

	bool pressed = false;

    std::vector<SDL_Point> points;
	float Mouse_x = 0;
	float Mouse_y = 0;

	while (running)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		handle_events(running, mouseEventHandler);
		if (mouseEventHandler.pressed)
		{
			if (mouseEventHandler.button == 0)
			{
				points.insert(points.end(), SDL_Point{ static_cast<int>(mouseEventHandler.x), static_cast<int>(mouseEventHandler.y) });
			}
			else
			{
				//points.clear();
				if (!points.empty())
				{
					points.pop_back();
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawLines(renderer, points.data(), points.size());

		if (!points.empty())
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
			SDL_RenderDrawLine(renderer, points.back().x, points.back().y, mouseEventHandler.x, mouseEventHandler.y);
		}

		SDL_RenderPresent(renderer);

	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	return 0;
}

void handle_events(bool& running, MouseEvent &mouseEvent) {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			running = false;
		}
		else if(event.type == SDL_KEYDOWN)
		{
			if(event.key.keysym.sym == SDLK_ESCAPE)
			{
				running = false;
			}
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN) {
			mouseEvent.pressed = true;
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				mouseEvent.button = 0;
			}
			else {
				mouseEvent.button = 1;
			}
		}
		else if (event.type == SDL_MOUSEBUTTONUP) {
			mouseEvent.pressed = false;
		}
		else if (event.type == SDL_MOUSEMOTION) {
			mouseEvent.x = event.motion.x;
			mouseEvent.y = event.motion.y;
		}
	}
}

