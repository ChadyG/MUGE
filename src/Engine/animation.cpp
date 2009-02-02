/*
 *  Animation.cpp
 *  Destructible
 *
 *  Created by Chad on 11/12/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */
#include "Animation.h"

Animation::Animation( Gosu::Graphics& graphics, std::wstring& fileName, int width, int height, int count)
	: m_Count(count)
{
	Gosu::imagesFromTiledBitmap(graphics, fileName, width, height, false, m_Sprites);
	m_Frame = 0;
	m_Timer = m_Count;
}

// I think this works...
Gosu::Image& Animation::operator->()
{
	m_Count--;
	if (m_Count <= 0) {
		m_Count = m_Timer;
		m_Frame++;
		if (m_Frame == m_Sprites.size())
			m_Frame = 0;
	}
	return *(m_Sprites.at(m_Frame).get());
}