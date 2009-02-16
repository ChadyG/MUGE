/*
 *  MainMenuState.cpp
 *  Destructible
 *
 *  Created by Chad on 2/14/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */
 
#include "MainMenuState.h"
#include "Engine/MUGE.h"

/**
*
*/

MainMenuState MainMenuState::m_StateInstance;

void MainMenuState::init(Gosu::Graphics &graphics)
{
	std::wstring filename = Gosu::resourcePrefix() + L"Images/Menu_screen.png";
	m_MenuScreen.reset(new Gosu::Image(graphics, filename, false));
	filename = Gosu::resourcePrefix() + L"Images/arrow.png";
	m_Cursor.reset(new Gosu::Image(graphics, filename, false));
	filename = Gosu::resourcePrefix() + L"Images/cursor.png";
	m_MouseCursor.reset(new Gosu::Image(graphics, filename, false));
	
	m_Held = false;
	m_CursorPos = 0;
}

void MainMenuState::cleanup()
{
	m_MenuScreen.reset(0);
}

void MainMenuState::pause()
{

}

void MainMenuState::resume()
{

}

void MainMenuState::update(const Gosu::Input &input, MUGE* engine)
{
	if (input.down(Gosu::kbDown)) {
		if (!m_Held) {
			++m_CursorPos;
			if (m_CursorPos >= 4)
				m_CursorPos = 0;
		
			m_Held = true;
		}
	}else
	if (input.down(Gosu::kbUp)) {
		if (!m_Held) {
			--m_CursorPos;
			if (m_CursorPos < 0)
				m_CursorPos = 3;
		
			m_Held = true;
		}
	}else
		m_Held = false;
	

	// Mouse stuff
	bool lClick = false;
	m_mouseX = input.mouseX();
	m_mouseY = input.mouseY();
	if (m_mouseX > 200 && m_mouseX < 310 && m_mouseY > 200 && m_mouseY < 360) {
		m_CursorPos = (m_mouseY - 200)/45;
		
		if (input.down(Gosu::msLeft)) 
			lClick = true;
	}
	
	// Selection
	if (input.down(Gosu::kbReturn) || lClick) {
		switch (m_CursorPos) {
			case 0:// New
				break;
			case 1:// Load
				break;
			case 2:// About
				break;
			case 3:// Exit
				engine->popState();
				break;
		}
	}
	
	// Exit on escape
	if (input.down(Gosu::kbEscape)) {
		engine->popState();
	}
}

void MainMenuState::draw() const
{
	m_MenuScreen->draw(0,0,0);
	m_Cursor->draw(168, 200 + 45*m_CursorPos, 1);
	m_MouseCursor->draw(m_mouseX - 7, m_mouseY, 1);
}
