
//#include <SFML/Graphics.hpp>
//#include <SFML/Graphics/Color.hpp>

#include "ReversiSFML.h"

void reposition(char a)
{
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);

	r.left += 20 * a;
	r.top += 20 * a;
	MoveWindow(console, r.left, r.top, r.right, r.bottom, TRUE);
}

char SelectProfile(char a)
{
	return (a >= 0 && a < 5) ? a : 0;
}

int main(int argc, char* argv[])
{
	char profile = 0;
	if (argc == 2)
	{
		profile = SelectProfile(*argv[1]);
		reposition(profile);
	}

	srand((unsigned)time(NULL));

	ReversiSFML app;
	app.profileIndex = profile;
	
	app.Init();
	app.Run();

}