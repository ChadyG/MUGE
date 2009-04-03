/*
 *  MUGE.cpp
 *  Destructible
 *
 *  Created by Chad on 12/12/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */
#include "MUGE.h"
#include "GameState.h"
#include <boost/bind.hpp>

/**
* Entry point to the engine
* manages states, and sends Gosu callbacks
* to the current state
*/
MUGE::MUGE()
	: Gosu::Window(768, 480, false, 20)
{
     inputManager.hookIntoCommand("Menu.Quit:Down", boost::bind(&MUGE::quitHandler, this));
     inputManager.setCurrentContext("Menu");
}

void MUGE::buttonDown(Gosu::Button button)
{
     inputManager.buttonDownHandler(button);
}

void MUGE::buttonUp(Gosu::Button button)
{
     inputManager.buttonUpHandler(button);
}

void MUGE::changeState( GameState *state )
{
	if (!m_States.empty()) {
		m_States.top()->cleanup();
		m_States.pop();
	}
	m_States.push(state);
	state->init(this);
}

void MUGE::pushState( GameState *state )
{
	m_States.push(state);
	state->init(this);
}

void MUGE::popState()
{
	m_States.pop();
	// Remove this for release (if desired)
	if (m_States.empty()) 
		close();
}

void MUGE::update()
{
	if (!m_States.empty())
		m_States.top()->update();
}

void MUGE::draw()
{
	if (!m_States.empty())
		m_States.top()->draw();
}

void MUGE::hookIntoCommand(const std::string& command, const InputManager::CommandSignalType::slot_type& slot)
{
	inputManager.hookIntoCommand(command, slot);
}

void MUGE::setCurrentContext(const std::string& newContext)
{
	inputManager.setCurrentContext(newContext);
}

void MUGE::quitHandler()
{
     close();
}
