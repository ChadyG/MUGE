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
	Sprite(Gosu::Graphics &graphics, std::wstring filename);
	
	void setImage(Gosu::Graphics &graphics, std::wstring filename);
	void setPosition( double x, double y, Gosu::ZPos z);
	void setScaling(double factorX, double factorY);
	void setCenter(double centerX, double centerY);
	void setRotation(double angle);
	void setColorMod(Gosu::Color colorMod);
	
	/**
	*	These values are offsets from position member data 
	*	this allows the sprite itself to keep track of local
	*	coordinates while being drawn into world space
	*/
	void draw(double x, double y, double z) const;
	
private:
	boost::shared_ptr<Gosu::Image> m_Image;
	double m_rotation;
	// Local transformation
	double m_X;
	double m_Y;
	Gosu::ZPos m_Z;
	
	double m_factX;
	double m_factY;
	double m_centerX;
	double m_centerY;
	Gosu::Color m_ColorMod;
};


#endif