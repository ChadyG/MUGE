/*
   MUGE.h
   Mizzou Game Engine
 
   Created by Chad Godsey on 12/12/08.
  
  
 Copyright 2009 Mizzou Game Design.

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
#include "../Input/InputManager.hpp"

class GameState;

/**
 * Entry point to the engine
 * manages states, and sends Gosu callbacks
 * to the current state
 **/
class MUGE : public Gosu::Window
{
public:
	MUGE(int _width=640, int _height=480, bool _fullscreen=false, double _updateInterval=20);

	void changeState( GameState *state );
	void pushState( GameState *state );
	void popState();
	void update();
	void draw();

	void buttonDown(Gosu::Button button);
	void buttonUp(Gosu::Button button);
     
	void hookIntoCommand(const std::string& command, const InputManager::CommandSignalType::slot_type& slot);
	void setCurrentContext(const std::string& newContext);

	void quitHandler();
	
	int getFPS();
	
private:
	InputManager inputManager;
	std::stack< GameState* > m_States;
	std::queue< GameState* > m_NextStates;
	
	int m_curFPS;
	int m_curTicks;
	int m_lastSecond;
	
	bool m_stackDirty;

};
