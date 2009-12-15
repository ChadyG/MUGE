/*
	SpriteSheet.h
	My Unnamed Game Engine

	Created by Chad on 11/12/08.
	Copyright 2009 BlitThis! studios. All rights reserved.
 
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


	SpriteSheet encapsulates a set of images that are cycled through
	to give the illusion of movement.
	This can be used two ways;
		as a table of images with arbitrary access
		as an incrementing series of images with a time delay between frames
 
 */
#ifndef SpriteSheet_H
#define SpriteSheet_H

#include <Gosu/Gosu.hpp>
#include <Box2D.h>
#include "../Scene/SceneObject.h"
#include "TranslationModules.h"

typedef boost::shared_ptr<Gosu::Image> t_Image;

class SpriteSheet
{
public:
	SpriteSheet();
	SpriteSheet(std::wstring _filename, int width, int height, int delay=1);
	
	unsigned getID() { return m_ID; }
	void setID( unsigned _id) { m_ID = _id; }
	
	void setImage(std::wstring _filename, int width, int height, int delay=1);
	
	void setScaling(double _factorX, double _factorY);
	void setCenter(double _centerX, double _centerY);
	void setColorMod(Gosu::Color _colorMod);
	
	Gosu::Image& getFrame(unsigned frame);
	const Gosu::Image& getFrame(unsigned frame) const;
	
	Gosu::Image& getCurFrame();
	const Gosu::Image& getCurFrame() const;
	
	//Call this to increment frame based on time delay
	void update();

	void draw(double _x, double _y, Gosu::ZPos _layer, double _zoom = 1.0, double _angle = 0.0) const;
	void drawFrame(int _frame, double _x, double _y, Gosu::ZPos _layer, double _zoom = 1.0, double _angle = 0.0) const;

private:
	std::vector< t_Image > m_Sprites;

	TransMod *m_TransMod;
	
	unsigned int m_ID;
	unsigned int m_Frame;
	int m_numFrames;
	int m_Timer;
	int m_Delay;
	
	double m_factX;
	double m_factY;
	double m_centerX;
	double m_centerY;
	Gosu::Color m_ColorMod;
};

#endif
