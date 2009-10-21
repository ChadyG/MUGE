/*
   MUGE.h
   Mizzou Game Engine
 
   Created by Chad Godsey on 12/12/08.
  
  
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

#ifndef MUGECORE_H
#define MUGECORE_H

#include "../Global.h"

#include <stack>
#include <queue>
//#include "../Input/InputManager.hpp"
#include "../Sprite/SpriteManager.h"
#include "../Sprite/AnimationManager.h"
#include "../Sprite/ResourceManager.h"

class GameState;

/**
 * Entry point to the engine
 * manages states, and sends Gosu callbacks
 * to the current state
 **/
class MUGE : public Gosu::Window
{
public:
	/// This constructor is wrapped in main, parameters are given by main.json
	/// this provides us with a configurable window and clock
	MUGE(int _width=640, int _height=480, bool _fullscreen=false, double _updateInterval=20);


	/// Standard state switching, given state will replace current 
	/// state on top of stack
	void changeState( GameState *state );
	/// Switch to this state, but do not replace current state.
	/// Calls pause on topmost state.
	void pushState( GameState *state );
	/// Cleans up current state and removes it.
	/// Be sure to have states beneath or the game will exit!
	void popState();
	

	/// Game tick, called from Gosu and sends to current state.
	void update();
	/// Render callback from Gosu, sends to current state.
	void draw();
	/// Input event callback from Gosu, sent to current state.
	void buttonDown(Gosu::Button button);
	/// Input event callback from Gosu, sent to current state.
	void buttonUp(Gosu::Button button);
	

	//void hookIntoCommand(const std::string& command, const InputManager::CommandSignalType::slot_type& slot);
	//void setCurrentContext(const std::string& newContext);
	
	//void quitHandler();
	

	/// Returns dynamic FPS calculation
	int getFPS();

	/// Screen to World coordinate transform
	/// wrapper function
	b2Vec2 worldToScreen( b2Vec2 _world, Gosu::ZPos _layer );


	///Resource Management utilities
	///Create a context for resources
	ResourceManager* createResContext(std::string _context);

	void deleteResContext(std::string _context);


	/// Wrapper for Sprite constructor
	int createSprite(MUGE *_engine, std::wstring _filename, std::string _group, std::string _name);
	/// Get a sprite by the manager's ID
	boost::shared_ptr<Sprite> getSpriteByID( std::string _group, int _id );
	/// Get a Sprite by the filename (project relative)
	boost::shared_ptr<Sprite> getSpriteByName( std::string _group, std::string _name );
	/// Clean up resources within a certain context
	void deleteSpritesByGroup( std::string _group );

	/// Wrapper for Animation constructor
	int createAnimation(MUGE *_engine, std::wstring _filename, std::string _group, std::string _name, int _width, int _height, int _delay = 1);
	/// Get an Animation by manager's ID
	boost::shared_ptr<Animation> getAnimationByID( std::string _group, int _id );
	/// Get an Animation by name 
	boost::shared_ptr<Animation> getAnimationByName( std::string _group, std::string _name );
	/// Clean up resources within a certain context
	void deleteAnimationsByGroup( std::string _group );

private:
	//InputManager inputManager;
	std::stack< boost::shared_ptr<GameState> > m_States;
	std::queue< boost::shared_ptr<GameState> > m_NextStates;

	// Resource management
	// Divided into groups or contexts
	//std::map< std::string, SpriteManager> m_SpriteGroups;
	//std::map< std::string, AnimationManager> m_AnimGroups;
	std::map< std::string, ResourceManager> m_ResGroups;
	
	int m_curFPS;
	int m_curTicks;
	int m_lastSecond;
	
	bool m_stackDirty;

};

#endif