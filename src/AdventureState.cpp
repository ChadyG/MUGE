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

void AdventureState::init(Gosu::Graphics &graphics)
{	
	m_Environment.reset( new Environment(Gosu::resourcePrefix() + L"Data/test_level.json", graphics));
	
}

void AdventureState::cleanup()
{
	m_Environment.reset(0);
}

void AdventureState::update(const Gosu::Input &input, MUGE *Engine)
{
	m_Environment->update(input);
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