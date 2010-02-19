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
#include <list>
#include "SpriteSheet.h"
#include "Sprite.h"

/**
*	Intended services 
*		* simple resource managing for Gosu::Image in Sprites
*		* state based rendering
*			- persistent rendering based on given values
*		* clean separation of world to screen transformation
*			- all given values are in world coordinates
*			- scene manager tells camera orientation + position for rendering
*/
class RenderManager
{
public:
	RenderManager() : m_curImageID(0) {}
	
	void setScreen( int _w, int _h, int _s)
	{
		m_screenW = _w; m_screenH = _h; m_screenScale = _s;
	};

	void setCamera( float _x, float _y, float _zoom = 1.f, float _rot = 0.f )
	{
		m_camX = _x; m_camY = _y, m_camZoom = _zoom; m_camRot = _rot;
	};

	void updateSpriteSheets();

	void doRender() const;

	void setLayerScale(int _layer, float _scale) { m_LayerScales[_layer] = _scale; }

	/// create a sprite
	Sprite* createSprite(int _layer, std::wstring _filename);
	/// create a SpriteSheet
	SpriteSheet* createSpriteSheet(
		int _layer, std::wstring _filename, int _width, int _height, int _delay = 1);

	/// register a sprite
	void registerSprite(int _layer, Sprite* _sp);
	/// register a SpriteSheet
	void registerSpriteSheet(int _layer, SpriteSheet* _ss);

	/// Static accessor to current render manager
	static boost::shared_ptr< RenderManager > getCurrentContext() { return s_CurrentContext; }

	/// Static setter for current render manager
	static void setCurrentContext(RenderManager* _context) { s_CurrentContext.reset(_context); }

protected:
	static boost::shared_ptr< RenderManager > s_CurrentContext;

	
	std::map< int, Gosu::Image* > m_Images;
	std::map< std::wstring, int > m_ImageMap;

	std::map< int, float > m_LayerScales;

	std::list< SpriteSheet > m_SpriteSheets;
	std::list< Sprite > m_Sprites;
	
	int m_curImageID;

	float m_camX, m_camY, m_camZoom, m_camRot;
	int m_screenW, m_screenH, m_screenScale;
};


#endif

