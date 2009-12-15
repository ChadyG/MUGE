/*
   TitleState.cpp
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
 
#include "TitleState.h"
#include "MainMenuState.h"
#include "Core/Core.h"

/**
*
*/

TitleState::TitleState( std::wstring _config )
{

}

void TitleState::init()
{
	m_Engine = Core::getCurrentContext();
	std::wstring filename = Gosu::resourcePrefix() + L"Images/MUGD_Title_screen.png";
	m_TitleScreen.reset(new Gosu::Image(m_Engine->graphics(), filename, false));
	
	counter = 256;
	step = 0;
}

void TitleState::cleanup()
{
	m_TitleScreen.reset(0);
}

void TitleState::pause()
{

}

void TitleState::resume()
{

}

void TitleState::update()
{
	if (m_Engine->input().down(Gosu::kbReturn) || counter <= 0) {
		MainMenuState *state = new MainMenuState( std::wstring(L"MainMenuState") );
		m_Engine->changeState( state );
	}
	--counter;
}

void TitleState::draw() const
{
	m_TitleScreen->draw(0,0,0, 1,1, Gosu::Color::fromHSV(0, 0, counter));
}
