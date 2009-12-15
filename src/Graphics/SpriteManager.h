/*
 SpriteManager.h
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

#include <Gosu/Gosu.hpp>
#include <map>

#include "Sprite.h"

class SpriteManager
{
public:
	SpriteManager();
	
	/// Wrapper for Sprite constructor
	boost::shared_ptr<Sprite> createSprite(Gosu::Graphics &_graphics, std::wstring _filename, std::string _name);
	
	/// Get a sprite by the manager's ID
	boost::shared_ptr<Sprite> getSpriteByID( int _id );
	
	/// Get a Sprite by the filename (project relative)
	boost::shared_ptr<Sprite> getSpriteByName( std::string _name );
	
	
private:
	std::map< int, boost::shared_ptr<Sprite> > m_Sprites;
	std::map< std::string, int > m_SpriteMap;
	
	int m_curID;
};