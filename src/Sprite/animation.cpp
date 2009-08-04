/*
 *  Animation.cpp
 *  Destructible
 *
 *  Created by Chad on 11/12/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */
#include "Animation.h"
#include "../Scene/Scene.h"

Animation::Animation( )
: m_Delay(0), m_Frame(0), m_Timer(0), m_centerX(0.5), m_Rotation(0.0),
m_centerY(0.5), m_factX(1.0), m_factY(1.0), m_ColorMod(Gosu::Colors::white)
{
	m_Position.Set( 0.0, 0.0);
}

Animation::Animation( Gosu::Graphics& _graphics, std::wstring _fileName, int _width, int _height, int _delay)
: m_centerX(0.5), m_centerY(0.5), m_factX(1.0), m_factY(1.0), m_Rotation(0.0),
m_ColorMod(Gosu::Colors::white), m_Delay(_delay), m_Frame(0)
{
	m_Position.Set( 0.0, 0.0);
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

void Animation::setColorMod(Gosu::Color _colorMod)
{
	m_ColorMod = _colorMod;
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

void Animation::drawToScreen(double _x, double _y, Gosu::ZPos _layer, double _zoom, double _angle) const
{
	double angle = m_Rotation * (180.0/Gosu::pi);
	(m_Sprites.at(m_Frame).get())->drawRot(m_Position.x + _x, 
										   m_Position.y + _y, 
										   _layer, 
										   angle + _angle, 
										   m_centerX, 
										   m_centerY, 
										   m_factX*_zoom, 
										   m_factY*_zoom, 
										   m_ColorMod);
}

void Animation::drawFrameToScreen(int _frame, double _x, double _y, Gosu::ZPos _layer, double _zoom, double _angle) const
{
	
	double angle = m_Rotation * (180.0/Gosu::pi);
	(m_Sprites.at(_frame).get())->drawRot(m_Position.x + _x, 
										  m_Position.y + _y, 
										  _layer, 
										  angle + _angle,
										  m_centerX, 
										  m_centerY, 
										  m_factX*_zoom, 
										  m_factY*_zoom, 
										  m_ColorMod);
}

void Animation::draw(double _x, double _y, Gosu::ZPos _layer, double _zoom, double _angle) const
{
	b2Vec2 screenPos = m_Scene->worldToScreen( b2Vec2( m_Position.x - _x, m_Position.y - _y), _layer );
	double angle = m_Rotation * (180.0/Gosu::pi);
	(m_Sprites.at(m_Frame).get())->drawRot(screenPos.x, 
										   screenPos.y, 
										   _layer, 
										   angle + _angle, 
										   m_centerX, 
										   m_centerY, 
										   m_factX*_zoom, 
										   m_factY*_zoom, 
										   m_ColorMod);
}

void Animation::drawFrame(int _frame, double _x, double _y, Gosu::ZPos _layer, double _zoom, double _angle) const
{
	
	b2Vec2 screenPos = m_Scene->worldToScreen( b2Vec2( m_Position.x - _x, m_Position.y - _y), _layer );
	double angle = m_Rotation * (180.0/Gosu::pi);
	(m_Sprites.at(_frame).get())->drawRot(screenPos.x, 
										  screenPos.y, 
										  _layer, 
										  angle + _angle,
										  m_centerX, 
										  m_centerY, 
										  m_factX*_zoom, 
										  m_factY*_zoom, 
										  m_ColorMod);
}