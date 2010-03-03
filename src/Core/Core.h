/*
	Core.h
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
#include <Gosu/Gosu.hpp>
#include <stack>
#include <queue>
#include "../Input/InputManager.h"

#include <string>

class GameState;

/**
 * Entry point to the engine
 * manages states, and sends Gosu callbacks
 * to the current state
 **/
class Core : public Gosu::Window
{
public:
	/// This constructor is wrapped in main, parameters are given by main.json
	/// this provides us with a configurable window and clock
	Core(int _width=640, int _height=480, bool _fullscreen=false, double _updateInterval=20);

	/// Standard state switching, given state will replace current 
	/// state on top of stack
	void changeState( GameState *state );
	
	/// Switch to this state, but do not replace current state.
	/// Calls pause on topmost state.
	void pushState( GameState *state );
	
	/// Cleans up current state and removes it.
	/// Be sure to have states beneath or the game will exit!
	void popState();
	
	/// Game tick, called from Gosu and sends to current state.
	void update();
	
	/// Render callback from Gosu, sends to current state.
	void draw();

	/// Input event callback from Gosu, sent to current state.
	void buttonDown(Gosu::Button _button);
	
	/// Input event callback from Gosu, sent to current state.
	void buttonUp(Gosu::Button _button);
	
	/// Returns dynamic FPS calculation
	int getFPS();

	/// Returns set FPS
	int getUpdateRate() { return m_FPS; }

	/// Static accessor to current state manager (central game class)
	static Core* getCurrentContext() { return s_CurrentContext; }

	/// Static setter for current state manager
	static void setCurrentContext(Core* _context) { s_CurrentContext = _context; }
	
private:
	
	static Core* s_CurrentContext;

	InputManager m_inputManager;
	std::stack< boost::shared_ptr<GameState> > m_States;
	std::queue< boost::shared_ptr<GameState> > m_NextStates;
	
	int m_FPS;
	int m_curFPS;
	int m_curTicks;
	int m_lastSecond;
	
	bool m_stackDirty;

	Gosu::Font *m_font;
	std::wstring m_message;
};
/*
namespace Core
{
	typedef b2Vec2 Point2D;
	
	
}
*/