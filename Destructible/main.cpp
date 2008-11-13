#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include <Gosu/Window.hpp>

#include <boost/scoped_ptr.hpp> // Used throughout Gosu and this tutorial.
#include <boost/shared_ptr.hpp> // Learn them, they're moving into standard C++!
#include <boost/lexical_cast.hpp> // Could also use <sstream>, just for int <-> string conversion

#include <cmath>
#include <cstdlib>
#include <list>
#include <vector>
#include <stack>
#include <exception>

#include "GameState.h"

/**
* Entry point to the engine
* manages states, and sends Gosu callbacks
* to the current state
*/
class MyWindow : public Gosu::Window
{
	std::stack< GameState* > m_States;

public:
	MyWindow()
	: Gosu::Window(640, 480, false, 20)
	{
		
	}

	void changeState( GameState *state )
	{
		if (!m_States.empty()) {
			m_States.top()->cleanup();
			m_States.pop();
		}
		m_States.push(state);
	}
	
	void pushState( GameState *state )
	{
		m_States.push(state);
	}
	
	void popState()
	{
		m_States.pop();
		// Remove this for release (if desired)
		if (m_States.empty()) 
			close();
	}

	void update()
	{
		m_States.top()->update();
	}

	void draw()
	{
		m_States.top()->draw();
	}
	
};

int main(int argc, char* argv[])
{
    MyWindow win;
    win.show();
    return 0;
}