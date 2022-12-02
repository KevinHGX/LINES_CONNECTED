#include "../inc/Framework.hpp"

int main(int , char** ){ //SDL_main
		
	Framework Point(1000,600);

		Point.SetUp();

		while(Point.GetExitWindowF()){
			Point.Event();	
			Point.Clean();
			Point.Update();
			Point.Render();
		}

	Point.~Framework();


	return 0;
}
