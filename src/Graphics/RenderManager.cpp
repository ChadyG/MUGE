/*
   RenderManager.cpp
   My Unnamed Game Engine
 
   Created by Chad Godsey on 12/23/09.
  
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

#include "RenderManager.h"
#include <Box2D.h>
#include "../Core/Core.h"

RenderManager* RenderManager::s_CurrentContext;

//save for later to do precaching of memory
//RenderManager::RenderManager() {}

void RenderManager::updateSpriteSheets()
{
	std::list<SpriteSheet>::iterator isheet;
	for (isheet = m_SpriteSheets.begin(); isheet != m_SpriteSheets.end(); isheet++) {
		isheet->update();
	}
}

void RenderManager::doRender() const
{
	float scale, zoom;

	std::map< int, float >::const_iterator iscale;
	std::list<Sprite>::const_iterator isprite;
	for (isprite = m_Sprites.begin(); isprite != m_Sprites.end(); isprite++) {
		if (isprite->visible()) {
			iscale = m_LayerScales.find(isprite->layer());
			scale = 1.0/iscale->second;
			zoom = 1.0 + scale * (m_camZoom - 1.0);

			b2Vec2 newPos((isprite->posX() - m_camX) * scale * m_screenScale * zoom, 
						  (isprite->posY() - m_camY) * scale * m_screenScale * zoom);
			b2XForm trans;
			trans.R.Set( m_camRot*(Gosu::pi/180.0) );
			trans.position = b2Mul( trans.R, newPos);
			trans.position.Set( trans.position.x + m_screenW/2, trans.position.y + m_screenH/2);

			isprite->draw(trans.position.x, trans.position.y, zoom, m_camRot);
		}
	}
	
	std::list<SpriteSheet>::const_iterator isheet;
	for (isheet = m_SpriteSheets.begin(); isheet != m_SpriteSheets.end(); isheet++) {
		if (isheet->visible()) {
			iscale = m_LayerScales.find(isheet->layer());
			scale = 1.0/iscale->second;
			zoom = 1.0 + scale * (m_camZoom - 1.0);

			b2Vec2 newPos((isheet->posX() - m_camX) * scale * m_screenScale * zoom, 
						  (isheet->posY() - m_camY) * scale * m_screenScale * zoom);
			b2XForm trans;
			trans.R.Set( m_camRot*(Gosu::pi/180.0) );
			trans.position = b2Mul( trans.R, newPos);
			trans.position.Set( trans.position.x + m_screenW/2, trans.position.y + m_screenH/2);

			isheet->draw(trans.position.x, trans.position.y, zoom, m_camRot);
		}
	}
}


Sprite* RenderManager::createSprite(int _layer, std::wstring _filename)
{
	//m_Sprites[m_CurSpriteID] = new Sprite();
	Sprite tsprite;
	if (m_ImageMap.find(_filename) != m_ImageMap.end()) {
		m_Images[m_curImageID] = new Gosu::Image(Core::getCurrentContext()->graphics(),_filename);
		m_ImageMap[_filename] = m_curImageID++;
	}
	tsprite.setImage(m_Images[m_ImageMap[_filename]]);
	tsprite.setLayer(_layer);
	m_Sprites.push_back(tsprite);
	return &(m_Sprites.back());
}

SpriteSheet* RenderManager::createSpriteSheet(
	int _layer, std::wstring _filename, int _width, int _height, int _delay)
{
	SpriteSheet isheet( _filename, _width, _height, _delay );
	isheet.setLayer(_layer);
	m_SpriteSheets.push_back(isheet);
	return &(m_SpriteSheets.back());
}

void RenderManager::registerSprite(int _layer, Sprite* _sp)
{
	m_Sprites.push_back(*_sp);
}

void RenderManager::registerSpriteSheet(int _layer, SpriteSheet* _ss)
{
	m_SpriteSheets.push_back(*_ss);
}

/*
double scale = 1.0/m_LayerScales[_layer];
double zoom = 1.0 + scale * (m_Zoom - 1.0);
b2Vec2 newPos((_x * m_Scale * scale) * zoom, 
			  (_y * m_Scale * scale) * zoom);
b2XForm trans;
trans.R.Set( m_Rot*(Gosu::pi/180.0) );
trans.position = b2Mul( trans.R, newPos);
trans.position.Set( trans.position.x + m_Width/2, trans.position.y + m_Height/2);
return trans;

std::map< std::wstring, boost::shared_ptr<Gosu::Image> >::iterator it = theImageCache.find(filename);
if (it != theImageCache.end()) {
	return (it->second);
}
boost::shared_ptr<Gosu::Image> newImage( new Gosu::Image(Core::getCurrentContext()->graphics(), filename) );
theImageCache[filename] = newImage;
return theImageCache[filename];
*/