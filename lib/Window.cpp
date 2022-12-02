#include "../inc/Window.hpp"
#include "../inc/Framework.hpp" 

Window::Window(const int _WIDHT,const int _HEIGHT){
	this->setting.WIDTH = _WIDHT;
	this->setting.HEIGHT = _HEIGHT;
}

void Window::InitSDL(){
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		throw SDL_Exception(__FILE__,SDL_GetError(),__LINE__);
	}

	if(IMG_Init(IMG_INIT_PNG) == 0){
		throw SDL_Exception(__FILE__,SDL_GetError(),__LINE__);
	}

	if(TTF_Init() == (-1)){
		throw SDL_Exception(__FILE__,SDL_GetError(),__LINE__);
	}
}


void Window::GenerateWindow(){
	SDL_CreateWindowAndRenderer(this->setting.WIDTH,
								this->setting.HEIGHT,
								SDL_WINDOW_SHOWN,
								&window,&renderer);

	if(!this->window || !this->renderer){
		throw SDL_Exception(__FILE__,SDL_GetError(),__LINE__);
	}

	SDL_SetWindowTitle(this->window,"ABSTRACT");
	SDL_ShowCursor(0);
	SetIcon();
}

void Window::SetIcon(){
	SDL_Surface* icon = IMG_Load("../res/icon.png");

	if(!icon){ throw SDL_Exception(__FILE__,SDL_GetError(),__LINE__); }

	SDL_SetWindowIcon(this->window,icon);

	SDL_FreeSurface(icon);
}
