/*
 *  Sprite.h
 *  Destructible
 *
 *  Created by Chad on 4/9/09.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 *	This class encapsulates drawing of a Gosu Image with an arbitrary 
 *	position, rotation, scaling, and color modification
 *
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
	void setPosition( double _x, double _y, Gosu::ZPos _z);
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
	void draw(double _x, double _y) const;
	void drawZoom(double _x, double _y, double _scale, double _zoom, int _scrWidth, int _scrHeight) const;
	
private:
	boost::shared_ptr<Gosu::Image> m_Image;
	double m_rotation;
	// Local transformation
	double m_X;
	double m_Y;
	Gosu::ZPos m_Z;
	
	double m_WinScale;
	double m_factX;
	double m_factY;
	double m_centerX;
	double m_centerY;
	Gosu::Color m_ColorMod;
};


#endif