/*
 SpriteManager.cpp
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

#include "SpriteManager.h"

SpriteManager::SpriteManager()
: m_curID(0)
{
}

boost::shared_ptr<Sprite> SpriteManager::createSprite(Gosu::Graphics &_graphics, std::wstring _filename)
{
	//if (m_SpriteMap[_filename])
	//	return m_Sprites[ m_SpriteMap[ _filename ] ];
	m_Sprites[m_curID].reset( new Sprite( _graphics, _filename) );
	m_SpriteMap[ _filename ] = m_curID;
	return m_Sprites[ m_curID++ ];
}

boost::shared_ptr<Sprite> SpriteManager::getSpriteByID( int _id )
{
	return m_Sprites[_id];
}

boost::shared_ptr<Sprite> SpriteManager::getSpriteByName( std::wstring _filename )
{
	return m_Sprites[ m_SpriteMap[ _filename ] ];
}