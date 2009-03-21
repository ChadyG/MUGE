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
#include "Engine/GameState.h"

class MUGE;

/**
 * State for main menu
 * central hub to move into other major game states
 *
 * This is a good place to toy around with things, as it is the first
 * piece of the game the player can interact with.
 */
class MainMenuState : public GameState
{
	boost::scoped_ptr<Gosu::Image> m_MenuScreen;
	boost::scoped_ptr<Gosu::Image> m_Cursor;
	boost::scoped_ptr<Gosu::Image> m_MouseCursor;
	std::vector< Gosu::Image* > m_LettersV;
	
	int m_CursorPos;
	b2Vec2 m_mousePos;
	bool m_Held, m_msLeftHeld;
	
	double m_width, m_height, m_units;
	int m_Iterations;
	float m_TimeStep;
	
	boost::scoped_ptr< b2World > m_Worldp;
	std::list< b2Body* > m_Bodies;
	b2MouseJoint* m_mouseJoint;
	
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