/*
   TitleState.cpp
   My Unnamed Game Engine
 
   Created by Chad Godsey on 11/12/08.
  
 Copyright 2009 BlitThis! studios.

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
 
#include "TitleState.h"
#include "MainMenuState.h"
#include "Core/Core.h"
#include "Input/JSONFile.hpp"

/**
*  Set up pluggable factory
*/

titleState_maker titleState_maker::s_sRegisteredMaker;

GameState* titleState_maker::makeState(std::wstring _config)
{
	return new TitleState(_config);
}

/**
*
*/

TitleState::TitleState( std::wstring _config )
{
	m_ConfigFile = _config;
}

void TitleState::init()
{
	m_Engine = Core::getCurrentContext();

	JSONFile jFile(Gosu::narrow(Gosu::resourcePrefix() + L"Data/" + m_ConfigFile));
	json::grammar<char>::array::const_iterator it, it2;
	json::grammar<char>::array arr, arr2;
	int i, j;
	std::string tString;
	std::wstring filename;

	arr = jFile.get<json::grammar<char>::array>("Screens");
	for (i = 0, it = arr.begin(); it != arr.end(); ++it, ++i) {
		tString = jFile.get<std::string>("Image", *it);
		filename = Gosu::resourcePrefix() + L"Images/" + Gosu::widen(tString);
		screen ts;
		ts.image = new Gosu::Image(m_Engine->graphics(), filename, false);
		ts.duration = jFile.get<double>("Duration", *it) * 50;
		ts.decayTime = jFile.get<double>("DecayTime", *it) * 50;
		ts.fadeTo = Gosu::Color(jFile.get<int>("FadeTo[0]", *it),
			jFile.get<int>("FadeTo[1]", *it),
			jFile.get<int>("FadeTo[2]", *it));

		m_Screens.push_back( ts );
	}
	
	m_curScreen = m_Screens.begin();
	counter = m_curScreen->duration;
	fade = false;
}

void TitleState::cleanup()
{
	m_Screens.clear();
}

void TitleState::pause()
{

}

void TitleState::resume()
{

}

void TitleState::update()
{
	if (m_Engine->input().down(Gosu::kbReturn)) {
		MainMenuState *state = new MainMenuState( std::wstring(L"MainMenuState") );
		m_Engine->changeState( state );
	}
	if (counter <= 0) {
		if (fade) {
			m_curScreen++;
			if (m_curScreen == m_Screens.end()) {
				MainMenuState *state = new MainMenuState( std::wstring(L"MainMenuState") );
				m_Engine->changeState( state );
				return;
			}
			counter = m_curScreen->duration;
			fade = false;
		}else{
			counter = m_curScreen->decayTime;
			fade = true;
		}
	}
	--counter;
}

void TitleState::draw() const
{
	if (m_curScreen != m_Screens.end()) {
		if (fade) {
			m_curScreen->image->draw(0,0,0, 1,1, 
				Gosu::interpolate(m_curScreen->fadeTo, Gosu::Colors::white, ((double)counter)/m_curScreen->decayTime));
		}else
			m_curScreen->image->draw(0,0,0, 1,1);
	}
}
