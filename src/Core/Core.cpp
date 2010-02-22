/*
	Core.cpp
	My Unnamed Game Engine
 
	Created by Chad Godsey on 12/12/08.
	Copyright 2009 BlitThis! studios.

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
 */
#include "Core.h"
#include "GameState.h"
#include <boost/bind.hpp>

Core* Core::s_CurrentContext;

/**
* Entry point to the engine
* manages states, and sends Gosu callbacks
* to the current state
*/
Core::Core(int _width, int _height, bool _fullscreen, double _updateInterval)
: Gosu::Window(_width, _height, _fullscreen, _updateInterval)
{
	InputManager::setCurrentContext( &m_inputManager );

	m_curFPS = 1;
	m_curTicks = 0;
	m_lastSecond = Gosu::milliseconds()/1000;
	m_stackDirty = false;
}

void Core::buttonDown(Gosu::Button _button)
{
	m_inputManager.buttonDown(_button);
}

void Core::buttonUp(Gosu::Button _button)
{
	m_inputManager.buttonUp(_button);
}

void Core::changeState( GameState *state )
{
	m_stackDirty = true;
	if (!m_States.empty()) {
		m_States.top()->setFocus(false);
		m_States.top()->setDirty();
	}
	m_NextStates.push( boost::shared_ptr<GameState>(state) );
}

void Core::pushState( GameState *state )
{
	m_stackDirty = true;
	if (!m_States.empty())
		m_States.top()->setFocus(false);
	m_NextStates.push( boost::shared_ptr<GameState>(state) );
}

void Core::popState()
{
	m_stackDirty = true;
	m_States.top()->setFocus(false);
	m_States.top()->setDirty();
	if (m_States.empty()) 
		close();
}

void Core::update()
{
	m_curTicks++;
	if (Gosu::milliseconds()/1000 != m_lastSecond) {
		m_curFPS = m_curTicks;
		m_curTicks = 0;
		m_lastSecond = Gosu::milliseconds()/1000;
	}
	
	if (m_stackDirty) {
		// Look for pending deletions
		while (!m_States.empty() && m_States.top()->dirty()) {
			m_States.top()->cleanup();
			m_States.pop();
		}
		// Transfer pending states to current stack
		while (!m_NextStates.empty()) {
			m_States.push(m_NextStates.front());
			m_NextStates.front()->init();
			m_NextStates.pop();
		}
		if (!m_States.empty())
			m_States.top()->setFocus(true);
		m_stackDirty = false;
	}
	
	if (!m_States.empty()) {
		m_States.top()->update();
	}else
		close();
		
	if (input().down(Gosu::kbEscape))
		close();

	// Done last as to keep queries valid for each game tick
	m_inputManager.update();
}

void Core::draw()
{
	m_font->draw(m_message, 4, 4, 15);
	if (!m_States.empty())
		m_States.top()->draw();
}
/*
void Core::quitHandler()
{
	close();
}
*/
int Core::getFPS()
{
	return m_curFPS;
}
