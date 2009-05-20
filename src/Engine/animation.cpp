/*
 *  Animation.cpp
 *  Destructible
 *
 *  Created by Chad on 11/12/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */
#include "Animation.h"

Animation::Animation( )
	: m_Delay(0), m_Frame(0), m_Timer(0)
{
}

void Animation::init( Gosu::Graphics& graphics, std::wstring& fileName, int width, int height, int delay)
{
	m_Delay = delay;
	Gosu::imagesFromTiledBitmap(graphics, fileName, width, height, false, m_Sprites);
	m_Frame = 0;
	m_Timer = m_Delay;
}

Gosu::Image& Animation::getFrame(unsigned frame)
{
	return *(m_Sprites.at(frame).get());
};

const Gosu::Image& Animation::getFrame(unsigned frame) const
{
	return *(m_Sprites.at(frame).get());
};

Gosu::Image& Animation::getCurFrame()
{
	return *(m_Sprites.at(m_Frame).get());
};

const Gosu::Image& Animation::getCurFrame() const
{
	return *(m_Sprites.at(m_Frame).get());
};

void Animation::update()
{
	m_Timer--;
	if (m_Timer <= 0) {
		m_Timer = m_Delay;
		m_Frame++;
		if (m_Frame == m_Sprites.size())
			m_Frame = 0;
	}
}