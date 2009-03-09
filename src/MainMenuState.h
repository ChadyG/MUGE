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
* State for title screen and other middleware screens
* goes into main menu
*/
class MainMenuState : public GameState
{
	boost::scoped_ptr<Gosu::Image> m_MenuScreen;
	boost::scoped_ptr<Gosu::Image> m_Cursor;
	boost::scoped_ptr<Gosu::Image> m_MouseCursor;
	boost::scoped_ptr<Gosu::Image> m_Letter_D;
	boost::scoped_ptr<Gosu::Image> m_Letter_E;
	boost::scoped_ptr<Gosu::Image> m_Letter_S;
	boost::scoped_ptr<Gosu::Image> m_Letter_T;
	boost::scoped_ptr<Gosu::Image> m_Letter_R;
	boost::scoped_ptr<Gosu::Image> m_Letter_U;
	boost::scoped_ptr<Gosu::Image> m_Letter_C;
	boost::scoped_ptr<Gosu::Image> m_Letter_I;
	boost::scoped_ptr<Gosu::Image> m_Letter_B;
	boost::scoped_ptr<Gosu::Image> m_Letter_L;
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