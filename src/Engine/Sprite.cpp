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

Sprite::Sprite(Gosu::Graphics &graphics, std::wstring filename)
	: m_X(0.0), m_Y(0.0), m_Z(0.0), m_rotation(0.0), m_centerX(0.5), m_centerY(0.5), m_factX(1.0), m_factY(1.0), m_ColorMod(Gosu::Colors::white)
{
	m_Image = Sprite::GetImage(graphics, filename);
}

void Sprite::setImage(Gosu::Graphics &graphics, std::wstring filename)
{
	m_Image = Sprite::GetImage(graphics, filename);
}

void Sprite::setPosition( double x, double y, Gosu::ZPos z)
{
	m_X = x;
	m_Y = y;
	m_Z = z;
}

void Sprite::setScaling(double factorX, double factorY)
{
	m_factX = factorX;
	m_factY = factorY;
}

void Sprite::setCenter(double centerX, double centerY)
{
	m_centerX = centerX;
	m_centerY = centerY;
}

void Sprite::setRotation(double angle)
{
	m_rotation = angle;
}

void Sprite::setColorMod(Gosu::Color colorMod)
{
	m_ColorMod = colorMod;
}


void Sprite::draw(double x, double y, double z) const
{
	m_Image->drawRot( m_X + x, m_Y + y, m_Z + z, m_rotation, m_centerX, m_centerY, m_factX, m_factY, m_ColorMod);
}