/*
 *  Animation.h
 *  Destructible
 *
 *  Created by Chad on 11/12/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 *	Animation encapsulates a set of images that are rotated through
 *	to give the illusion of movement.
 *	This can be used two ways;
 *		as a table of images with arbitrary access
 *		as an incrementing series of images with a time delay between frames
 *
 */
#ifndef ANIMATION_H
#define ANIMATION_H

#include <Gosu/Gosu.hpp>

typedef boost::shared_ptr<Gosu::Image> t_Image;

class Animation
{
public:
	Animation();
	void init(Gosu::Graphics&, std::wstring&, int width, int height, int delay=1);
	
	Gosu::Image& getFrame(unsigned frame);
	const Gosu::Image& getFrame(unsigned frame) const;
	
	Gosu::Image& getCurFrame();
	const Gosu::Image& getCurFrame() const;
	
	//Call this to increment frame based on time delay
	void update();

private:
	std::vector< t_Image > m_Sprites;
	
	unsigned int m_Frame;
	int m_numFrames;
	int m_Timer;
	int m_Delay;
};

#endif
