/*
 *  Animation.cpp
 *  Destructible
 *
 *  Created by Chad on 11/12/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
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

void Animation::draw(double _x, double _y) const
{
	(m_Sprites.at(m_Frame).get())->drawRot( m_X * m_WinScale + _x, m_Y * m_WinScale + _y, m_Z, m_rotation, m_centerX, m_centerY, m_factX, m_factY, m_ColorMod);
}

void Animation::drawFrameAt(int _frame, double _x, double _y) const
{
	(m_Sprites.at(_frame).get())->drawRot( m_X * m_WinScale + _x, m_Y * m_WinScale + _y, m_Z, m_rotation, m_centerX, m_centerY, m_factX, m_factY, m_ColorMod);
}

void Animation::drawZoom(double _x, double _y, double _scale, double _zoom, int _scrWidth, int _scrHeight) const
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