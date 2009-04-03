/*
 *  AdventureState.cpp
 *  Destructible
 *
 *  Created by Chad on 3/9/09.
 *  Copyright 2009 University of Missouri Columbia. All rights reserved.
 *
 */

#include "AdventureState.h"
#include "Engine/MUGE.h"

/**
 *
 */

AdventureState AdventureState::m_StateInstance;

void AdventureState::init( MUGE* _engine)
{	
	m_Engine = _engine;
	m_Environment.reset( new Environment(Gosu::resourcePrefix() + L"Data/test_level.json", m_Engine->graphics()));
	m_Music.reset( new Gosu::Song( m_Engine->audio(), Gosu::resourcePrefix() + L"Sound/Colugo-Fantastic_face.ogg"));
	m_Music->play(true);
}

void AdventureState::cleanup()
{
	m_Environment.reset(0);
}

void AdventureState::update()
{
	m_Environment->update(m_Engine->input());
}

void AdventureState::draw() const
{
	m_Environment->draw();
}

void AdventureState::resume()
{
	
}

void AdventureState::pause()
{
	
}