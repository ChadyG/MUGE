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
*/
/*

	SpriteSheet encapsulates a set of images that are cycled through
	to give the illusion of movement.
	This can be used two ways;
		as a table of images with arbitrary access
		as an incrementing series of images with a time delay between frames
 
 */
#ifndef SpriteSheet_H
#define SpriteSheet_H

#include <Gosu/Gosu.hpp>

typedef boost::shared_ptr<Gosu::Image> t_Image;

class SpriteSheet
{
public:
	SpriteSheet();
	SpriteSheet(std::wstring _filename, int width, int height, int delay=1);
	
	// Safe version
	void setImage(std::wstring _filename, Gosu::Graphics &_graphics, int width, int height, int delay=1);
	void setImage(std::wstring _filename, int width, int height, int delay=1);
	
	void setScaling(double _factorX, double _factorY);
	void setCenter(double _centerX, double _centerY);
	void setColorMod(Gosu::Color _colorMod);

	float posX() const { return m_posX; }
	float posY() const { return m_posY; }
	float zoom() const { return m_zoom; }
	float angle() const { return m_angle; }
	double layer() const { return m_layer; }
	bool visible() const { return m_visible; }

	void setFrame(int _frame) { m_Frame = Gosu::clamp<int>(_frame, 0, m_numFrames-1); }
	void setSpeed(float _speed) { m_Speed = _speed; }
	void setX(float _x) { m_posX = _x; }
	void setY(float _y) { m_posY = _y; }
	void setZoom(float _zoom) { m_zoom = _zoom; }
	void setAngle(float _angle) { m_angle = _angle; }
	void setLayer(double _layer) { m_layer = _layer; }
	void setVisible(bool _vis) { m_visible = _vis; }
	
	Gosu::Image& getFrame(unsigned frame);
	const Gosu::Image& getFrame(unsigned frame) const;
	
	Gosu::Image& getCurFrame();
	const Gosu::Image& getCurFrame() const;
	
	//Call this to increment frame based on time delay
	void update();

	void draw(double _x, double _y, double _zoom = 1.0, double _angle = 0.0) const;
	void drawFrame(int _frame, double _x, double _y, double _zoom = 1.0, double _angle = 0.0) const;

	// Back asswards support for UI stuff
	void drawAt(double _x, double _y, Gosu::ZPos _layer, double _zoom = 1.0, double _angle = 0.0) const;
	void drawFrameAt(int _frame, double _x, double _y, Gosu::ZPos _layer, double _zoom = 1.0, double _angle = 0.0) const;

private:
	std::vector< t_Image > m_Sprites;

	float m_posX, m_posY, m_zoom, m_angle;
	double m_layer;
	
	unsigned int m_Frame;
	int m_numFrames;
	float m_Timer;
	int m_Delay;
	float m_Speed;
	
	bool m_visible;
	double m_factX;
	double m_factY;
	double m_centerX;
	double m_centerY;
	Gosu::Color m_ColorMod;
};

#endif
