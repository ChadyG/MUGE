/*
   Paths.h
   My Unnamed Game Engine
 
   Created by Chad Godsey.
  
 Copyright 2010 BlitThis! studios.

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

#include <Box2D/Box2D.h>
#include <list>

#ifndef PATHS_H
#define PATHS_H

struct Step
{
	b2Vec2 point;
	float time;
};

/// Abstract base of all path types
class Path
{
public:
	Path() : m_TotalTime(0.0) {}

	void setRepeat(bool _repeat) { m_Repeat = _repeat; }

	virtual void addStep(Step _step) = 0;

	virtual void insertStep(Step _step) = 0;

	virtual void deleteStep(int _p) = 0;

	virtual void setStep(int _p, Step _step) = 0;

	virtual b2Vec2 getPosition(float _time) const = 0;

	std::list<Step> getPoints() { return m_Path; }

	std::list<Step> getPoints() const { return m_Path; }

	float getTotalTime() { return m_TotalTime; }

protected:

	std::list< Step > m_Path;
	float m_TotalTime;
	bool m_Repeat;
};

/// Stepped path
/// p(x) = p(k)
/// for x on (x(k), x(k+1))
class Step_Path : public Path
{
public:
	Step_Path() {}

	void addStep(Step _step);

	void insertStep(Step _step);

	void deleteStep(int _p);

	void setStep(int _p, Step _step);

	b2Vec2 getPosition(float _time) const;

private:

};

/// Linear path
/// p(x) = p1 + (p2 - p1)*t
/// t = (x - x(k)) / h
/// h = x(k+1) - x(k)
class Line_Path : public Path
{
public:
	Line_Path() {}

	void addStep(Step _step);

	void insertStep(Step _step);

	void deleteStep(int _p);

	void setStep(int _p, Step _step);

	b2Vec2 getPosition(float _time) const;

private:

};

/// Cubic Hermite Spline using Catmull-Rom tangents
/// for interpolating x on the interval (x(k), x(k+1))
///
/// p(x) = h00(t)*p0 + h10(t)*h*m0 + h01(t)*p1 + h11(t)*h*m1
/// h = x(k+1) - x(k)
/// t = (x - x(k)) / h
///
/// h00(t) = (1 + 2*t)(1 - t)^2
/// h10(t) = t*(1 - t)^2
/// h01(t) = t^2(3 - 2*t)
/// h11(t) = t^2*(t - 1)
///
/// Tangent is found by
/// m(k) = (p(k+1) - p(k-1)) / 2
class Smooth_Path : public Path
{
public:
	Smooth_Path() {}

	void addStep(Step _step);

	void insertStep(Step _step);

	void deleteStep(int _p);

	void setStep(int _p, Step _step);

	b2Vec2 getPosition(float _time) const;

private:

	std::list< b2Vec2 > m_Tangents;

};

/// Cubic Bezier Spline
/// Formed of four points per segment
/// p0 p1 p2 p3
/// p0 and p3 lie on path
/// p1 and p2 approximate path
class Bezier_Path : public Path
{
public:
	Bezier_Path() {}

	void addStep(Step _step);

	void insertStep(Step _step);

	void deleteStep(int _p);

	void setStep(int _p, Step _step);

	b2Vec2 getPosition(float _time) const;

	std::list<b2Vec2> getTangents() { return m_Tangents; }

private:
	
	std::list< b2Vec2 > m_Tangents;

};

#endif