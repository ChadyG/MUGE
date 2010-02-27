/*
   MainMenuState.cpp
   My Unnamed Game Engine
 
   Created by Chad Godsey on 2/14/08.
  
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
 
#include "MainMenuState.h"
#include "AdventureState.h"
#include "Core/Core.h"

/**
*
*/

MainMenuState::MainMenuState( std::wstring _config )
{

}

void MainMenuState::init()
{	
	m_Engine = Core::getCurrentContext();

	m_Font.reset( new Gosu::Font(m_Engine->graphics(), Gosu::defaultFontName(), 20));

	std::wstring filename = Gosu::resourcePrefix() + L"Images/Menu/Menu_screen.png";
	m_MenuScreen.reset(new Gosu::Image(m_Engine->graphics(), filename, false));
	filename = Gosu::resourcePrefix() + L"Images/Menu/About_screen.png";
	m_AboutScreen.reset(new Gosu::Image(m_Engine->graphics(), filename, false));
	filename = Gosu::resourcePrefix() + L"Images/Menu/Scores_screen.png";
	m_ScoresScreen.reset(new Gosu::Image(m_Engine->graphics(), filename, false));

	filename = Gosu::resourcePrefix() + L"Images/Menu/arrow.png";
	m_Cursor.reset(new Gosu::Image(m_Engine->graphics(), filename, false));
	filename = Gosu::resourcePrefix() + L"Images/Menu/cursor.png";
	m_MouseCursor.reset(new Gosu::Image(m_Engine->graphics(), filename, false));
	
	//Sounds!
	filename = Gosu::resourcePrefix() + L"Sound/cursor_move.wav";
	m_CursorMove.reset(new Gosu::Sample(m_Engine->audio(), filename));
	filename = Gosu::resourcePrefix() + L"Sound/takin_drugz.wav";
	m_CursorSelect.reset(new Gosu::Sample(m_Engine->audio(), filename));
	filename = Gosu::resourcePrefix() + L"Sound/music.ogg";
	m_Music.reset(new Gosu::Song(m_Engine->audio(), filename));

	m_State = eMenu;
	m_Held = false;
	m_msLeftHeld = false;
	m_CursorPos = 0;
	m_lastCursorPos = 0;
	m_units = 10.0;
	m_width = m_Engine->graphics().width();
	m_height = m_Engine->graphics().height();
}

void MainMenuState::cleanup()
{
	m_MenuScreen.reset(0);
	m_AboutScreen.reset(0);
	m_ScoresScreen.reset(0);
	m_Cursor.reset(0);
	m_MouseCursor.reset(0);
}

void MainMenuState::pause()
{

}

void MainMenuState::resume()
{

}

void MainMenuState::update()
{
	//keep that music playing!
	if (!m_Music->playing()) {
		m_Music->play();
	}
	
	InputManager* input = InputManager::getCurrentContext();
	
	// Mouse stuff
	bool lClick = false;
	m_mousePos.x = m_Engine->input().mouseX();
	m_mousePos.y = m_Engine->input().mouseY();
	if (m_mousePos.x > 200 && m_mousePos.x < 310 && m_mousePos.y > 200 && m_mousePos.y < 360) {
		m_CursorPos = (m_mousePos.y - 200)/45;
		if (input->query("Menu.MouseSelect") == InputManager::actnBegin) {
			lClick = true;
		}
	}
	
	if (input->query("Menu.Select") == InputManager::actnBegin) {
		lClick = true;
	}
	
	// Selection sounds
	if (m_lastCursorPos != m_CursorPos) {
		m_CursorMove->play();
		m_lastCursorPos = m_CursorPos;
	}
	
	AdventureState *state;
	// Selection
	if (lClick) {
		m_CursorSelect->play();
		switch (m_CursorPos) {
			case 0:// New
				state = new AdventureState( std::wstring(L"AdventureState") );
				m_Engine->changeState( state );
				break;
			case 1:// Scores
				break;
			case 2:// About
				break;
			case 3:// Exit
				m_Engine->popState();
				break;
		}
	}
}

void MainMenuState::draw() const
{
	m_MenuScreen->draw(0,0,0);
	m_Cursor->draw(168, 200 + 45*m_CursorPos, 2);
	m_MouseCursor->draw(m_mousePos.x - 7, m_mousePos.y, 3);
}
