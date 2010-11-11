/*
   MessageBubble.cpp
   My Unnamed Game Engine
 
   Created by Chad Godsey on 11/10/10.
  
 Copyright 2010 BlitThis! studios.

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


#include "MessageBubble.h"
#include "../Core/Core.h"
#include "SpriteSheet.h"
#include "Sprite.h"
#include "Camera.h"

SpriteSheet* MessageBubble::s_MessageCorners = 0;
Gosu::Image* MessageBubble::s_MessageTip = 0;

MessageBubble::MessageBubble()
: m_message(L""), m_isStatic(false), m_visible(true)
{
	int width = (std::min)((unsigned)256, Gosu::textWidth(m_message, Gosu::defaultFontName(), 12));
	m_text = new Gosu::Image(
		Core::getCurrentContext()->graphics(), Gosu::createText(
			m_message, 
			Gosu::defaultFontName(), 
			12, 
			1, 
			width, 
			Gosu::taCenter));
	m_height = m_text->height();

	if (s_MessageCorners == 0) {
		s_MessageCorners = new SpriteSheet(Gosu::resourcePrefix() + L"Images/Message_corners.png", 16, 16, 50);
		s_MessageTip = new Gosu::Image(Core::getCurrentContext()->graphics(), Gosu::resourcePrefix() + L"Images/Message_tip.png");
	}
}

MessageBubble::MessageBubble(std::wstring _message, bool _static)
: m_message(_message), m_isStatic(_static), m_visible(true)
{
	int width = (std::min)((unsigned)256, Gosu::textWidth(m_message, Gosu::defaultFontName(), 12));
	m_text = new Gosu::Image(
		Core::getCurrentContext()->graphics(), Gosu::createText(
			m_message, 
			Gosu::defaultFontName(), 
			12, 
			1, 
			width, 
			Gosu::taCenter));
	m_height = m_text->height();
	
	if (s_MessageCorners == 0) {
		s_MessageCorners = new SpriteSheet(Gosu::resourcePrefix() + L"Images/Message_corners.png", 16, 16, 50);
		s_MessageTip = new Gosu::Image(Core::getCurrentContext()->graphics(), Gosu::resourcePrefix() + L"Images/Message_tip.png");
	}
}


//----------Operations----------

void MessageBubble::draw(double _x, double _y, 
					 double _zoom, double _angle) const
{
	int width, height;
	double x, y;
	RenderManager *rman = RenderManager::getCurrentContext();
	int camW = rman->camera().Width(), camH = rman->camera().Height();

	if (m_visible) {
		if (!m_isStatic) {
			width = (int)rman->font().textWidth(m_message);
		}

		bool flip = false;
		if (_y > (height + 48)) {
			_y = _y - height - 24;
			flip = true;
		}else
			_y += 24; 

		x = Gosu::clamp((int)_x - 2*(int)width/3, 8, camW - (int)width - 8);
		y = Gosu::clamp((int)_y, 24, camH - (int)height - 24);

		Core* core = Core::getCurrentContext();

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
		s_MessageCorners->drawFrameAt(0, x, y, 19);
		s_MessageCorners->drawFrameAt(1, x + width, y, 19);
		s_MessageCorners->drawFrameAt(3, x + width, y + height, 19);
		s_MessageCorners->drawFrameAt(2, x, y + height, 19);

		//Draw message tip
		s_MessageTip->draw(Gosu::clamp( (double)(int)_x, 16.0, camW - 32.0), 
			Gosu::clamp((double)(int)_y + (flip ? height + 7.0 : -7.0), 17.0, camH - 17.0), 
			19, 1.0, flip ? 1.0: -1.0);

		if (m_isStatic)
			m_text->draw(x, y, 19, 1.0, 1.0, Gosu::Colors::black);
		else
			rman->font().draw(m_message, x, y, 19, 1.0, 1.0, Gosu::Colors::black);
	}
}