/*
   TitleState.cpp
   Mizzou Game Engine
 
   Created by Chad Godsey on 11/12/08.
  
 Copyright 2009 Mizzou Game Design.

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
 */
 
// remove this when lua is working
#include "TitleState.h"
#include "MainMenuState.h"

#include "Core/MUGE.h"
#include "Input/JSONFile.hpp"

/**
*
*/

TitleState::TitleState( std::wstring _config )
{
	m_ConfigFile = _config;
}

void TitleState::init(MUGE* _engine)
{
	m_Engine = _engine;

	JSONFile jFile(Gosu::narrow(Gosu::resourcePrefix() + L"Data/" + m_ConfigFile + L".json"));
	json::grammar<char>::array::const_iterator it;
	json::grammar<char>::array arr;
	std::wstring filename;

	arr = jFile.get<json::grammar<char>::array>("Screens");
	for ( it = arr.begin(); it != arr.end(); ++it) {
		TitleScreen tScreen;
		filename = Gosu::resourcePrefix() + L"Images/" + Gosu::widen(jFile.get< std::string >("Image", *it));

		tScreen.Image.reset(new Gosu::Image(m_Engine->graphics(), filename, false));
		tScreen.Duration = jFile.get< double >("Duration", *it) * m_Engine->getFPS();
		tScreen.Decay = jFile.get< double >("DecayTime", *it) * m_Engine->getFPS();
		tScreen.FadeTo = Gosu::Color( 
			jFile.get< int >("FadeTo[0]", *it),
			jFile.get< int >("FadeTo[1]", *it),
			jFile.get< int >("FadeTo[2]", *it));
		
		m_Titles.push_back( tScreen );
	}
	m_counter = 0;
	m_fading = false;
}

void TitleState::cleanup()
{
	m_Titles.clear();
}

void TitleState::pause()
{

}

void TitleState::resume()
{

}

void TitleState::update()
{
	++m_counter;
	if (m_fading) {
		if (m_counter >= m_Titles.front().Duration + m_Titles.front().Decay)
			m_Titles.pop_front();
	}else{
		if (m_counter >= m_Titles.front().Duration)
			m_fading = true;
	}

	// Temporary state switch, to go into Lua
	if (m_Engine->input().down(Gosu::kbReturn) || m_Titles.empty()) {
		MainMenuState *state = new MainMenuState( std::wstring(L"MainMenuState") );
		m_Engine->changeState( state );
	}
}

void TitleState::draw() const
{
	if (!m_Titles.empty()) {
		if (m_fading)
			m_Titles.front().Image->draw(0,0,0, 1,1, 
				Gosu::interpolate( 
					Gosu::Colors::white, 
					m_Titles.front().FadeTo, 
					(m_counter - m_Titles.front().Duration) / (double)(m_Titles.front().Decay) ));
		else
			m_Titles.front().Image->draw(0,0,0);
	}
}

b2Vec2 TitleState::worldToScreen( b2Vec2 _world, Gosu::ZPos _layer )
{
	return _world;
}