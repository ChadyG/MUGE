/*
 *  MainMenuState.h
 *  Destructible
 *
 *  Created by Chad on 11/12/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */
#include <Gosu/Gosu.hpp>
#include <box2D.h>
#include <list>
#include "Core/GameState.h"
#include "Physics/ContactListener.h"

class MUGE;
class UISheet;
class UIWindow;
//class UIText;

/**
 * State for main menu
 * central hub to move into other major game states
 *
 * This is a good place to toy around with things, as it is the first
 * piece of the game the player can interact with.
 */
class MainMenuState : public GameState
{
public:
	void init( MUGE* );
	void cleanup();
	
	void pause();
	void resume();
	
	void update();
	void draw() const;
	
	void CursorUp();
	void CursorDown();
	void CursorSelect();
	void LeftMouseDown();
	void LeftMouseUp();
	
	static MainMenuState* instance() {
		return &m_StateInstance;
	}
	
private:
	boost::scoped_ptr<Gosu::Image> m_MenuScreen;
	boost::scoped_ptr<Gosu::Image> m_Cursor;
	boost::scoped_ptr<Gosu::Image> m_MouseCursor;
	boost::scoped_ptr<Gosu::Sample> m_CursorMove;
	boost::scoped_ptr<Gosu::Sample> m_CursorSelect;
	boost::scoped_ptr<Gosu::Sample> m_PhysHit;
	boost::scoped_ptr<Gosu::Sample> m_PhysBigHit;
	std::vector< Gosu::Image* > m_LettersV;
	
	int m_CursorPos, m_lastCursorPos;
	b2Vec2 m_mousePos;
	bool m_Held, m_msLeftHeld;
	
	double m_width, m_height, m_units;
	int m_Iterations;
	float m_TimeStep;
	
	boost::scoped_ptr< b2World > m_Worldp;
	MenuListener m_ContactListener;
	std::list< b2Body* > m_Bodies;
	b2MouseJoint* m_mouseJoint;
	
	
	UISheet *m_UI;
	UIWindow *m_UIWin;
	
	static MainMenuState m_StateInstance;

protected:
	MainMenuState() { }
	
};