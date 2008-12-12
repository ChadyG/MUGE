#include <Gosu/Gosu.hpp>
#include <cmath>
#include <cstdlib>
#include <list>
#include <vector>
#include <stack>
#include <exception>

class GameState;

/**
* Entry point to the engine
* manages states, and sends Gosu callbacks
* to the current state
*/
class MUGE : public Gosu::Window
{
	std::stack< GameState* > m_States;

public:
	MUGE();

	void changeState( GameState *state );
	void pushState( GameState *state );
	void popState();
	void update();
	void draw();
};