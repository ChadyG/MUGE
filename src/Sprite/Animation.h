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
#include <Box2D.h>
#include "../Scene/SceneObject.h"

typedef boost::shared_ptr<Gosu::Image> t_Image;

class Animation : public SceneObject
{
public:
	Animation();
	Animation(Gosu::Graphics &_graphics, std::wstring _filename, int width, int height, int delay=1);
	
	unsigned getID() { return m_ID; }
	void setID( unsigned _id) { m_ID = _id; }
	
	void setImage(Gosu::Graphics &_graphics, std::wstring _filename, int width, int height, int delay=1);
	
	/// setRotation - sets world Rotation
	void setRotation( double _angle ) { m_Rotation = _angle; }
	/// setPosition - sets world position
	void setPosition( b2Vec2 _pos ) { m_Position = _pos; }
	
	void setScaling(double _factorX, double _factorY);
	void setCenter(double _centerX, double _centerY);
	void setColorMod(Gosu::Color _colorMod);
	
	Gosu::Image& getFrame(unsigned frame);
	const Gosu::Image& getFrame(unsigned frame) const;
	
	Gosu::Image& getCurFrame();
	const Gosu::Image& getCurFrame() const;
	
	//Call this to increment frame based on time delay
	void update();
	
	void drawToScreen(double _x, double _y, Gosu::ZPos _layer, double _zoom = 1.0, double _angle = 0.0) const;
	void drawFrameToScreen(int _frame, double _x, double _y, Gosu::ZPos _layer, double _zoom = 1.0, double _angle = 0.0) const;
	
	void draw(double _x, double _y, Gosu::ZPos _layer, double _zoom = 1.0, double _angle = 0.0) const;
	void drawFrame(int _frame, double _x, double _y, Gosu::ZPos _layer, double _zoom = 1.0, double _angle = 0.0) const;

private:
	std::vector< t_Image > m_Sprites;
	
	unsigned int m_ID;
	unsigned int m_Frame;
	int m_numFrames;
	int m_Timer;
	int m_Delay;
	
	double m_factX;
	double m_factY;
	double m_centerX;
	double m_centerY;
	b2Vec2 m_Position;
	double m_Rotation;
	Gosu::Color m_ColorMod;
};

#endif
