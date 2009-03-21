/*
 *  CutScene.h
 *  Destructible
 *
 *  Created by Chad on 3/9/09.
 *  Copyright 2009 University of Missouri Columbia. All rights reserved.
 *
 */

#include <Gosu/Gosu.hpp>
#include "Animation.h"

/**
 * Basically a series of screens with music
 * let player skip through certain portions or whole
 * for us, just create child classes of particular cutscenes
 * we can worry about how to generalize this in some other project
 */
class CutScene
{
	boost::scoped_ptr< Animation > m_Screens;
	boost::scoped_ptr< Gosu::Song > m_Song;
public:
	CutScene(std::string descriptionFile);
	
	void update();
	void draw() const;
};