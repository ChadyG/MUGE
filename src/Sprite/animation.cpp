/*
   Animation.cpp
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
 
#include "Animation.h"

Animation::Animation( )
: m_Delay(0), m_Frame(0), m_Timer(0), m_X(0.0), 
m_Y(0.0), m_Z(0.0), m_rotation(0.0), m_centerX(0.5), 
m_centerY(0.5), m_factX(1.0), m_factY(1.0), m_ColorMod(Gosu::Colors::white)
{
}

Animation::Animation( Gosu::Graphics& _graphics, std::wstring _fileName, int _width, int _height, int _delay)
: m_X(0.0), m_Y(0.0), m_Z(0.0), m_rotation(0.0), 
m_centerX(0.5), m_centerY(0.5), m_factX(1.0), m_factY(1.0), 
m_ColorMod(Gosu::Colors::white), m_Delay(_delay), m_Frame(0)
{
	Gosu::imagesFromTiledBitmap(_graphics, _fileName, _width, _height, false, m_Sprites);
	m_Timer = m_Delay;
}

//----------Setters----------

void Animation::setImage(Gosu::Graphics &_graphics, std::wstring _fileName, int _width, int _height, int _delay)
{
	m_Delay = _delay;
	Gosu::imagesFromTiledBitmap(_graphics, _fileName, _width, _height, false, m_Sprites);
	m_Frame = 0;
	m_Timer = m_Delay;
}

void Animation::setPosition( double _x, double _y, Gosu::ZPos _z)
{
	m_X = _x;
	m_Y = _y;
	m_Z = _z;
}

void Animation::setScaling(double _factorX, double _factorY)
{
	m_factX = _factorX;
	m_factY = _factorY;
}

void Animation::setCenter(double _centerX, double _centerY)
{
	m_centerX = _centerX;
	m_centerY = _centerY;
}

void Animation::setRotation(double _angle)
{
	m_rotation = _angle;
}

void Animation::setColorMod(Gosu::Color _colorMod)
{
	m_ColorMod = _colorMod;
}

void Animation::setWindowScale(double _scale)
{
	m_WinScale = _scale;
}

//----------Getters----------

Gosu::Image& Animation::getFrame(unsigned frame)
{
	return *(m_Sprites.at(frame).get());
};

const Gosu::Image& Animation::getFrame(unsigned frame) const
{
	return *(m_Sprites.at(frame).get());
};

Gosu::Image& Animation::getCurFrame()
{
	return *(m_Sprites.at(m_Frame).get());
};

const Gosu::Image& Animation::getCurFrame() const
{
	return *(m_Sprites.at(m_Frame).get());
};

void Animation::update()
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

void Animation::drawCurFrame(double _x, double _y) const
{
	(m_Sprites.at(m_Frame).get())->drawRot( m_X * m_WinScale + _x, m_Y * m_WinScale + _y, m_Z, m_rotation, m_centerX, m_centerY, m_factX, m_factY, m_ColorMod);
}

void Animation::drawFrameAt(int _frame, double _x, double _y) const
{
	(m_Sprites.at(_frame).get())->drawRot( m_X * m_WinScale + _x, m_Y * m_WinScale + _y, m_Z, m_rotation, m_centerX, m_centerY, m_factX, m_factY, m_ColorMod);
}

void Animation::drawCurFrameZoom(double _x, double _y, double _scale, double _zoom, int _scrWidth, int _scrHeight) const
{
	(m_Sprites.at(m_Frame).get())->drawRot(((m_X - _x) * m_WinScale * _scale) * _zoom + _scrWidth, 
										   ((m_Y - _y) * m_WinScale * _scale) * _zoom + _scrHeight, 
										   m_Z, m_rotation, m_centerX, m_centerY, m_factX*_zoom, m_factY*_zoom, m_ColorMod);
}

void Animation::drawFrameAtZoom(int _frame, double _x, double _y, double _scale, double _zoom, int _scrWidth, int _scrHeight) const
{
	(m_Sprites.at(_frame).get())->drawRot(((m_X - _x) * m_WinScale * _scale) * _zoom + _scrWidth, 
										  ((m_Y - _y) * m_WinScale * _scale) * _zoom + _scrHeight, 
										  m_Z, m_rotation, m_centerX, m_centerY, m_factX*_zoom, m_factY*_zoom, m_ColorMod);
}