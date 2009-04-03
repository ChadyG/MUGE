/*
 *  TitleState.cpp
 *  Destructible
 *
 *  Created by Chad on 11/12/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */
 
#include "TitleState.h"
#include "MainMenuState.h"
#include "Engine/MUGE.h"

/**
*
*/

TitleState TitleState::m_StateInstance;

void TitleState::init(MUGE* _engine)
{
	m_Engine = _engine;
	std::wstring filename = Gosu::resourcePrefix() + L"Images/MUGD_Title_screen.png";
	m_TitleScreen.reset(new Gosu::Image(m_Engine->graphics(), filename, false));
	
	counter = 256;
	step = 0;
}

void TitleState::cleanup()
{
	m_TitleScreen.reset(0);
}

void TitleState::pause()
{

}

void TitleState::resume()
{

}

void TitleState::update()
{
	if (m_Engine->input().down(Gosu::kbReturn) || counter <= 0) {
		m_Engine->changeState(MainMenuState::instance());
	}
	--counter;
}

void TitleState::draw() const
{
	m_TitleScreen->draw(0,0,0, 1,1, Gosu::Color::fromHSV(0, 0, counter));
}
