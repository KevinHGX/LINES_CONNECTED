#include "../inc/Framework.hpp"
#include "../inc/Timer.hpp"

Framework::~Framework(){
	
	if(!stackC.empty()){ this->stackC.clear(); }

	delete window;
	window = nullptr;
}

void Framework::SetUp(){

	try{
		window->InitSDL();
		window->GenerateWindow();

		SetUpFont();
		this->logo.SetUpX();

	}catch(SDL_Exception const &e){
		window->setting.exit = false;
		std::cerr<<"SDL_Exception: "<<e.what()<<std::endl;
	}

	time.fpsTimer.start();
	time.capTimer.start();
	std::string backL="../res/square.png";
	this->logo.background = LoadTexture(backL); 

	this->rect[0] = {10,10,window->setting.WIDTH-20,window->setting.HEIGHT-20};
	this->rect[1] = {7,(window->setting.HEIGHT/2)-25,5,(window->setting.HEIGHT/2)-20};
	this->rect[2] = {(window->setting.WIDTH-15),30,5,(window->setting.HEIGHT/2)-20};
	
}

void Framework::SetUpFont(){
	this->font = TTF_OpenFont("../res/fonts/Kontanter-Bold.otf",40);
	
	if(!font){
		throw SDL_Exception(__FILE__,SDL_GetError(),__LINE__);
	}
	
	SDL_Color color = { 0, 0, 0, 255};
	SDL_Surface * surface_ttf = TTF_RenderText_Solid(font,"ABSTRACT", color);
	
	fontTexture = SDL_CreateTextureFromSurface(window->GetRenderer(),surface_ttf);
	
	if(!fontTexture){
		throw SDL_Exception(__FILE__,SDL_GetError(),__LINE__);
	}

	dest = {abs((surface_ttf->w/2)-(window->setting.WIDTH/2)),abs((surface_ttf->h/2)-(window->setting.HEIGHT/2)),surface_ttf->w,surface_ttf->h};
	
	SDL_FreeSurface(surface_ttf);
}


void Framework::Event(){
	SDL_PollEvent(&event);

	switch(event.type){
		case SDL_QUIT:
			window->setting.exit = false;
		break;
		case SDL_MOUSEBUTTONDOWN:
			try{
				if(event.button.button == SDL_BUTTON_LEFT) {
					NewObject(event.motion.x,event.motion.y);
				}
			}catch(SDL_Exception const &e){
				window->setting.exit = false;
				std::cerr<<"SDL_Exception: "<<e.what()<<std::endl;
			}
		break;
	}
}

void Framework::NewObject(int _x,int _y) {	

	int aux_angle,aux_radius,BASE=0;

	aux_angle = (0+rand()%360);//angulo
	aux_radius = (30+rand()%100);//radio

	Circle newCircle = { _x,_y,
						static_cast<int>(_x + cos(aux_angle)*aux_radius),
						static_cast<int>(_y + sin(aux_angle)*aux_radius),
						LoadTexture(ObjectPNG(aux_angle,BASE)),
						(4+rand()%8),aux_radius,
						((aux_angle < 180) ? aux_angle+180 : aux_angle-180),BASE };//Circle

	this->stackC.push_back(newCircle);
}

std::string Framework::ObjectPNG(int _random,int &_BASE){
	std::string key;

	if((_random%5) == 0){//RED
		key = ruta(circler.png);
		_BASE = CIRCLE_RED;
	}else{
		if((_random%2) == 0){//WHITE
			key = ruta(circle.png);
			_BASE = CIRCLE_WHITE;
		}else{//GRAY
			key = ruta(circleg.png);
			_BASE = CIRCLE_GRAY;
		}
	}

	return key;
}

SDL_Texture* Framework::LoadTexture(const std::string _file) const{
	SDL_Texture* newTexture{nullptr};
	
	newTexture = IMG_LoadTexture(window->GetRenderer(),_file.c_str());
	
	if(!newTexture){
		throw SDL_Exception(__FILE__,SDL_GetError(),__LINE__);
	}
	
	return newTexture;
}

void Framework::Clean() {
	SDL_SetRenderDrawColor(window->GetRenderer(), 40, 40, 48, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(window->GetRenderer());	
}

void Framework::Update(){

	try{
		this->logo.Update(static_cast<double>(this->aux),window);
	}catch(SDL_Exception const &e){
		std::cerr<<"SDL_Exception: "<<e.what()<<std::endl;
		window->setting.exit = false;
	}

	this->time.capTimer.start();
	this->time.Update();
	
	if(!this->stackC.empty()){
		size_t size=stackC.size();

		for (size_t i = 0; i < size ; ++i){
			stackC[i].UpdateAngle();
		}
	}
}

void Framework::Render(){

	if(!this->stackC.empty()){
		size_t size= stackC.size();
		for (size_t i = 0; i < size; ++i){
			this->scaled={stackC[i].x,stackC[i].y,10,10};
			SDL_RenderCopy(window->GetRenderer(),stackC[i].image,NULL,&scaled);//SDL_IMAGE
		}

		SDL_Rect A,B;

		for (size_t i = 0; i < size; ++i){
			for (size_t j = 0; j < size; ++j){
				SetColorLine(stackC[i].BASE);				
				if(SDL_HasIntersection(&(A = stackC[i].Colision()),&(B = stackC[j].Colision())) && i != j){
					SDL_RenderDrawLine(window->GetRenderer(),stackC[i].x+5,stackC[i].y+5,stackC[j].x+5,stackC[j].y+5);
				}
			}
		}	
	}	

	SDL_SetRenderDrawColor(window->GetRenderer(), 8,14,55, 50);
	SDL_RenderCopy(window->GetRenderer(),logo.x_rotate,NULL,&logo.x_scaled_first);//SDL_IMAGE_X
	SDL_RenderCopy(window->GetRenderer(),logo.x_rotate,NULL,&logo.x_scaled_second);//SDL_IMAGE_X
	SDL_RenderCopy(window->GetRenderer(),logo.background,NULL,&logo.scaled);//SDL_IMAGE
	SDL_RenderDrawRect(window->GetRenderer(),&logo.margen);	
	SDL_RenderCopy(window->GetRenderer(),this->fontTexture,NULL,&dest);//font
	Extras();

	SDL_RenderPresent(window->GetRenderer());
	this->time.Delay(this->aux);
}

void Framework::SetColorLine(int _BASE){	
	switch(_BASE){
		case CIRCLE_WHITE:
			SDL_SetRenderDrawColor(window->GetRenderer(), 255,255,255, SDL_ALPHA_OPAQUE);
			break;
		case CIRCLE_GRAY:
			SDL_SetRenderDrawColor(window->GetRenderer(), 131,134,134, SDL_ALPHA_OPAQUE);
			break;
		case CIRCLE_RED:
			SDL_SetRenderDrawColor(window->GetRenderer(), 255,0,0, SDL_ALPHA_OPAQUE);
			break;
	}
}

//-----------------------------------------------

void Framework::Extras(){

	SDL_SetRenderDrawColor(window->GetRenderer(), 255,255,255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(window->GetRenderer(),&this->rect[0]);	

	SDL_SetRenderDrawColor(window->GetRenderer(), 0,0,0, 50);
	SDL_RenderFillRect(window->GetRenderer(),&this->rect[1]);	
	SDL_RenderFillRect(window->GetRenderer(),&this->rect[2]);	
}