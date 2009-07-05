/*
 *  AdventureState.cpp
 *  Destructible
 *
 *  Created by Chad on 3/9/09.
 *  Copyright 2009 University of Missouri Columbia. All rights reserved.
 *
 */

#include "AdventureState.h"
#include "Core/MUGE.h"

/**
 *
 */

AdventureState AdventureState::m_StateInstance;

void AdventureState::init( MUGE* _engine)
{	
	m_Engine = _engine;
	m_Environment.reset( new Environment(L"level_v3", m_Engine));// later the name will be given via configuration
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