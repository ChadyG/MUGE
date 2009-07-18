/*
   Sprite.h
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
 
 	This class encapsulates drawing of a Gosu Image with an arbitrary 
 	position, rotation, scaling, and color modification
 
*/

#ifndef SPRITE_H
#define SPRITE_H

#include <Gosu/Gosu.hpp>
#include <map>

class Sprite
{
public:
	/**
	*	Image management functions
	*	used by Sprite to keep from duplicating Images
	*/
	static std::map< std::wstring, boost::shared_ptr<Gosu::Image> > theImageCache;
	static boost::shared_ptr<Gosu::Image> GetImage(Gosu::Graphics &graphics, std::wstring filename);
	
	Sprite();
	Sprite(Gosu::Graphics &_graphics, std::wstring _filename);
	
	void setImage(Gosu::Graphics &_graphics, std::wstring _filename);
	void setPosition( double _x, double _y);
	void setScaling(double _factorX, double _factorY);
	void setCenter(double _centerX, double _centerY);
	void setRotation(double _angle);
	void setColorMod(Gosu::Color _colorMod);
	void setWindowScale(double _scale);
	
	/**
	*	These values are offsets from position member data 
	*	this allows the sprite itself to keep track of local
	*	coordinates while being drawn into world space
	*
	*	Zoom requires telling the half width and height of the 
	*	screen in order to zoom from the center.
	*/
	void draw(double _x, double _y, Gosu::ZPos _layer) const;
	void drawZoom(double _x, double _y, Gosu::ZPos _layer, double _scale, double _zoom, int _scrWidth, int _scrHeight) const;
	void drawRot(double _x, double _y, Gosu::ZPos _layer, double _scale, double _zoom, double _angle, int _scrWidth, int _scrHeight) const;
	
private:
	boost::shared_ptr<Gosu::Image> m_Image;
	double m_rotation;
	// Local transformation
	double m_X;
	double m_Y;
	
	double m_WinScale;
	double m_factX;
	double m_factY;
	double m_centerX;
	double m_centerY;
	Gosu::Color m_ColorMod;
};


#endif