/*
 AnimationManager.cpp
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

#include "AnimationManager.h"

AnimationManager::AnimationManager()
: m_curID(0)
{
}

boost::shared_ptr<Animation> AnimationManager::createAnimation(Gosu::Graphics &_graphics, std::wstring _filename, int _width, int _height, int _delay)
{
	//if (m_AnimationMap[_filename])
	//	return m_Animations[ m_AnimationMap[ _filename ] ];
	m_Animations[m_curID].reset( new Animation( _graphics, _filename, _width, _height, _delay) );
	m_AnimationMap[ _filename ] = m_curID;
	return m_Animations[ m_curID++ ];
}

boost::shared_ptr<Animation> AnimationManager::getAnimationByID( int _id )
{
	return m_Animations[_id];
}

boost::shared_ptr<Animation> AnimationManager::getAnimationByName( std::wstring _filename )
{
	return m_Animations[ m_AnimationMap[ _filename ] ];
}