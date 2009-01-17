#include "MUGE.h"
#include "GameState.h"

/**
* Entry point to the engine
* manages states, and sends Gosu callbacks
* to the current state
*/
MUGE::MUGE()
	: Gosu::Window(640, 480, false, 20)
{
		
}

void MUGE::changeState( GameState *state )
{
	if (!m_States.empty()) {
		m_States.top()->cleanup();
		m_States.pop();
	}
	m_States.push(state);
	state->init(graphics());
}

void MUGE::pushState( GameState *state )
{
	m_States.push(state);
	state->init(graphics());
}

void MUGE::popState()
{
	m_States.pop();
	// Remove this for release (if desired)
	if (m_States.empty()) 
		close();
}

void MUGE::update()
{
	if (!m_States.empty())
		m_States.top()->update(input(), this);
}

void MUGE::draw()
{
	if (!m_States.empty())
		m_States.top()->draw();
}