/*
   Sprite.cpp
   My Unnamed Game Engine
 
   Created by Chad Godsey on 4/9/09.
  
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
 
#include "Sprite.h"
#include "../Scene/Scene.h"
#include "../Core/Core.h"

std::map< std::wstring, boost::shared_ptr<Gosu::Image> > Sprite::theImageCache;

boost::shared_ptr<Gosu::Image> Sprite::GetImage(std::wstring filename)
{
	std::map< std::wstring, boost::shared_ptr<Gosu::Image> >::iterator it = theImageCache.find(filename);
	if (it != theImageCache.end()) {
		return (it->second);
	}
	boost::shared_ptr<Gosu::Image> newImage( new Gosu::Image(Core::getCurrentContext()->graphics(), filename) );
	theImageCache[filename] = newImage;
	return theImageCache[filename];
}

Sprite::Sprite()
	: m_centerX(0.5), m_centerY(0.5), m_Rotation(0.0),
	m_factX(1.0), m_factY(1.0), m_ColorMod(Gosu::Colors::white)
{
	m_Position.Set( 0.0, 0.0);
}

Sprite::Sprite(std::wstring _filename)
	: m_centerX(0.5), m_centerY(0.5), m_Rotation(0.0),
	m_factX(1.0), m_factY(1.0), m_ColorMod(Gosu::Colors::white)
{
	m_Position.Set( 0.0, 0.0);
	m_Image = Sprite::GetImage(_filename);
}

//----------Setters----------

void Sprite::registerTransMod( TransMod *_tmod )
{
	m_TransMod = _tmod;
}

void Sprite::setImage(std::wstring _filename)
{
	m_Image = Sprite::GetImage(_filename);
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

void Sprite::setColorMod(Gosu::Color _colorMod)
{
	m_ColorMod = _colorMod;
}

//----------Operations----------

void Sprite::draw(double _x, double _y, Gosu::ZPos _layer, 
					 double _zoom, double _angle) const
{
	m_Image->drawRot( _x, 
					 _y, 
					 _layer, 
					 _angle, 
					 m_centerX, 
					 m_centerY, 
					 m_factX*_zoom, 
					 m_factY*_zoom, 
					 m_ColorMod);
}