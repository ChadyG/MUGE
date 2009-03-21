/*
 *  AdventureState.h
 *  Destructible
 *
 *  Created by Chad on 3/9/09.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */

#include <Gosu/Gosu.hpp>
#include "Engine/GameState.h"
#include "Engine/Environment.h"
#include "Engine/CutScene.h"

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
		boost::scoped_ptr< CutScene > m_CutScene;
		
		static AdventureState m_StateInstance;
		
	protected:
		AdventureState() { }
		
	public:
		void init(Gosu::Graphics &graphics);
		void cleanup();
		
		void pause();
		void resume();
		
		void update(const Gosu::Input &, MUGE *);
		void draw() const;
		
		static AdventureState* instance() {
			return &m_StateInstance;
		}
		
		void save();
	};