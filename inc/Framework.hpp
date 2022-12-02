#ifndef FRAMEWORK_HPP_
#define FRAMEWORK_HPP_

#pragma once

#include <iostream>
#include <string>
#include <exception>
#include <cmath>
#include <vector>
#include <algorithm>

class SDL_Exception: public std::exception{
	public:
		SDL_Exception(std::string _file,const char* _msg,int _line){
			_file += " ";
			aux.assign(_msg);
			this->message += _file + std::to_string(_line)+" -> " + aux; 
		}

		virtual const char* what() const throw(){
			return this->message.c_str();
		}

	private:
		std::string message = "";
		std::string aux;
};

//own headers CLASS
#include "Window.hpp"
#include "Timer.hpp"

#define AREA 90

enum ColorPNG{
	CIRCLE_WHITE,
	CIRCLE_GRAY,
	CIRCLE_RED
};

struct Circle{
	int x,y,sx,sy;
	SDL_Texture* image{nullptr};
	int radius,sradius,angle,BASE,cont = 0;

	SDL_Rect colision;

	void UpdateAngle(){
		x = sx + sin(angle) * sradius;
		y = sy + cos(angle) * sradius;

		if(cont++ == 4){
			if((angle+=6) >= 360){ angle = round(angle-360); } 
			cont = 0;
		}

		colision = {x-45,y-45,AREA,AREA};
	}

	SDL_Rect Colision() const {
		return colision;
	}
};

struct FPS{
	Timer fpsTimer,capTimer;

	float avgFPS = 0;
	int contFrames = 0,fpsCap = 60, frameTicks;
   	int screenTicksPerFrame = 1000 / fpsCap;
   	//-----------------------

   	void Update(){
   		avgFPS = contFrames/ (fpsTimer.GetTicks() / 1000.f);
   		contFrames++;
   	}

 	void Delay(int &_x_timeRotation){
 		frameTicks = capTimer.GetTicks();
 		if(frameTicks < screenTicksPerFrame){
 			if(++_x_timeRotation == 360){ _x_timeRotation = 0; } 
 			SDL_Delay(screenTicksPerFrame - frameTicks);
 		}
 	}  	
};

extern "C"{
	#include "../arc/SDL2_gfx-1.0.1/SDL2_rotozoom.h"
}

#define ruta(png) "../res/"#png;


struct Logo{
	SDL_Texture* background{nullptr};
	SDL_Rect scaled = {350,250,300,100};
	SDL_Rect margen = {363,266,275,75};
	//------------------------------------
	SDL_Surface* templateX{nullptr};
	SDL_Texture* x_rotate{nullptr};
	SDL_Rect x_scaled_first = {275,275,50,50};//1000 600
	SDL_Rect x_scaled_second = {675,275,50,50};//1000 600

	void SetUpX(){
		templateX = IMG_Load("../res/Xabstract_50.png");

		if(!templateX){
			throw SDL_Exception(__FILE__,SDL_GetError(),__LINE__);
		}
	}

	void Update(double x_angle,Window * window){
		SDL_Surface * surfaceX{nullptr};
		surfaceX = rotozoomSurface(templateX,x_angle,1.0,1);

		x_rotate = SDL_CreateTextureFromSurface(window->GetRenderer(),surfaceX);

		if(!surfaceX){
			throw SDL_Exception(__FILE__,SDL_GetError(),__LINE__);
		}

	}
};

class Framework{
	public:
		explicit Framework(int _WIDTH, int _HEIGHT){
			window = new Window(_WIDTH,_HEIGHT);
		}

		~Framework();
		
		void SetUp();
		void Event();
		void Clean();
		void Update();
		void Render();

		inline bool GetExitWindowF() const {
			return window->setting.exit;
		}

		void SetUpFont();

		void NewObject(int _x,int _y);

		//-----------------------
		SDL_Texture* LoadTexture(const std::string _file) const;
		void Extras();
		std::string ObjectPNG(int _random,int &_BASE);
		void SetColorLine(int _BASE);

		//-----------------------

	private:

		Window* window{nullptr};

		std::vector<Circle> stackC;
		FPS time;

		SDL_Event event;
		SDL_Rect rect[3];

		SDL_Rect scaled;

		TTF_Font* font{nullptr};
		SDL_Texture* fontTexture{nullptr};
		SDL_Rect dest;

		Logo logo;

		int aux = 0;
};

#endif