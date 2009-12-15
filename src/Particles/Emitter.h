/*
	Emitter.h
	My Unnamed Game Engine
 
	Created by Chad Godsey on 7/18/09.
 
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
#include <Gosu/Gosu.hpp>
#include <list>

struct Particle
{
	// Position
	double x, y, z, r, theta;
	// Orientation
	double rotation;
	// Size
	double scale, xscale, yscale;
	// Color
	double red, green, blue, alpha;
	
};

/**
 modifiers
 momentum, accelleration, rate
 function types
	addition, negate, multiply, piecewise defined
 connectable
*/

/**
  Emitter Types
 	Point
		Emits from a point, movement based on angle (min, max)
 	Shape
		Emits particles within some shape
			Rectangle
			Circle
 
 */

class Emitter
{
public:
	Emitter();
	
	void emit() { m_Emit = true; }
	void stop() { m_Emit = false; }
	
	virtual void update() = 0;
	virtual void draw(int _x, int _y, Gosu::ZPos _layer) const = 0;
	
private:
	std::list<Particle> m_Particles;
	
	bool m_Emit;
	double m_Frequency;
	int m_Min, m_Max;
	
};