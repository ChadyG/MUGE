/*
   AdventureState.h
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

#include <Gosu/Gosu.hpp>
#include "Core/GameState.h"
#include "Scene/Environment.h"

class MUGE;

/**
 * State for Adventure gameplay
 * 
 * Must be able to load from a save, store to a save, and start from scratch.
 * If just starting, show opening sequence then load first level.
 * While playing, is able to swap out levels as player progresses.
 * Pause menu control can go here as well.  Allow player to save, quit and likely
 * open up some options to control.
 */
class AdventureState : public GameState
{
	boost::scoped_ptr< Environment > m_Environment;
	
	static AdventureState m_StateInstance;
	
protected:
	AdventureState() { }
	
public:
	void init( MUGE* );
	void cleanup();
	
	void pause();
	void resume();
	
	void update();
	void draw() const;
	
	static AdventureState* instance() {
		return &m_StateInstance;
	}
	
	void save();
};