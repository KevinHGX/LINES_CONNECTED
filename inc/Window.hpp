#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#pragma once

extern "C"{
	#include "SDL.h"
	#include "SDL_image.h"
	#include "SDL_ttf.h"
}

struct Setting{
	bool exit = true;
	int WIDTH;
	int HEIGHT;
};

class Window{
public:
	Window() = default;
	explicit Window(const int _WIDHT,const int _HEIGHT);
	~Window(){
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
		SDL_DestroyWindow(window);
		window = nullptr;

		IMG_Quit();
		SDL_Quit();
	}
	
	void InitSDL();
	void GenerateWindow();
	void SetIcon();

	inline SDL_Renderer* GetRenderer() const{
		return this->renderer;
	} 

private:
	SDL_Window* window{nullptr};
	SDL_Renderer* renderer{nullptr};
public:
	Setting setting;
};

#endif