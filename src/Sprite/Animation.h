/*
 *  Animation.h
 *  Destructible
 *
 *  Created by Chad on 11/12/08.
 *  Copyright 2009 Mizzou Game Design. All rights reserved.
 *
 *	Animation encapsulates a set of images that are cycled through
 *	to give the illusion of movement.
 *	This can be used two ways;
 *		as a table of images with arbitrary access
 *		as an incrementing series of images with a time delay between frames
 *
 */
#ifndef ANIMATION_H
#define ANIMATION_H

#include <Gosu/Gosu.hpp>

typedef boost::shared_ptr<Gosu::Image> t_Image;

class Animation
{
public:
	Animation();
	Animation(Gosu::Graphics &_graphics, std::wstring _filename, int width, int height, int delay=1);
	
	void setImage(Gosu::Graphics &_graphics, std::wstring _filename, int width, int height, int delay=1);
	
	void setPosition( double _x, double _y);
	void setScaling(double _factorX, double _factorY);
	void setCenter(double _centerX, double _centerY);
	void setRotation(double _angle);
	void setColorMod(Gosu::Color _colorMod);
	void setWindowScale(double _scale);
	
	Gosu::Image& getFrame(unsigned frame);
	const Gosu::Image& getFrame(unsigned frame) const;
	
	Gosu::Image& getCurFrame();
	const Gosu::Image& getCurFrame() const;
	
	//Call this to increment frame based on time delay
	void update();
	
	void draw(double _x, double _y, Gosu::ZPos _layer) const;
	void drawFrameAt(int _frame, double _x, double _y, Gosu::ZPos _layer) const;
	
	void drawZoom(double _x, double _y, Gosu::ZPos _layer, double _scale, double _zoom, int _scrWidth, int _scrHeight) const;
	void drawFrameAtZoom(int _frame, double _x, double _y, Gosu::ZPos _layer, double _scale, double _zoom, int _scrWidth, int _scrHeight) const;
	
	void drawRot(double _x, double _y, Gosu::ZPos _layer, double _scale, double _zoom, double _angle, int _scrWidth, int _scrHeight) const;
	void drawFrameAtRot(int _frame, double _x, double _y, Gosu::ZPos _layer, double _scale, double _zoom, double _angle, int _scrWidth, int _scrHeight) const;

private:
	std::vector< t_Image > m_Sprites;
	
	unsigned int m_Frame;
	int m_numFrames;
	int m_Timer;
	int m_Delay;
	
	// Local transformation
	double m_rotation;
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
