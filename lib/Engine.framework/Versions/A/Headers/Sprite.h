/*
   Sprite.h
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

/*
 	This class encapsulates operations on a Gosu Image with a state
	for arbitrary position, rotation, scaling, and color modifications
 
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
	*
	*	TODO: move these into some sort of image manager? (owned by engine)
	*/
	Sprite();
	Sprite(Gosu::Image* _image);
	
	void setImage(Gosu::Image* _image);
	
	void setScaling(double _factorX, double _factorY);
	void setCenter(double _centerX, double _centerY);
	void setColorMod(Gosu::Color _colorMod);
	void setAlphaMode(Gosu::AlphaMode _amode) { m_Alpha = _amode; }

	double posX() const { return m_posX; }
	double posY() const { return m_posY; }
	double zoom() const { return m_zoom; }
	double angle() const { return m_angle; }
	int layer() const { return m_layer; }
	bool visible() const { return m_visible; }

	void setX(double _x) { m_posX = _x; }
	void setY(double _y) { m_posY = _y; }
	void setZoom(double _zoom) { m_zoom = _zoom; }
	void setAngle(double _angle) { m_angle = _angle; }
	void setLayer(double _layer) { m_layer = (int)_layer; }
	void setVisible(bool _vis) { m_visible = _vis; }
	
	/**
	*	These values are offsets from position member data 
	*	this allows the sprite itself to keep track of local
	*	coordinates while being drawn into world space
	*
	*	Zoom requires telling the half width and height of the 
	*	screen in order to zoom from the center.
	*/
	void draw(double _x, double _y, double _zoom = 1.0, double _angle = 0.0) const;
	
private:
	Gosu::Image* m_Image;

	double m_posX, m_posY, m_zoom, m_angle;
	int m_layer;
	
	bool m_visible;
	//zoom factors
	double m_factX;
	double m_factY;
	//rotation center
	double m_centerX;
	double m_centerY;
	//color modifier
	Gosu::Color m_ColorMod;
	Gosu::AlphaMode m_Alpha;
};


#endif