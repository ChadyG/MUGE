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

#include <Gosu/Gosu.hpp>
#include "RenderManager.h"
#include <Box2D.h>
#include "../Core/Core.h"

RenderManager* RenderManager::s_CurrentContext;

//save for later to do precaching of memory
RenderManager::RenderManager() 
	: m_curImageID(0),
	m_MessageCorners(Gosu::resourcePrefix() + L"Images/Message_corners.png", 16, 16, 50),
	m_MessageTip(Core::getCurrentContext()->graphics(), Gosu::resourcePrefix() + L"Images/Message_tip.png"),
	m_Font(Core::getCurrentContext()->graphics(), Gosu::defaultFontName(), 12, 0)
{	
	m_Camera = new Camera();
}

RenderManager::~RenderManager()
{
	std::map< std::wstring, Gosu::Image* >::iterator it;
	for (it = m_Images.begin(); it != m_Images.end(); ++it) {
		delete it->second;
	}
}

void RenderManager::updateSpriteSheets()
{
	std::list<SpriteSheet>::iterator isheet;
	for (isheet = m_SpriteSheets.begin(); isheet != m_SpriteSheets.end(); isheet++) {
		isheet->update();
	}
}

void RenderManager::doRender() const
{
	int camW = m_Camera->Width(), camH = m_Camera->Height();
	//m_camX = _x; m_camY = _y, 
	double camZoom = m_Camera->Zoom(), camRot = m_Camera->Rotation(), camScale = m_Camera->Scale();

	std::map< int, float >::const_iterator iscale;
	std::list<Sprite>::const_iterator isprite;
	for (isprite = m_Sprites.begin(); isprite != m_Sprites.end(); isprite++) {
		if (isprite->visible()) {
			/*
			iscale = m_LayerScales.find(isprite->layer());
			scale = 1.0f/iscale->second;
			zoom = 1.0f + scale * (m_camZoom - 1.0f);

			b2Vec2 newPos((isprite->posX() - m_camX) * scale * m_screenScale * zoom, 
						  (isprite->posY() - m_camY) * scale * m_screenScale * zoom);
			b2Transform trans;
			trans.R.Set( m_camRot*((float)Gosu::pi/180.0f) );
			trans.position = b2Mul( trans.R, newPos);
			trans.position.Set( trans.position.x + m_screenW/2, trans.position.y + m_screenH/2);

			*/
			CameraTransform trans = m_Camera->worldToScreen( isprite->posX(), isprite->posY(), isprite->layer());
			isprite->draw(trans.x, trans.y, trans.zoom, trans.rot);
		}
	}
	
	std::list<SpriteSheet>::const_iterator isheet;
	for (isheet = m_SpriteSheets.begin(); isheet != m_SpriteSheets.end(); isheet++) {
		if (isheet->visible()) {
			/*
			iscale = m_LayerScales.find(isheet->layer());
			scale = 1.0/iscale->second;
			zoom = 1.0 + scale * (m_camZoom - 1.0);

			b2Vec2 newPos((isheet->posX() - m_camX) * scale * m_screenScale * zoom, 
						  (isheet->posY() - m_camY) * scale * m_screenScale * zoom);
			b2Transform trans;
			trans.R.Set( m_camRot*(Gosu::pi/180.0) );
			trans.position = b2Mul( trans.R, newPos);
			trans.position.Set( trans.position.x + m_screenW/2, trans.position.y + m_screenH/2);
			*/
			CameraTransform trans = m_Camera->worldToScreen( isheet->posX(), isheet->posY(), isheet->layer());
			isheet->draw(trans.x, trans.y, trans.zoom, trans.rot);
		}
	}

	Core* core = Core::getCurrentContext();

	//Draw messages
	bool flip;
	int x, y;
	unsigned int width, height;
	//Gosu::Image *text;

	std::list<MessageBubble>::const_iterator ibubble;
	for (ibubble = m_Messages.begin(); ibubble != m_Messages.end(); ibubble++) {
		/*
		b2Vec2 newPos((ibubble->x - m_camX) * scale * m_screenScale * zoom, 
						(ibubble->y - m_camY) * scale * m_screenScale * zoom);
		b2Transform trans;
		trans.R.Set( m_camRot*(Gosu::pi/180.0) );
		trans.position = b2Mul( trans.R, newPos);
		trans.position.Set( trans.position.x + m_screenW/2 + 4, trans.position.y + m_screenH/2 - 4);
		*/
		CameraTransform trans = m_Camera->worldToScreen( ibubble->x, ibubble->y, 0);
		

		width = ibubble->width;
		height = ibubble->height;
		if (!ibubble->isStatic) {
			width = (int)m_Font.textWidth(ibubble->Message);
		}

		flip = false;
		if (trans.y > (height + 48)) {
			trans.y = trans.y - height - 24;
			flip = true;
		}else
			trans.y += 24; 

		x = Gosu::clamp((int)trans.x - 2*(int)width/3, 8, camW - (int)width - 8);
		y = Gosu::clamp((int)trans.y, 24, camH - (int)height - 24);

		//Draw background top middle to bottom middle
		core->graphics().drawQuad( 
			x, y - 8, Gosu::Colors::white, 
			x + width, y - 8, Gosu::Colors::white,
			x + width, y + height + 8, Gosu::Colors::white,
			x, y + height + 8, Gosu::Colors::white, 19);
		//Draw background left middle to right middle
		core->graphics().drawQuad( 
			x - 8, y, Gosu::Colors::white, 
			x + width + 8, y, Gosu::Colors::white,
			x + width + 8, y + height, Gosu::Colors::white,
			x - 8, y + height, Gosu::Colors::white, 19);
#if _WINDOWS
		//Draw border top, bottom
		core->graphics().drawLine(
			x , y - 8, Gosu::Colors::black, 
			x + width, y - 8, Gosu::Colors::black, 19);
		core->graphics().drawLine(
			x + width, y + height + 7, Gosu::Colors::black,
			x , y + height + 7, Gosu::Colors::black, 19);
	
		//Draw border left, right
		core->graphics().drawLine(
			x - 7, y, Gosu::Colors::black, 
			x - 7, y + height, Gosu::Colors::black, 19);
		core->graphics().drawLine(
			x + width + 8, y, Gosu::Colors::black,
			x + width + 8, y + height, Gosu::Colors::black, 19);
#else
		//Draw border top, bottom
		core->graphics().drawLine(
			x , y - 7, Gosu::Colors::black, 
			x + width, y - 7, Gosu::Colors::black, 19);
		core->graphics().drawLine(
			x + width, y + height + 8, Gosu::Colors::black,
			x , y + height + 8, Gosu::Colors::black, 19);
	
		//Draw border left, right
		core->graphics().drawLine(
			x - 8, y, Gosu::Colors::black, 
			x - 8, y + height, Gosu::Colors::black, 19);
		core->graphics().drawLine(
			x + width + 7, y, Gosu::Colors::black,
			x + width + 7, y + height, Gosu::Colors::black, 19);
#endif
		//Draw corners
		m_MessageCorners.drawFrameAt(0, x, y, 19);
		m_MessageCorners.drawFrameAt(1, x + width, y, 19);
		m_MessageCorners.drawFrameAt(3, x + width, y + height, 19);
		m_MessageCorners.drawFrameAt(2, x, y + height, 19);

		//Draw message tip
		m_MessageTip.draw(Gosu::clamp( (double)(int)trans.x, 16.0, camW - 32.0), 
			Gosu::clamp((double)(int)trans.y + (flip ? height + 7.0 : -7.0), 17.0, camH - 17.0), 
			19, 1.0, flip ? 1.0: -1.0);

		if (ibubble->isStatic)
			ibubble->text->draw(x, y, 19, 1.0, 1.0, Gosu::Colors::black);
		else
			m_Font.draw(ibubble->Message, x, y, 19, 1.0, 1.0, Gosu::Colors::black);
	}
}

//
//	Standard creators

MessageBubble* RenderManager::createMessage(std::wstring _message, double _x, double _y, bool _static)
{
	MessageBubble nMB;
	nMB.isStatic = _static;
	nMB.Message = _message;
	nMB.x = _x;
	nMB.y = _y;

	unsigned int width = Gosu::textWidth(nMB.Message, Gosu::defaultFontName(), 12);
	nMB.width = (std::min)((unsigned)256, Gosu::textWidth(nMB.Message, Gosu::defaultFontName(), 12));
	nMB.text = new Gosu::Image(
		Core::getCurrentContext()->graphics(), Gosu::createText(
			nMB.Message, 
			Gosu::defaultFontName(), 
			12, 
			1, 
			nMB.width, 
			Gosu::taCenter));
	nMB.height = nMB.text->height();

	m_Messages.push_back(nMB);
	return &(m_Messages.back());
}

Sprite* RenderManager::createSprite(int _layer, std::wstring _filename)
{
	//m_Sprites[m_CurSpriteID] = new Sprite();
	Sprite tsprite;
	if (m_Images.find(_filename) == m_Images.end()) {
		m_Images[_filename] = new Gosu::Image(Core::getCurrentContext()->graphics(),_filename);
	}
	tsprite.setImage(m_Images[_filename]);
	tsprite.setLayer((float)_layer);
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

//
// Asset registration

void RenderManager::registerSprite(int _layer, Sprite* _sp)
{
	m_Sprites.push_back(*_sp);
}

void RenderManager::registerSpriteSheet(int _layer, SpriteSheet* _ss)
{
	m_SpriteSheets.push_back(*_ss);
}

//
// Asset deletion

void RenderManager::deleteSprite(Sprite* _sprite) 
{ 
	std::list<Sprite>::iterator sit = m_Sprites.begin();
	while (sit != m_Sprites.end()) {
		if (&(*sit) == _sprite) {
			sit = m_Sprites.erase(sit);
			continue;
		}
		sit++;
	}
}
void RenderManager::deleteSpriteSheet(SpriteSheet* _sheet) 
{ 
	std::list<SpriteSheet>::iterator sit = m_SpriteSheets.begin();
	while (sit != m_SpriteSheets.end()) {
		if (&(*sit) == _sheet) {
			sit = m_SpriteSheets.erase(sit);
			continue;
		}
		sit++;
	}
}
void RenderManager::deleteMessage(MessageBubble* _message)
{
	std::list<MessageBubble>::iterator sit = m_Messages.begin();
	while (sit != m_Messages.end()) {
		if (&(*sit) == _message) {
			sit = m_Messages.erase(sit);
			continue;
		}
		sit++;
	}
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