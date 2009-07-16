/*
   AdventureState.cpp
   Mizzou Game Engine
 
   Created by Chad Godsey on 3/9/09.
  
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

#include "AdventureState.h"
#include "Core/MUGE.h"

/**
 *
 */

AdventureState::AdventureState( std::wstring _config )
{
	m_ConfigFile = _config;
}

void AdventureState::init( MUGE* _engine)
{	
	m_Engine = _engine;
	JSONFile jFile(Gosu::narrow(Gosu::resourcePrefix() + L"Data/" + m_ConfigFile + L".json"));
	json::grammar<char>::array::const_iterator it, it2;
	json::grammar<char>::array arr, arr2;
	json::grammar<char>::object o;
	int i, j;
	std::string tString;
	
	arr = jFile.get<json::grammar<char>::array>("PlayerAnimations");
	for (i = 0, it = arr.begin(); it != arr.end(); ++it, ++i) {
		Animation *anim = new Animation();
		anim->setImage( m_Engine->graphics(), 
			Gosu::resourcePrefix() + L"Images/" + Gosu::widen(jFile.get< std::string >("FileName", *it)), 
			jFile.get< int >("Width", *it),
			jFile.get< int >("Height", *it),
			jFile.get< int >("Duration", *it));
		m_Player.addAnimation( jFile.get< std::string >("Name", *it), anim );
	}
	
	m_Environment.reset( new Environment(m_Engine, Gosu::widen( jFile.get< std::string >("Levels[0]") )));
	m_Environment->tellPlayer( &m_Player );
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