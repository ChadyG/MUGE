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
 
#include <Core/Core.h>

#include "MainMenuState.h"
#include "AdventureState.h"

/**
*  Set up pluggable factory
*/

menuState_maker menuState_maker::s_sRegisteredMaker;

GameState* menuState_maker::makeState(std::wstring _config)
{
	return new MainMenuState(_config);
}

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

	filename = Gosu::resourcePrefix() + L"Images/Menu/arrow.png";
	m_Cursor.reset(new Gosu::Image(m_Engine->graphics(), filename, false));
	filename = Gosu::resourcePrefix() + L"Images/Menu/cursor.png";
	m_MouseCursor.reset(new Gosu::Image(m_Engine->graphics(), filename, false));
	
	//filename = Gosu::resourcePrefix() + L"Images/" + Gosu::widen(m_jFile->get< std::string >("Background"));
	//m_MenuScreen.reset(new Gosu::Image(m_Engine->graphics(), filename, false));
	//filename = Gosu::resourcePrefix() + L"Images/" + Gosu::widen(m_jFile->get< std::string >("CursorImage"));
	//m_MouseCursor.reset(new Gosu::Image(m_Engine->graphics(), filename, false));
	
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
	
	// UI Stuff (finally!)
	//m_UI = new UIScreen(m_Engine);
	
	//arr = m_jFile->get<json::grammar<char>::array>("Objects");
	//evalJSON( arr, m_UI );
}
/* Holy crap when did I lose these?
void MainMenuState::evalJSON( json::grammar<char>::array _array, UIScreen *_container )
{
	std::string type;
	json::grammar<char>::array::const_iterator it;
	for (it = _array.begin(); it != _array.end(); ++it) {
		type = m_jFile->get<std::string>("Type", *it);
		if (type == "Button") {
			evalButton( it, _container );
		}
		if (type == "TextBox" ) {
			evalTextBox( it, _container );
		}
		if (type == "TextArea" ) {
			evalTextArea( it, _container );
		}
		if (type == "Window" ) {
			evalWindow( it, _container );
		}
	}
}

void MainMenuState::evalButton( json::grammar<char>::array::const_iterator _it, UIContainer *_container )
{
	buttonDef bDef;
	bDef.height = m_jFile->get< int >("Height", *_it);
	bDef.width = m_jFile->get< int >("Width", *_it);
	bDef.x = m_jFile->get< int >("Position[0]", *_it);
	bDef.y = m_jFile->get< int >("Position[1]", *_it);
	UIButton* button = _container->createButton(bDef);
	std::wstring filename = Gosu::resourcePrefix() + L"Images/StartButton.png";
	button->setImage(filename);
	m_UIObjects.push_back( button );
	m_UINames[m_jFile->get< std::string >("Name", *_it)] = button;
}

void MainMenuState::evalTextBox( json::grammar<char>::array::const_iterator _it, UIContainer *_container )
{
	texDef tDef;
	tDef.x = m_jFile->get< int >("Position[0]", *_it);
	tDef.y = m_jFile->get< int >("Position[1]", *_it);
	tDef.width = m_jFile->get< int >("Width", *_it);
	tDef.maxsize = m_jFile->get< int >("maxSize", *_it);
	UITextBox* textBox = _container->createTextBox( tDef );
	textBox->setText( Gosu::widen(m_jFile->get< std::string >("Text", *_it)) );
	m_UIObjects.push_back( textBox );
	m_UINames[m_jFile->get< std::string >("Name", *_it)] = textBox;
}

void MainMenuState::evalTextArea( json::grammar<char>::array::const_iterator _it, UIContainer *_container )
{
	texAreaDef tDef;
	tDef.x = m_jFile->get< int >("Position[0]", *_it);
	tDef.y = m_jFile->get< int >("Position[1]", *_it);
	tDef.width = m_jFile->get< int >("Width", *_it);
	
	std::string justification =  m_jFile->get< std::string >("Justify", *_it);
	if (justification == "Left")
		tDef.justify = UITextArea::Left;
	if (justification == "Right")
		tDef.justify = UITextArea::Right;
	if (justification == "Center")
		tDef.justify = UITextArea::Center;
	
	UITextArea* textArea = _container->createTextArea( tDef );
	textArea->setText( Gosu::widen(m_jFile->get< std::string >("Text", *_it)) );
	m_UIObjects.push_back( textArea );
	m_UINames[m_jFile->get< std::string >("Name", *_it)] = textArea;
}

void MainMenuState::evalWindow( json::grammar<char>::array::const_iterator _it, UIScreen *_container )
{
	windowDef wDef;
	wDef.width = m_jFile->get< int >("Width", *_it);
	wDef.height = m_jFile->get< int >("Height", *_it);
	UIWindow* win = _container->createWindow(wDef);
	win->giveFocus();
	m_UIObjects.push_back( win );
	m_UINames[m_jFile->get< std::string >("Name", *_it)] = win;
	
	json::grammar<char>::array childrenArray = m_jFile->get<json::grammar<char>::array>("Objects", *_it);
	if (!childrenArray.empty()) {
		std::string type;
		json::grammar<char>::array::const_iterator it;
		for (it = childrenArray.begin(); it != childrenArray.end(); ++it) {
			type = m_jFile->get<std::string>("Type", *it);
			if (type == "Button") {
				evalButton( it, win );
			}
			if (type == "TextBox" ) {
				evalTextBox( it, win );
			}
			if (type == "TextArea" ) {
				evalTextArea( it, win );
			}
		}
	}
}
*/
void MainMenuState::cleanup()
{
	m_MenuScreen.reset(0);
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
