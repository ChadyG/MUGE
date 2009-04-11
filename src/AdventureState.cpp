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
	m_Environment.reset( new Environment(Gosu::resourcePrefix() + L"Data/level_v2.json", m_Engine));
}

void AdventureState::cleanup()
{
	m_Environment.reset(0);
}

void AdventureState::update()
{
	m_Environment->update();
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