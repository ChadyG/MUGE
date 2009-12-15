/*
	SpriteSheet.cpp
	My Unnamed Game Engine
 
	Created by Chad on 11/12/08.
	Copyright 2009 BlitThis! studios. All rights reserved.

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
#include "../Core/Core.h"
#include "SpriteSheet.h"
#include "../Scene/Scene.h"

SpriteSheet::SpriteSheet( )
: m_Delay(0), m_Frame(0), m_Timer(0), m_centerX(0.5), m_Rotation(0.0),
m_centerY(0.5), m_factX(1.0), m_factY(1.0), m_ColorMod(Gosu::Colors::white)
{
	m_Position.Set( 0.0, 0.0);
}

SpriteSheet::SpriteSheet( std::wstring _fileName, int _width, int _height, int _delay)
: m_centerX(0.5), m_centerY(0.5), m_factX(1.0), m_factY(1.0), m_Rotation(0.0),
m_ColorMod(Gosu::Colors::white), m_Delay(_delay), m_Frame(0)
{
	m_Position.Set( 0.0, 0.0);
	Gosu::imagesFromTiledBitmap(Core::getCurrentContext()->graphics(), _fileName, _width, _height, false, m_Sprites);
	m_Timer = m_Delay;
}

//----------Setters----------

void SpriteSheet::registerTransMod( TransMod *_tmod )
{
	m_TransMod = _tmod;
}

void SpriteSheet::setImage( std::wstring _fileName, int _width, int _height, int _delay)
{
	m_Delay = _delay;
	Gosu::imagesFromTiledBitmap(Core::getCurrentContext()->graphics(), _fileName, _width, _height, false, m_Sprites);
	m_Frame = 0;
	m_Timer = m_Delay;
}

void SpriteSheet::setScaling(double _factorX, double _factorY)
{
	m_factX = _factorX;
	m_factY = _factorY;
}

void SpriteSheet::setCenter(double _centerX, double _centerY)
{
	m_centerX = _centerX;
	m_centerY = _centerY;
}

void SpriteSheet::setColorMod(Gosu::Color _colorMod)
{
	m_ColorMod = _colorMod;
}

//----------Getters----------

Gosu::Image& SpriteSheet::getFrame(unsigned frame)
{
	return *(m_Sprites.at(frame).get());
};

const Gosu::Image& SpriteSheet::getFrame(unsigned frame) const
{
	return *(m_Sprites.at(frame).get());
};

Gosu::Image& SpriteSheet::getCurFrame()
{
	return *(m_Sprites.at(m_Frame).get());
};

const Gosu::Image& SpriteSheet::getCurFrame() const
{
	return *(m_Sprites.at(m_Frame).get());
};

void SpriteSheet::update()
{
	m_Timer--;
	if (m_Timer <= 0) {
		m_Timer = m_Delay;
		m_Frame++;
		if (m_Frame == m_Sprites.size())
			m_Frame = 0;
	}
}

//----------Operations----------

void SpriteSheet::draw(double _x, double _y, Gosu::ZPos _layer, double _zoom, double _angle) const
{
	(m_Sprites.at(m_Frame).get())->drawRot(_x, 
										   _y, 
										   _layer, 
										   _angle, 
										   m_centerX, 
										   m_centerY, 
										   m_factX*_zoom, 
										   m_factY*_zoom, 
										   m_ColorMod);
}

void SpriteSheet::drawFrame(int _frame, double _x, double _y, Gosu::ZPos _layer, double _zoom, double _angle) const
{
	(m_Sprites.at(_frame).get())->drawRot(_x, 
										  _y, 
										  _layer, 
										  _angle,
										  m_centerX, 
										  m_centerY, 
										  m_factX*_zoom, 
										  m_factY*_zoom, 
										  m_ColorMod);
}