/*
   Sprite.cpp
   Mizzou Game Engine
 
   Created by Chad Godsey on 4/9/09.
  
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
 
#include "Sprite.h"
#include "Scene.h"

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
	: m_X(0.0), m_Y(0.0), m_rotation(0.0), m_centerX(0.5), m_centerY(0.5), 
	m_factX(1.0), m_factY(1.0), m_ColorMod(Gosu::Colors::white)
{
}

Sprite::Sprite(Gosu::Graphics &_graphics, std::wstring _filename)
	: m_X(0.0), m_Y(0.0), m_rotation(0.0), m_centerX(0.5), m_centerY(0.5), 
	m_factX(1.0), m_factY(1.0), m_ColorMod(Gosu::Colors::white)
{
	m_Image = Sprite::GetImage(_graphics, _filename);
}

//----------Setters----------

void Sprite::setImage(Gosu::Graphics &_graphics, std::wstring _filename)
{
	m_Image = Sprite::GetImage(_graphics, _filename);
}

void Sprite::setPosition( double _x, double _y)
{
	m_X = _x;
	m_Y = _y;
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

void Sprite::draw(double _x, double _y, Gosu::ZPos _layer) const
{
	m_Image->drawRot( m_X * m_WinScale + _x, 
					 m_Y * m_WinScale + _y, 
					 _layer, 
					 m_rotation, 
					 m_centerX, 
					 m_centerY, 
					 m_factX, 
					 m_factY, 
					 m_ColorMod);
}

void Sprite::drawZoom(double _x, double _y, Gosu::ZPos _layer, double _scale, 
					  double _zoom, int _scrWidth, int _scrHeight) const
{
	m_Image->drawRot( ((m_X - _x) * m_WinScale * _scale) * _zoom + _scrWidth, 
					 ((m_Y - _y) * m_WinScale * _scale) * _zoom + _scrHeight, 
					 _layer, 
					 m_rotation, 
					 m_centerX, 
					 m_centerY, 
					 m_factX*_zoom, 
					 m_factY*_zoom, 
					 m_ColorMod);
}

void Sprite::drawRot(double _x, double _y, Gosu::ZPos _layer, double _scale, 
					 double _zoom, double _angle, int _scrWidth, int _scrHeight) const
{
	double nX = ((m_X - _x) * m_WinScale * _scale) * _zoom, 
			nY = ((m_Y - _y) * m_WinScale * _scale) * _zoom;
	double dist = Gosu::distance( 0.0, 0.0, nX, nY );
	double ang = Gosu::angle( 0.0, 0.0, nX, nY );
	double rotX = Gosu::offsetX( ang + _angle, dist );
	double rotY = Gosu::offsetY( ang + _angle, dist );
	m_Image->drawRot( rotX + _scrWidth, 
					 rotY + _scrHeight, 
					 _layer, 
					 m_rotation + _angle, 
					 m_centerX, 
					 m_centerY, 
					 m_factX*_zoom, 
					 m_factY*_zoom, 
					 m_ColorMod);
}