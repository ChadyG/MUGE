/*
	GameState.h
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
#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "../Global.h"
class MUGE;

/**
* Abstract class to describe a game state
* all logic will go into these classes
*
*	Three types:
*		Menu
*		Play
*		Choreograph
*/
class GameState
{
public:
	/// Base class handles focus and dirty flags (used in engine for delayed state switching)
	GameState() : m_hasFocus(false), m_isDirty(false) { }
	
	/// Called when added to state stack in Engine
	/// Try to do most initialization here instead of constructor
	virtual void init( MUGE* _engine ) = 0;
	
	/// Called when being removed from state stack.
	/// Good place to cleanup all data and write to disk.
	virtual void cleanup() = 0;
	
	
	/// Called when being pushed down the stack (loss of focus).
	/// Try to minimize memory footprint here.
	virtual void pause() = 0;
	
	/// Called when reclaiming focus in state stack.
	/// If minimizing memory footprint, reclaim here.
	virtual void resume() = 0;
	
	
	/// Game tick update callback
	virtual void update() = 0;
	
	/// Game render callback
	virtual void draw() const = 0;

	/// Screen to World coordinate transform
	/// wrapper function
	virtual b2Vec2 worldToScreen( b2Vec2 _world, Gosu::ZPos _layer ) = 0;
	
	
	void setFocus(bool _focus) { m_hasFocus = _focus; };
	bool inFocus() const { return m_hasFocus; };
	void setDirty() { m_isDirty = true; };
	bool dirty() const { return m_isDirty; };
	
protected:
	
	MUGE* m_Engine;
	
private:
	bool m_hasFocus;
	bool m_isDirty;
	
};

#endif