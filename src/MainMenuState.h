/*
 *  MainMenuState.h
 *  Destructible
 *
 *  Created by Chad on 11/12/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */
#include <Gosu/Gosu.hpp>
#include "Engine/GameState.h"

class MUGE;

/**
* State for title screen and other middleware screens
* goes into main menu
*/
class MainMenuState : public GameState
{
	boost::scoped_ptr<Gosu::Image> m_MenuScreen;
	boost::scoped_ptr<Gosu::Image> m_Cursor;
	boost::scoped_ptr<Gosu::Image> m_MouseCursor;
	int m_CursorPos;
	int m_mouseX, m_mouseY;
	bool m_Held;
	
	static MainMenuState m_StateInstance;

protected:
	MainMenuState() { }
	
public:
	void init(Gosu::Graphics &graphics);
	void cleanup();
	
	void pause();
	void resume();
	
	void update(const Gosu::Input &, MUGE *);
	void draw() const;
	
	static MainMenuState* instance() {
		return &m_StateInstance;
	}
};