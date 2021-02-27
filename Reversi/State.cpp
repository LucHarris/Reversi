#include "State.h"
#include <assert.h>
State::State(ReversiSFML* app)
	:
	mpApp(app)
{
	assert(mpApp);
}
