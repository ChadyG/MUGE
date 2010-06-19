/*
   TitleState.h
   My Unnamed Game Engine
 
   Created by Chad Godsey on 11/12/08.
  
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
 
#include <list>
#include <Gosu/Gosu.hpp>
#include "Core/GameState.h"

class Core;

/**
* State for title screen and other middleware screens
* goes into main menu
*/
class TitleState : public GameState
{
public:
	TitleState( std::wstring _config );
	
	void init();
	void cleanup();
	
	void pause();
	void resume();
	
	void update();
	void draw() const;
	
private:
	struct screen
	{
		Gosu::Image *image;
		int decayTime, duration;
		Gosu::Color fadeTo;
	};
	std::wstring m_ConfigFile;
	std::list<screen> m_Screens;
	std::list<screen>::iterator m_curScreen;
	int counter;
	bool fade;
};

class titleState_maker : public State_maker
{
public:
	titleState_maker() : State_maker("TitleState") {}
protected:
	GameState* makeState(std::wstring _config);
	static titleState_maker s_sRegisteredMaker;
};
