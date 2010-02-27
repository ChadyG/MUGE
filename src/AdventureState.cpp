/*
   AdventureState.cpp
   My Unnamed Game Engine
 
   Created by Chad Godsey on 3/9/09.
  
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

#include "AdventureState.h"
#include "Core/Core.h"
#include "Input/JSONFile.hpp"
#include "Graphics/Sprite.h"
#include "Graphics/SpriteSheet.h"
#include "Graphics/RenderManager.h"

/**
 *
 */

AdventureState::AdventureState( std::wstring _config )
{
	m_ConfigFile = _config;
}

void AdventureState::init()
{	
	m_Engine = Core::getCurrentContext();
	
	//Let's set up the player!
	JSONFile jFile(Gosu::narrow(Gosu::resourcePrefix() + L"Data/" + m_ConfigFile + L".json"));
	json::grammar<char>::array::const_iterator it, it2;
	json::grammar<char>::array arr, arr2;
	json::grammar<char>::object o;
	int i, j;
	std::string tString;
	
	m_Scene.reset( new Scene(Gosu::widen( jFile.get< std::string >("Levels[0]") )));
	m_Scene->registerManagers();

	RenderManager* rendMan = RenderManager::getCurrentContext();

	arr = jFile.get<json::grammar<char>::array>("PlayerAnimations");
	for (i = 0, it = arr.begin(); it != arr.end(); ++it, ++i) {
		SpriteSheet *anim = rendMan->createSpriteSheet( 3,
			Gosu::resourcePrefix() + L"Images/" + Gosu::widen(jFile.get< std::string >("FileName", *it)), 
			jFile.get< int >("Width", *it),
			jFile.get< int >("Height", *it),
			jFile.get< int >("Duration", *it));
		m_Player.addSpriteSheet( jFile.get< std::string >("Name", *it), anim );
	}
	
	m_Scene->tellPlayer( &m_Player );
}

void AdventureState::cleanup()
{
	// here is where level progress saving would occur
	m_Scene.reset(0);
}

void AdventureState::update()
{
	// will be more complicated later (level switching, maybe just have a callback to switch levels)
	m_Scene->update();
}

void AdventureState::draw() const
{
	m_Scene->draw();
}

void AdventureState::resume()
{
	
}

void AdventureState::pause()
{
	
}