/*
 *  GameState.h
 *  Destructible
 *
 *  Created by Chad on 11/12/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */
#ifndef GAMESTATE_H
#define GAMESTATE_H

class MUGE;

/**
* Abstract class to describe a game state
* all logic will go into these classes
* they use the singleton paradigm so simplify
* use
*/
class GameState
{
protected:
	GameState() { }
	
	MUGE* m_Engine;

public:
	virtual void init( MUGE* ) = 0;
	virtual void cleanup() = 0;
	
	virtual void pause() = 0;
	virtual void resume() = 0;
	
	virtual void update() = 0;
	virtual void draw() const = 0;
	
};

#endif