/*
 *  Animation.h
 *  Destructible
 *
 *  Created by Chad on 11/12/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */
#ifndef ANIMATION_H
#define ANIMATION_H

#include <Gosu/Gosu.hpp>

typedef boost::shared_ptr<Gosu::Image> t_Image;
/**
* The camera takes care of following player,
* identifying what needs to be drawn, and
* triggering level zones (onEnterCamera or so)
* which we can use for spawn nodes
*/

class Animation
{
	std::vector< t_Image > m_Sprites;
	
	unsigned int m_Frame;
	int m_numFrames;
	int m_Timer;
	int m_Count;

public:
	Animation(Gosu::Graphics&, std::wstring&, int width, int height, int count=1);
	
	Gosu::Image& getFrame(unsigned frame);
	Gosu::Image& operator->();
};

#endif
