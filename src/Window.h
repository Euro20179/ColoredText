#pragma once
#include "SDL2/SDL.h"
#include "propertystructs.h"
#include <iostream>

class Window {
public:
	Window(const char* title, int x, int y, int w, int h, int flags) {
		window = SDL_CreateWindow(title, x, y, w, h, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
	}
	SDL_Window* getWindow() {
		return window;
	}
	SDL_Renderer* getRenderer() {
		return renderer;
	}
	void clear(int r, int g, int b, int a) {
		SDL_SetRenderDrawColor(renderer, r, g, b, a);
		SDL_RenderClear(renderer);
	}
	void present() {
		SDL_RenderPresent(renderer);
	}
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
};