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
	: m_Count(0), m_Frame(0), m_Timer(0)
{
}

void Animation::init( Gosu::Graphics& graphics, std::wstring& fileName, int width, int height, int count)
{
	m_Count = count;
	Gosu::imagesFromTiledBitmap(graphics, fileName, width, height, false, m_Sprites);
	m_Frame = 0;
	m_Timer = m_Count;
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

void Animation::increment()
{
	m_Count--;
	if (m_Count <= 0) {
		m_Count = m_Timer;
		m_Frame++;
		if (m_Frame == m_Sprites.size())
			m_Frame = 0;
	}
	//return *(m_Sprites.at(m_Frame).get());
}