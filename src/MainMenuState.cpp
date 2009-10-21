/*
   MainMenuState.cpp
   Mizzou Game Engine
 
   Created by Chad Godsey on 2/14/08.
  
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
 
#include "MainMenuState.h"
#include "AdventureState.h"
#include "Core/MUGE.h"
#include "UI/GUIObjects.h"
#include <boost/bind.hpp>

/**
*
*/

MainMenuState::MainMenuState( std::wstring _config )
{
	m_ConfigFile = _config;
}

void MainMenuState::init( MUGE* _engine)
{	
	m_Engine = _engine;

	m_jFile.reset( new JSONFile(Gosu::narrow(Gosu::resourcePrefix() + L"Data/" + m_ConfigFile + L".json")));
	json::grammar<char>::array::const_iterator it;
	json::grammar<char>::array arr;

	//m_Engine->hookIntoCommand("Menu.CursorDown:Down", boost::bind(&MainMenuState::CursorDown, this));
	//m_Engine->hookIntoCommand("Menu.CursorUp:Down", boost::bind(&MainMenuState::CursorUp, this));
	//m_Engine->setCurrentContext("Menu");


	// This functionality will be pushed to script (selectable list)
	std::wstring filename = Gosu::resourcePrefix() + L"Images/Menu/arrow.png";
	m_Cursor.reset(new Gosu::Image(m_Engine->graphics(), filename, false));

	filename = Gosu::resourcePrefix() + L"Images/" + Gosu::widen(m_jFile->get< std::string >("Background"));
	m_MenuScreen.reset(new Gosu::Image(m_Engine->graphics(), filename, false));
	filename = Gosu::resourcePrefix() + L"Images/" + Gosu::widen(m_jFile->get< std::string >("CursorImage"));
	m_MouseCursor.reset(new Gosu::Image(m_Engine->graphics(), filename, false));
	
	//Sounds!
	filename = Gosu::resourcePrefix() + L"Sound/cursor_move.wav";
	m_CursorMove.reset(new Gosu::Sample(m_Engine->audio(), filename));
	filename = Gosu::resourcePrefix() + L"Sound/takin_drugz.wav";
	m_CursorSelect.reset(new Gosu::Sample(m_Engine->audio(), filename));
	
	// Maybe we should make a UICursor...
	m_Held = false;
	m_msLeftHeld = false;
	//Temp
	m_CursorPos = 0;
	m_lastCursorPos = 0;
	//Screen stuff
	m_units = 10.0;
	m_width = m_Engine->graphics().width();
	m_height = m_Engine->graphics().height();
	
	
	// UI Stuff (finally!)
	m_UI = new UIScreen(m_Engine);
	m_UI->giveFocus();
	
	arr = m_jFile->get<json::grammar<char>::array>("Objects");
	evalJSON( arr, m_UI );
	
}

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

void MainMenuState::cleanup()
{
	m_MenuScreen.reset(0);
	m_Cursor.reset(0);
	m_MouseCursor.reset(0);
	m_Worldp.reset(0);
}

void MainMenuState::pause()
{

}

void MainMenuState::resume()
{

}

void MainMenuState::CursorUp()
{
	--m_CursorPos;
	if (m_CursorPos < 0)
		m_CursorPos = 3;
}

void MainMenuState::CursorDown()
{
	++m_CursorPos;
	if (m_CursorPos >= 4)
		m_CursorPos = 0;
}

void MainMenuState::update()
{
	m_UI->update();
	/*if (m_UIWin->isClosed() && m_UIWin->isVisible()) {
		m_UIWin->takeFocus();
		m_UIWin->hide();
	}*/
	
	
	// Mouse stuff
	bool lClick = false;
	m_mousePos.x = m_Engine->input().mouseX();
	m_mousePos.y = m_Engine->input().mouseY();
	b2Vec2 mouse2d = m_mousePos;
	mouse2d *= (1.0f/m_units);
	if (m_mousePos.x > 200 && m_mousePos.x < 310 && m_mousePos.y > 200 && m_mousePos.y < 360) {
		m_CursorPos = (m_mousePos.y - 200)/45;
	}
	
	if (m_Engine->input().down(Gosu::msLeft)) {
		// Is this a new click or hold?
		if (!m_msLeftHeld) {
			// Are we over the menu items?
			if (m_mousePos.x > 200 && m_mousePos.x < 310 && m_mousePos.y > 200 && m_mousePos.y < 360) {
				lClick = true;
			}
		}
		m_msLeftHeld = true;																																							
	}else
		m_msLeftHeld = false;
	
	
	// Selection sounds
	if (m_lastCursorPos != m_CursorPos) {
		m_CursorMove->play();
		m_lastCursorPos = m_CursorPos;
	}
	
	AdventureState *state;
	// Selection
	if (/*m_Engine->input().down(Gosu::kbReturn) || */lClick) {
		m_CursorSelect->play();
		switch (m_CursorPos) {
			case 0:// New
				state = new AdventureState( std::wstring(L"AdventureState") );
				m_Engine->changeState( state );
				break;
			case 1:// Load
				break;
			case 2:// About
				//m_UIWin->show();
				//m_UIWin->giveFocus();
				break;
			case 3:// Exit
				m_Engine->popState();
				break;
		}
	}
	/*
	// Exit on escape
	if (m_Engine->input().down(Gosu::kbEscape)) {
		m_Engine->popState();
	}
	*/
}

void MainMenuState::draw() const
{
	m_MenuScreen->draw(0,0,0);
	m_Cursor->draw(168, 200 + 45*m_CursorPos, 2);
	m_MouseCursor->draw(m_mousePos.x - 7, m_mousePos.y, 3);
	
	m_UI->draw(2);
}

b2Vec2 MainMenuState::worldToScreen( b2Vec2 _world, Gosu::ZPos _layer )
{
	return _world;
}