/*
   RenderManager.h
   My Unnamed Game Engine
 
   Created by Chad Godsey on 12/9/09.
  
 Copyright 2009 BlitThis! studios.

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

#ifndef RENMAN_H
#define RENMAN_H

#include <Gosu/Gosu.hpp>
#include <map>
#include "SpriteSheet.h"
#include "Sprite.h"

/**
*	Provide lazy-loading
*	Allow resource sharing
*/
class RenderManager
{
public:
	RenderManager() {}
	
	void registerCamera( Camera *_cam );

	void updateSpriteSheets();

	void doRender();

	/// create a sprite
	int createSprite(std::wstring _filename, std::string _name);
	/// create an SpriteSheet
	int createSpriteSheet(std::wstring _filename, std::string _name, int _width, int _height, int _delay = 1);
		
	/// Get a sprite by ID
	boost::shared_ptr<Sprite> getSpriteByID( int _id );
	/// Get an SpriteSheet by ID
	boost::shared_ptr<SpriteSheet> getSpriteSheetByID( int _id );

	/// Get a Sprite by name
	int getSpriteByName( std::string _name );
	/// Get an SpriteSheet by name 
	int getSpriteSheetByName( std::string _name );

	/// draw operations for sprite
	void drawSprite(int _id, double _x, double _y, Gosu::ZPos _layer, double _zoom = 1.0, double _angle = 0.0);
	void drawSpriteToScreen(int _id, double _x, double _y, Gosu::ZPos _layer, double _zoom = 1.0, double _angle = 0.0);
	/// draw operations for SpriteSheet
	void drawSheet(int _id, double _x, double _y, Gosu::ZPos _layer, double _zoom = 1.0, double _angle = 0.0);
	void drawSheetToScreen(int _id, double _x, double _y, Gosu::ZPos _layer, double _zoom = 1.0, double _angle = 0.0);
	void drawSheetFrame(int _id, int _frame, double _x, double _y, Gosu::ZPos _layer, double _zoom = 1.0, double _angle = 0.0);
	void drawSheetFrameToScreen(int _id, int _frame, double _x, double _y, Gosu::ZPos _layer, double _zoom = 1.0, double _angle = 0.0);


	/// Static accessor to current render manager
	static boost::shared_ptr< RenderManager > getCurrentContext() { return s_CurrentContext; }

	/// Static setter for current render manager
	static void setCurrentContext(RenderManager* _context) { s_CurrentContext.reset(_context); }

protected:

	static boost::shared_ptr< RenderManager > s_CurrentContext;

	std::map< int, boost::shared_ptr<SpriteSheet> > m_SpriteSheets;
	std::map< std::string, int > m_SpriteSheetMap;
	
	std::map< int, boost::shared_ptr<Sprite> > m_Sprites;
	std::map< std::string, int > m_SpriteMap;
	
	int m_curSpriteID;
	int m_curSheetID;
};


#endif