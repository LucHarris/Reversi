
//#include <SFML/Graphics.hpp>
//#include <SFML/Graphics/Color.hpp>

#include "ReversiSFML.h"

int main()
{
	srand((unsigned)time(NULL));

	ReversiSFML app;
	
	app.Init();
	app.Run();

}