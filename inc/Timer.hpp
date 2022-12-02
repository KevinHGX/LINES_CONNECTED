#ifndef TIMER_HPP_
#define TIMER_HPP_

extern "C"{
	#include <SDL.h>
}

class Timer{
public:
	explicit Timer():t_start(0){}

	inline void start(){ this->t_start = SDL_GetTicks64(); }

	inline Uint32 GetTicks(){ return (SDL_GetTicks64()-this->t_start);}

private:
	Uint32 t_start;
};

#endif