/*
   Animation.h
   Mizzou Game Engine
 
   Created by Chad Godsey on 11/12/08.
  
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
 
 	Animation encapsulates a set of images that are cycled through
 	to give the illusion of movement.
 	This can be used two ways;
 		as a table of images with arbitrary access
 		as an incrementing series of images with a time delay between frames
 
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
	
	void setPosition( double _x, double _y, Gosu::ZPos _z);
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
	
	void drawCurFrame(double _x, double _y) const;
	void drawFrameAt(int _frame, double _x, double _y) const;
	
	void drawCurFrameZoom(double _x, double _y, double _scale, double _zoom, int _scrWidth, int _scrHeight) const;
	void drawFrameAtZoom(int _frame, double _x, double _y, double _scale, double _zoom, int _scrWidth, int _scrHeight) const;

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
	Gosu::ZPos m_Z;
	
	double m_WinScale;
	double m_factX;
	double m_factY;
	double m_centerX;
	double m_centerY;
	Gosu::Color m_ColorMod;
};

#endif
