/*
 *  MUGE.h
 *  Destructible
 *
 *  Created by Chad on 12/12/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */
#include <Gosu/Gosu.hpp>
#include <stack>
#include <queue>
#include "../Input/InputManager.hpp"

class GameState;

/**
 * Entry point to the engine
 * manages states, and sends Gosu callbacks
 * to the current state
 **/
class MUGE : public Gosu::Window
{
	InputManager inputManager;
	std::stack< GameState* > m_States;
	std::queue< GameState* > m_NextStates;
	
	int m_curFPS;
	int m_curTicks;
	int m_lastSecond;
	
	bool m_stackDirty;

public:
	MUGE(int _width=640, int _height=480, bool _fullscreen=false, double _updateInterval=20);

	void changeState( GameState *state );
	void pushState( GameState *state );
	void popState();
	void update();
	void draw();

	void buttonDown(Gosu::Button button);
	void buttonUp(Gosu::Button button);
     
	void hookIntoCommand(const std::string& command, const InputManager::CommandSignalType::slot_type& slot);
	void setCurrentContext(const std::string& newContext);

	void quitHandler();
	
	int getFPS();
};
