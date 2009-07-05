/*
 *  Sprite.cpp
 *  Destructible
 *
 *  Created by Chad on 4/9/09.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 */
#include "Sprite.h"

std::map< std::wstring, boost::shared_ptr<Gosu::Image> > Sprite::theImageCache;

boost::shared_ptr<Gosu::Image> Sprite::GetImage(Gosu::Graphics &graphics, std::wstring filename)
{
	std::map< std::wstring, boost::shared_ptr<Gosu::Image> >::iterator it = theImageCache.find(filename);
	if (it != theImageCache.end()) {
		return (it->second);
	}
	boost::shared_ptr<Gosu::Image> newImage( new Gosu::Image(graphics, filename) );
	theImageCache[filename] = newImage;
	return theImageCache[filename];
}

Sprite::Sprite()
	: m_X(0.0), m_Y(0.0), m_Z(0.0), m_rotation(0.0), m_centerX(0.5), m_centerY(0.5), m_factX(1.0), m_factY(1.0), m_ColorMod(Gosu::Colors::white)
{
}

Sprite::Sprite(Gosu::Graphics &_graphics, std::wstring _filename)
	: m_X(0.0), m_Y(0.0), m_Z(0.0), m_rotation(0.0), m_centerX(0.5), m_centerY(0.5), m_factX(1.0), m_factY(1.0), m_ColorMod(Gosu::Colors::white)
{
	m_Image = Sprite::GetImage(_graphics, _filename);
}

//----------Setters----------

void Sprite::setImage(Gosu::Graphics &_graphics, std::wstring _filename)
{
	m_Image = Sprite::GetImage(_graphics, _filename);
}

void Sprite::setPosition( double _x, double _y, Gosu::ZPos _z)
{
	m_X = _x;
	m_Y = _y;
	m_Z = _z;
}

void Sprite::setScaling(double _factorX, double _factorY)
{
	m_factX = _factorX;
	m_factY = _factorY;
}

void Sprite::setCenter(double _centerX, double _centerY)
{
	m_centerX = _centerX;
	m_centerY = _centerY;
}

void Sprite::setRotation(double _angle)
{
	m_rotation = _angle;
}

void Sprite::setColorMod(Gosu::Color _colorMod)
{
	m_ColorMod = _colorMod;
}

void Sprite::setWindowScale(double _scale)
{
	m_WinScale = _scale;
}

//----------Operations----------

void Sprite::draw(double _x, double _y) const
{
	m_Image->drawRot( m_X * m_WinScale + _x, m_Y * m_WinScale + _y, m_Z, m_rotation, m_centerX, m_centerY, m_factX, m_factY, m_ColorMod);
}

void Sprite::drawZoom(double _x, double _y, double _scale, double _zoom, int _scrWidth, int _scrHeight) const
{
	m_Image->drawRot( ((m_X - _x) * m_WinScale * (1.0/_scale)) * _zoom + _scrWidth, 
					 ((m_Y - _y) * m_WinScale * (1.0/_scale)) * _zoom + _scrHeight, 
					 m_Z, m_rotation, m_centerX, m_centerY, m_factX*_zoom, m_factY*_zoom, m_ColorMod);
}